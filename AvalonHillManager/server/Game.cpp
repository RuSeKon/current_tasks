#include <sys/socket.h>
#include <cstdio> 
#include <ctime>
#include <unistd.h>
#include <arpa/inet.h>
#include <memory>
#include <cstring>
#include <vector> 
#include <tuple>
#include <algorithm>
#include "errproc.h"
#include "application.h"
#include "game.h"
 

/* SECTION FOR CONSTANT MESSAGES */
static const char g_AlreadyPlayingMsg[]={"\nSorry, game is already started." 
						" You can play next one\n"};
static const char g_BadRequestMsg[]={"\nBad request, please try again! Or type"
									   " help:)\n"};
static const char g_BoughtResMsg[]={"\nYour bought %d units of resources at a "
								"price of %d $.\n"};
static const char g_SellResMsg[]={"\nYour sell %d units of products at a price "
								"of %d $.\n"};

static const char g_WelcomeAllMsg[]={"\nPlayer number %d joined to the game!\n"};

static const char g_GameStartSoonMsg[]={"\nThe game will start soon!:)\n"};
static const char g_InvalidArgumentMsg[]={"\nInvalid argument, please try again!"
											" Or type help:)\n"};

static const char g_UnknownReqMsg[]={"\nUnknown request, please enter help!\n"};
static const char g_MarketCondMsg[]={"\n            In the current month:  "
										"            \nQuantity of materials sold:"
										" %d, by min price $%d /unit.\nQuantity "
										"of purchaced products: %d, by max price "
										"$%d /unit.\n"};
static const char g_GetInfoMsg[]={"\n%s's state of affairs (num: %d):\n"
								"Money: %d;\nMaterials: %d;\nProducts: %d;\n"
								"Regular factorie: %d;\nBuild factorie: %d;\n"};
static const char g_HelpMsg[]={"helpMe\n"};
static const char g_PlayerListMsg[]={"\n%d. %s\n"};
static const char g_BadRawQuantMsg[]={"\nThe amount of raw materials sold by the market is less.\n"};
static const char g_BadRawCostMsg[]={"\nYour cost is less than market.\n"};
static const char g_BadProdQuantMsg[]={"\nYou don't have that many products,or bank don't buy that quantity.\n"};
static const char g_BadProdCostMsg[]={"\nYour cost is larger than market.\n"};
static const char g_TooFewFactories[]={"\nYou don't have as many factories to produce.\n"};
static const char g_InsufficientFunds[]={"\nInsufficient funds to build so many factoryes.\n"};


static const char *g_CommandList[] = {"market\0", "info\0", "pod\0",
                     "buy\0", "sell\0", "build\0", "turn\0", "help\0", "infoLst\0"};

/////////////////////////////////////////GAME//////////////////////////////////////

Game::Game(EventSelector *sel, int fd) : IFdHandler(fd), m_pSelector(sel),
										 m_GameBegun(false), m_Month(1),
										 m_MarketLevel(0), m_BankerRaw{0, 0},
										 m_BankerProd{0, 0}					 
{
	m_pSelector->Add(this);
	m_List.reserve(g_MaxGamerNumber);
	m_Numbers.reserve(g_MaxGamerNumber);
	for(size_t i=0; i < g_MaxGamerNumber; i++)
		m_Numbers.push_back(0);
}

Game::~Game()
{
	for(auto x = m_List.begin(); x != m_List.end(); x++)
	{
		m_pSelector->Remove(*x);
		delete *x;
	}
	m_pSelector->Remove(this);
}

Game *Game::GameStart(EventSelector *sel, int port)
{
	struct sockaddr_in addr;

	int ls = socket(AF_INET, SOCK_STREAM, 0);
	if(ls == -1)
		return nullptr;

	int opt = 1;
	setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	int res = bind(ls, (struct sockaddr*) &addr, sizeof(addr));
	if(res == -1)
		return nullptr;

	res = listen(ls, 16);
	if(res == -1)
	return nullptr;
	
	return new Game(sel, ls);
}

void Game::RemovePlayer(Player *s)
{
	for(auto x = m_List.begin(); x != m_List.end(); x++)
	{
		if(*x == s)
		{
			m_pSelector->Remove(*x);
			delete *x;
			m_List.erase(x);
			m_Numbers[(*x)->m_PlayerNumber -1] = 0;
			break;
		}
	}
}

void Game::VProcessing(bool r, bool w)
{
	if(!r)
		return;
	int session_descriptor;
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);

	session_descriptor = accept(GetFd(), (struct sockaddr*) &addr, &len);
	if(session_descriptor == -1)
		return;
	
	size_t num;
    for(num = 0; num < g_MaxGamerNumber; num++)
	{
        if(!m_Numbers[num])
            break;	
	}
			
	Player *p = new Player(this, session_descriptor, num+1);
	
	if(m_GameBegun)
	{
		p->Send(g_AlreadyPlayingMsg);
		delete p;
	}
	else
	{
		m_pSelector->Add(p);
		
		m_List.push_back(p);
		m_Numbers[num] = 1;
		if(m_List.size() == g_MaxGamerNumber) 
		{
			m_GameBegun = true;
			SendAll("Game begining!\n", nullptr);
			SetMarketLvl(3);
		}


		std::unique_ptr<char> msg(new char[strlen(g_WelcomeAllMsg)+3]);
		sprintf(msg.get(), g_WelcomeAllMsg, p->m_PlayerNumber);
		SendAll(msg.get(), p);
	}
}

void Game::SendAll(const char* message, Player* except)
{
	for(auto x : m_List)
		if(x != except)
			x->Send(message);
}



void Game::RequestProc(Player* plr, const Request& req)
{
	int res{0};


	for(size_t i=0; i < g_CommandListSize; i++) 
	{
		if(!strcmp(req.GetText(), g_CommandList[i]))
		{
			res = i+1;
			break;
		}
	}
	switch(res)
	{
		case Market:
		case PlayerAll:		
		case AnotherPlayer:
				GetInfo(plr, req, res);
				break;
		case Production:
				Enterprise(plr, req);
				break;
		case Buy:
				BuyReq(plr, req);
				break;
		case Sell:
				SellReq(plr, req);
				break;
		case Build:
				BuildFactory(plr);
				break;
		case Turn:
				plr->m_End = true;
				break;
		case Help:
				plr->Send(g_HelpMsg);	
				break;
		
		default:
				plr->Send(g_UnknownReqMsg);
	}

	for(auto x : m_List)
		if(!x->m_End)
			return;
	
	NextMonth();
	return;
}

void Game::GetInfo(Player* plr, const Request& req, int all)
{
	
	if(all == PlayerAll)
	{
		std::unique_ptr<char> msg(new char[(strlen(g_PlayerListMsg)+9)*g_MaxGamerNumber]);
		
		char* ptr = msg.get();
		int b{0};

		for(auto x : m_List)
		{
				b = sprintf(ptr, g_PlayerListMsg, 
								   x->m_PlayerNumber,
								   x->m_Name);
				ptr += b;
				b=0;
		}
		plr->Send(msg.get());
	}
	else if(all == Market)
	{
		std::unique_ptr<char> msg(new char[strlen(g_MarketCondMsg) + 13]);
    	sprintf(msg.get(), g_MarketCondMsg, 
							m_BankerRaw[0], m_BankerRaw[1],
							m_BankerProd[0], m_BankerProd[1]);
    	plr->Send(msg.get());
	}
	else
	{
		int res = req.GetParam(1);
		if(res < 0 || res > static_cast<int>(m_List.size()))
		{
			plr->Send(g_BadRequestMsg);
			return;
		}

		Player* tmp;
		for(auto x : m_List)
		{
			if(res == 0)
			{
				tmp = plr;
				break;
			}
			else if(res == x->m_PlayerNumber)
			{
				tmp = x;
				break;
			}
		}
		std::unique_ptr<char> msg(new char[strlen(g_GetInfoMsg)+32]);
    	sprintf(msg.get(), g_GetInfoMsg, tmp->m_Name, 
						   tmp->m_PlayerNumber, tmp->m_Resources[Money],
						   tmp->m_Resources[Raw], tmp->m_Resources[Prod],
						   static_cast<int>(tmp->m_ConstrFactories.size()), 
						   tmp->m_Resources[Factory]);
    	plr->Send(msg.get());
	}
}

void Game::Enterprise(Player* plr, const Request& arg)
{
	int quantity = plr->m_Enterprise + arg.GetParam(1);
	if(quantity > plr->m_Resources[Factory])
	{
		plr->Send(g_TooFewFactories);
	}
	else
	{
		plr->m_Enterprise = quantity;
	}

}

void Game::BuyReq(Player* plr, const Request& arg) 
{
	if(arg.GetParam(1) == 0 || arg.GetParam(2) == 0)
	{
		plr->Send(g_BadRequestMsg);
		return;
	}

	int quantity = m_BankerRaw[0];
	int cost = m_BankerRaw[1];


	if(arg.GetParam(1) > quantity)
	{
			plr->Send(g_BadRawQuantMsg);
			return;
	}
	else if(arg.GetParam(2) < cost)
	{
			plr->Send(g_BadRawCostMsg);
			return;
	}
	
	for(int i=0; i < 2; i++)
			plr->m_PlayerRaw[i] = arg.GetParam(i+1);
	plr->Send("\nApplication apply!\n");
}

void Game::SellReq(Player* plr, const Request& arg)
{
	if(arg.GetParam(1) == 0 || arg.GetParam(2) == 0)
	{
		plr->Send(g_BadRequestMsg);
		return;
	}

	int quantity = m_BankerProd[0];
	int cost = m_BankerProd[1];


	if(arg.GetParam(1) > quantity || arg.GetParam(1) < plr->m_Resources[Prod])
	{
			plr->Send(g_BadProdQuantMsg);
			return;
	}
	else if(arg.GetParam(2) > cost)
	{
			plr->Send(g_BadProdCostMsg);
			return;
	}
	
	for(int i=0; i < 2; i++)
			plr->m_PlayerProd[i] = arg.GetParam(i+1);
	
	plr->Send("\nApplication apply!\n");
}

void Game::BuildFactory(Player* plr)
{
	if(plr->m_Resources[Money] < 2500)
	{
		plr->Send(g_InsufficientFunds);
		return;
	}
	plr->m_Resources[Money] -= 2500;
	plr->m_ConstrFactories.push_back(m_Month); //List value is month of start construction
}

void Game::SetMarketLvl(int num)
{
	int PlayerCount = m_List.size();
	float multi{0};
	multi = 1+((num-1)*0.5);
	m_BankerRaw[0] = multi*PlayerCount;

	multi = 3-((num-1)*0.5);
	m_BankerProd[0] = multi*PlayerCount;

	m_BankerProd[1] = 6500 - ((num-1)*500);
	
	if(num <=3)
		m_BankerRaw[1] = 800 - ((num-1)*150);
	else
		m_BankerRaw[1] = 500 - ((num-3)*100);

	m_MarketLevel = num;
}

void Game::ChangeMarketLvl()
{
	srand(time(NULL));

	/*from Numerical Recipes in C: The Art of Scientific Computing*/
	int rnd = 1 + (int)(12.0*rand()/(RAND_MAX+1.0)); //1 <= rnd <= 12
	int i{0};

	
	for(int res=0; res < rnd; i++)
		res += g_MarketLevels[m_MarketLevel][i];
	SetMarketLvl(i);
}


void Game::AuctionRaw()
{
	auto compare = [](Player* a, Player* b){return a->m_PlayerRaw[0] > b->m_PlayerRaw[0];};

	std::vector<Player*> tmp;
	std::sort(m_List.begin(), m_List.end(), compare);

	int left = m_BankerRaw[0];
	int pi{0};
	size_t it{0};

	while(left)
	{	
		// collect of equal prices
		for(pi = it; it < m_List.size() && 
			m_List[it]->m_PlayerRaw[1] == m_List[pi]->m_PlayerRaw[1]; it++) 
		{	
			tmp.push_back(m_List[it]);
		}
		
		int sum{0};
		for(auto x : tmp)
			sum += x->m_PlayerRaw[0];

		if(sum <= left)
		{
			for(size_t i=0; i < tmp.size(); i++)
			{
				tmp[i]->m_Resources[Raw] += tmp[i]->m_PlayerRaw[0];
				tmp[i]->m_Resources[Money] -= tmp[i]->m_PlayerRaw[0] 
											* tmp[i]->m_PlayerRaw[1];
				std::unique_ptr<char> msg(new char[strlen(g_BoughtResMsg)+8]);
				sprintf(msg.get(), g_BoughtResMsg, tmp[i]->m_PlayerRaw[0],
												   tmp[i]->m_PlayerRaw[1]);
				tmp[i]->Send(msg.get());
				left -= tmp[i]->m_PlayerRaw[0];
			}
			tmp.clear();
			break;
		}
		else
		{
			int how{0};

			for(size_t i=0; i < tmp.size() && left; i++)
			{
				if(i == tmp.size()-1)
				{
					how = left;
				}
				else
				{
					srand(time(NULL));
					how = rand()%left;
				}	
				tmp[i]->m_Resources[Raw] += how;
				tmp[i]->m_Resources[Money] -= how * tmp[i]->m_PlayerRaw[1];

				std::unique_ptr<char> msg(new char[strlen(g_BoughtResMsg)+8]);
				sprintf(msg.get(), g_BoughtResMsg, how, tmp[i]->m_PlayerRaw[1]); 
				tmp[i]->Send(msg.get());
				left -= how;
			}
			return; //maybe break for left check
		}

		for(auto x : m_List)
		{
			x->m_PlayerRaw[0] = 0;
			x->m_PlayerRaw[1] = 0;
		}
	}
}

void Game::AuctionProd()
{
	auto compare = [](Player* a, Player* b){return a->m_PlayerProd[0] < b->m_PlayerProd[0];};

	std::vector<Player*> tmp;
	std::sort(m_List.begin(), m_List.end(), compare);

	int left = m_BankerProd[0];
	int pi{0};
	size_t it{0};

	while(left)
	{	
		// collect of equal prices
		for(pi = it; it < m_List.size() && 
			m_List[it]->m_PlayerProd[1] == m_List[pi]->m_PlayerProd[1]; it++) 
		{	
			tmp.push_back(m_List[it]);
		}
		
		int sum{0};
		for(auto x : tmp)
			sum += x->m_PlayerProd[0];

		if(sum <= left)
		{
			for(size_t i=0; i < tmp.size(); i++)
			{
				tmp[i]->m_Resources[Prod] -= tmp[i]->m_PlayerProd[0];
				tmp[i]->m_Resources[Money] += tmp[i]->m_PlayerProd[0] 
											* tmp[i]->m_PlayerProd[1];
				std::unique_ptr<char> msg(new char[strlen(g_SellResMsg)+8]);
				sprintf(msg.get(), g_SellResMsg, tmp[i]->m_PlayerProd[0],
												   tmp[i]->m_PlayerProd[1]);
				tmp[i]->Send(msg.get());
				left -= tmp[i]->m_PlayerProd[0];
			}
			tmp.clear();
			break;
		}
		else
		{
			int how{0};

			for(size_t i=0; i < tmp.size() && left; i++)
			{
				if(i == tmp.size()-1)
				{
					how = left;
				}
				else
				{
					srand(time(NULL));
					how = rand()%left;
				}	
				tmp[i]->m_Resources[Prod] -= how;
				tmp[i]->m_Resources[Money] += how * tmp[i]->m_PlayerProd[1];

				std::unique_ptr<char> msg(new char[strlen(g_SellResMsg)+8]);
				sprintf(msg.get(), g_SellResMsg, how, tmp[i]->m_PlayerProd[1]); 
				tmp[i]->Send(msg.get());
				left -= how;
			}
			return; //maybe break for left check
		}

		for(auto x : m_List)
		{
			x->m_PlayerProd[0] = 0;
			x->m_PlayerProd[1] = 0;
		}
	}
}

void Game::NextMonth()
{
	AuctionRaw();
	AuctionProd();
	
	for(auto x : m_List)
	{
////////*Cost write-off*/
		x->m_Resources[Money] -= (300*x->m_Resources[Raw]) +
									 (500*x->m_Resources[Prod]) +
									 (1000*x->m_Resources[Factory]);
		
		if(x->m_Resources[Money] < 0)
		{
			RemovePlayer(x);
			//NEED TO SEND MESSAGE
			continue;
		}

////////*Product enterprise*/
		if(x->m_Enterprise*2000 > x->m_Resources[Money])
		{
			int e = x->m_Resources[Money]/2000;
			e = x->m_Resources[Raw] >= e ? e : x->m_Resources[Raw];
			
			//NEED TO SEND MESSAGE
			x->m_Resources[Money] -= e*2000;
			x->m_Resources[Prod] += e;
			x->m_Enterprise -= e; ///Not clear m_Enterprise counter
		}
		else if(x->m_Resources[Raw] < x->m_Enterprise)
		{
			int e = x->m_Resources[Raw];
			//NEED TO SEND MESSAGE
			x->m_Resources[Money] -= e*2000;
			x->m_Resources[Prod] += e;
			x->m_Enterprise -= e;
		}
		else
		{
			x->m_Resources[Money] -= 2000 * x->m_Enterprise;
			x->m_Resources[Prod] += x->m_Enterprise;
			x->m_Enterprise = 0;
		}

////////*Factory construction*/
		for(auto a = x->m_ConstrFactories.begin(); 
			a != x->m_ConstrFactories.end(); a++)
		{
			if(m_Month - *a == 4)
				x->m_Resources[Money] -= 2500;
			else if(m_Month - *a == 5)
			{
				x->m_Resources[Factory] +=1;
				x->m_ConstrFactories.erase(a);
			}
		}

		if(x->m_Resources[Money] < 0)
		{
			RemovePlayer(x);
			//NEED TO SEND MESSAGE
			continue;
		}

///////*Change of market level*/
		ChangeMarketLvl();
		x->m_End = false;
	}
	m_Month++;
}
