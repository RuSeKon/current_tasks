#include <sys/socket.h>
#include <cstdio> 
#include <ctime>
#include <arpa/inet.h>
#include <unistd.h> 
#include <memory>
#include <cstring>
#include <vector> 
#include <algorithm>
#include "share/errproc.h"
#include "share/application.h"
#include "share/game.h"
 

/* SECTION FOR CONSTANT MESSAGES */
static const char g_AlreadyPlayingMsg[]={"Game is already started.\n"};

static const char g_NotNameMsg[]={"Enter only first name (not more ten symbols).\n"};

static const char g_BadRequestMsg[]={"Bad request, try again or type help:)\n"};

static const char g_WelcomeMsg[]={"Welcome to the game %s, " 
						   "your play-number: %d\n"};

static const char g_WelcomeAllMsg[]={"%s number %d"
												" joined to the game!\n"};
//static const char g_GameStartSoonMsg[]={"The game will start soon!:)\n"};
static const char g_InvalidArgumentMsg[]={"Invalid argument, try "
										  "again or type help.\n"};

static const char g_UnknownReqMsg[]={"Unknown request, enter help!\n"};

static const char g_MarketCondMsg[]={"\nCurrent month is %dth\n"
									"_________________________"
									"Players still active:\n"
									"%			    %d\n"
									"Bank sells:  items   min.price\n"
									"%				%d		  %d\n"
									"Bank buys:   items	  max.price\n"
									"%				%d		  %d\n"};

static const char g_GetInfoMsg[]={"\n%s's state of affairs (num: %d):\n"
							"_________________________________"
							"Money: %d\n"
							"Materials: %d\n"
							"Products: %d\n"
							"Regular factorie: %d;\n"
							"Build factorie: %d;\n"};

static const char g_HelpMsg[]={"helpMe\n"};

static const char g_PlayerListMsg[]={"\n%d. %s\n"};

static const char g_BadRawQuantMsg[]={"The bank does not sell that amount.\n"};

static const char g_BadRawCostMsg[]={"Your cost is less than market.\n"};

static const char g_BadProdQuantMsg[]={"Сheck the number of products in the "
															"application.\n"};

static const char g_BadProdCostMsg[]={"Your cost is larger than market.\n"};

static const char g_TooFewFactories[]={"You don't have as many factories to"
																" produce.\n"};

static const char g_InsufficientFunds[]={"Insufficient funds to build so "
														  "many factoryes.\n"};

static const char g_GameNotBegunMsg[]={"The game haven't started yet. " 
														    "Please wait!\n"};

static const char g_AplApplyMsg[] = {"Application apply!\n"};


static const char *g_CommandList[] = {"market\0", "info\0", "pod\0",
                "buy\0", "sell\0", "build\0", "turn\0", "help\0", "infoLst\0"};

enum RequestConstants { //for request processing
	
	reqMarket= 1,
	reqAnotherPlayer = 2,
	reqProduction = 3,
	reqBuy = 4,
	reqSell = 5,
	reqBuild = 6,
	reqTurn = 7,
	reqHelp = 8,
	reqPlayerAll = 9,
};

/////////////////////////////////////////GAME//////////////////////////////////////

Game::Game(EventSelector *sel, int fd) : IFdHandler(fd), m_pSelector(sel),
										 m_GameBegun(false), m_Month(1),
										 m_MarketLevel(0), m_List(), 
										 m_Numbers(nullptr), 
										 m_BankerRaw{0, 0}, m_BankerProd{0, 0}					 
{
	m_pSelector->Add(this);
	m_List.reserve(g_MaxGamerNumber);
	m_Numbers = new int[g_MaxGamerNumber]{0};
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
	
	if(!req.GetText())
	{
		plr->Send(g_BadRequestMsg);
		return;
	}
	else if(!plr->m_Name)
	{
		if(strlen(req.GetText()) > g_MaxName)
		{
			plr->Send(g_NotNameMsg);
			return;
		}
		plr->m_Name = new char[sizeof(req.GetText())];
		strcpy(plr->m_Name, req.GetText());

		std::unique_ptr<char> msg(new char[58/*strlen(g_WelcomeMsg)+13*/]);
		sprintf(msg.get(), g_WelcomeMsg, plr->m_Name, plr->m_PlayerNumber);
		plr->Send(msg.get());
		
		sprintf(msg.get(), g_WelcomeAllMsg, p->m_Name, p->m_PlayerNumber);
		SendAll(msg.get(), plr);
		return;
	}

	if(!GameBegun())
	{
		plr->Send(g_GameNotBegunMsg);
		return;
	}

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
		case reqMarket:
		case reqPlayerAll:		
		case reqAnotherPlayer:
				GetInfo(plr, req, res);
				break;
		case reqProduction:
				Enterprise(plr, req);
				break;
		case reqBuy:
				BuyReq(plr, req);
				break;
		case reqSell:
				SellReq(plr, req);
				break;
		case reqBuild:
				BuildFactory(plr);
				break;
		case reqTurn:
				plr->m_End = true;
				plr->Send("\nWait for other players to finish their turn.\n");
				break;
		case reqHelp:
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
	
	if(all == reqPlayerAll)
	{
		std::unique_ptr<char> msg(new char[(strlen(g_PlayerListMsg)+9)*
													 g_MaxGamerNumber]);
		
		char* ptr = msg.get();
		size_t b{0};

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
	else if(all == reqMarket)
	{
		std::unique_ptr<char> msg(new char[strlen(g_MarketCondMsg) + 14]);
    	sprintf(msg.get(), g_MarketCondMsg, m_Month,
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
		if(res == 0)
		{
			tmp = plr;
		}
		else
		{
			for(auto x : m_List)
			{
				if(res == x->m_PlayerNumber)
				{	
					tmp = x;
					break;
				}
			}
		}
		std::unique_ptr<char> msg(new char[strlen(g_GetInfoMsg)+32]);
    	sprintf(msg.get(), g_GetInfoMsg, tmp->m_Name, 
						   tmp->m_PlayerNumber, tmp->m_Resources[resMoney],
						   tmp->m_Resources[resRaw], tmp->m_Resources[resProd],
						   static_cast<int>(tmp->m_ConstrFactories.size()), 
						   tmp->m_Resources[resFactory]);
    	plr->Send(msg.get());
	}
}

void Game::Enterprise(Player* plr, const Request& arg)
{
	int amount = plr->m_Enterprise + arg.GetParam(1);
	if(amount > plr->m_Resources[resFactory])
	{
		plr->Send(g_TooFewFactories);
	}
	else
	{
		plr->m_Enterprise = amount;
	}
}

void Game::BuyReq(Player* plr, const Request& arg) 
{
	if(arg.GetParam(1) == 0 || arg.GetParam(2) == 0)
	{
		plr->Send(g_BadRequestMsg);
		return;
	}

	int amount = m_BankerRaw[0];
	int cost = m_BankerRaw[1];


	if(arg.GetParam(1) > amount)
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

	int amount = m_BankerProd[0];
	int cost = m_BankerProd[1];


	if(arg.GetParam(1) > amount || arg.GetParam(1) > plr->m_Resources[resProd])
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
	
	plr->Send(g_AplApplyMsg);
}

void Game::BuildFactory(Player* plr)
{
	if(plr->m_Resources[resMoney] < 2500)
	{
		plr->Send(g_InsufficientFunds);
		return;
	}
	plr->m_Resources[resMoney] -= 2500;
	//List value is month of start construction
	plr->m_ConstrFactories.push_back(m_Month); 
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


void Game::Auction(std::vector<Application>& src, int flag)
{
	int left{0};

	if(flag == Raw)
	{
		std::sort(src.begin(), src.end(), 
				 [](const Application& a, const Application& b)
				 {return a.m_ResrsCost > b.m_ResrsCost;});
		left = m_BankerRaw[0];
	}
	else if(flag == Prod)
	{
		std::sort(src.begin(), src.end(), 
				 [](const Application& a, const Application& b)
				 {return a.m_ResrsCost < b.m_ResrsCost;});
		left = m_BankerProd[0];
	}
	else
		return;

	std::vector<Application> tmp;
	
	int pi{0};
	size_t it{0};

	while(left)
	{	
		// collect of equal prices
		for(pi = it; it < src.size() && 
			src[it].m_ResrsCost == src[pi].m_ResrsCost; it++) 
		{	
			tmp.push_back(src[it]);
		}
		
		int sum{0};
		for(auto x : tmp)
			sum += x.m_ResrsAmount;

		if(sum <= left)
		{
			for(size_t i=0; i < tmp.size(); i++)
			{
				left -= tmp[i].plr->ApplicationAccepted(Full, flag);
			}
			tmp.clear();
			continue;
		}
		else
		{
			int how{0};

			for(size_t i=0; left && i < tmp.size(); i++)
			{
				if(i == tmp.size()-1)
				{	
					how = left;
					i = -1;
				}
				else
				{
					srand(time(NULL));
					how = rand()%left;
				}
				left -= tmp[i].plr->ApplicationAccepted(how, flag);
			}
			continue;
		}
	}
	return;
}

void Game::NextMonth()
{
	std::vector<Application> RawContainer;
	std::vector<Application> ProdContainer;

	for(auto x : m_List)
	{
////////*Cost write-off*/
		x->m_Resources[resMoney] -= (300*x->m_Resources[resRaw]) +
									 (500*x->m_Resources[resProd]) +
									 (1000*x->m_Resources[resFactory]);
		
		if(x->m_Resources[resMoney] < 0)
		{
			RemovePlayer(x);
			//NEED TO SEND MESSAGE
			continue;
		}

////////*Product enterprise*/
		if(x->m_Enterprise*2000 > x->m_Resources[resMoney])
		{
			int e = x->m_Resources[resMoney]/2000;
			e = x->m_Resources[resRaw] >= e ? e : x->m_Resources[resRaw];
			
			//NEED TO SEND MESSAGE
			x->m_Resources[resMoney] -= e*2000;
			x->m_Resources[resProd] += e;
			x->m_Enterprise -= e; ///Not clear m_Enterprise counter
		}
		else if(x->m_Resources[resRaw] < x->m_Enterprise)
		{
			int e = x->m_Resources[resRaw];
			//NEED TO SEND MESSAGE
			x->m_Resources[resMoney] -= e*2000;
			x->m_Resources[resProd] += e;
			x->m_Enterprise -= e;
		}
		else
		{
			x->m_Resources[resMoney] -= 2000 * x->m_Enterprise;
			x->m_Resources[resProd] += x->m_Enterprise;
			x->m_Enterprise = 0;
		}

////////*Factory construction*/
		for(auto a = x->m_ConstrFactories.begin(); 
			a != x->m_ConstrFactories.end(); a++)
		{
			if(m_Month - *a == 4)
				x->m_Resources[resMoney] -= 2500;
			else if(m_Month - *a == 5)
			{
				x->m_Resources[resFactory] +=1;
				x->m_ConstrFactories.erase(a);
			}
		}

		if(x->m_Resources[resMoney] < 0)
		{
			RemovePlayer(x);
			//NEED TO SEND MESSAGE
			continue;
		}

///////*Change of market level*/
		ChangeMarketLvl();
		x->m_End = false;
		RawContainer.push_back(Application(x, x->m_PlayerRaw[0], 
											x->m_PlayerRaw[1]));
		ProdContainer.push_back(Application(x, x->m_PlayerProd[0], 
											x->m_PlayerProd[1]));
	}

	SendAll("\nAuction starting!\n", 0);
	Auction(RawContainer, Raw);
	Auction(ProdContainer, Prod);
	
	m_Month++;
}
