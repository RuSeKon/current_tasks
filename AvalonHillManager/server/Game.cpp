#include <sys/socket.h>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <memory>
#include <cstring>
#include "errproc.h"
#include "application.h"
#include "game.h"
 

/////////////////////////////////////////GAME//////////////////////////////////////

Game::Game(EventSelector *sel, int fd) : IFdHandler(fd), m_pSelector(sel),
										 m_GameBegun(false), m_Month(1),
										 m_Players(0), m_MarketLevel(0),
										 m_pList(nullptr), m_BankerRaw{0, 0}, 
										 m_BankerProd{0, 0}
{
	m_pSelector->Add(this);
	m_pList = new Player*[g_MaxGamerNumber]{0};
}

Game::~Game()
{
	for(size_t i=0; i < g_MaxGamerNumber; i++)
	{
		if(m_pList[i])
		{
			m_pSelector->Remove(m_pList[i]);
			delete m_pList[i];
		} 
	}
	if(m_pList) delete[] m_pList;
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
	for(size_t i=0; i < g_MaxGamerNumber; i++)
	{
		if(m_pList[i] == s)
		{
			m_pSelector->Remove(s);
			delete s;
			m_pList[i] = nullptr;
			break;
		}
	}
	m_Players--;
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
        if(!m_pList[num])
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

		m_Players++; // NEED TO START GAME
		
		m_pList[num] = p;
		if(m_Players == g_MaxGamerNumber) 
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
	for(size_t i=0; i < g_MaxGamerNumber; i++)
		if(m_pList[i] != except && m_pList[i])
			m_pList[i]->Send(message);
}


void Game::RequestProc(Player* plr, Request& req)
{
	int res{0};
	for(size_t i=0; i < g_CommandList.size(); i++) 
	{
		if(!strcmp(req.GetText(), g_CommandList[i].c_str()))
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

	for(int i=0; i < g_MaxGamerNumber; i++)
	{
		if(m_pList[i])
		{
			if(!m_pList[i]->m_End)
				return;
		}
	}
	Cycle(); //////////////////////////////////////////////////////
	return;
}

void Game::GetInfo(Player* plr, Request& req, int all)
{
	
	if(all == PlayerAll)
	{
		std::unique_ptr<char> msg(new char[(strlen(g_PlayerListMsg)+9)*g_MaxGamerNumber]);
		
		char* ptr = msg.get();
		for(int i=0, b=0; i < g_MaxGamerNumber; i++)
		{
			if(m_pList[i])
			{
				b = sprintf(ptr, g_PlayerListMsg, 
								   m_pList[i]->m_PlayerNumber,
								   m_pList[i]->m_Name);
				ptr += b;
				b=0;
			}
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
		if(res < 0 || res > m_Players)
		{
			plr->Send(g_BadRequestMsg);
			return;
		}

		Player* tmp;
		for(size_t i=0; i < g_MaxGamerNumber; i++)
		{
			if(res == 0)
			{
				tmp = plr;
				break;
			}
			else if(res == m_pList[i]->m_PlayerNumber)
			{
				tmp = m_pList[i];
				break;
			}
		}
		std::unique_ptr<char> msg(new char[strlen(g_GetInfoMsg)+32]);
    	sprintf(msg.get(), g_GetInfoMsg, tmp->m_Name, 
						   tmp->m_PlayerNumber, tmp->m_Resources["Money"],
						   tmp->m_Resources["Raw"], tmp->m_Resources["Prod"],
						   static_cast<int>(tmp->m_ConstrFactories.size()), 
						   tmp->m_Resources["Factories"]);
    	plr->Send(msg.get());
	}
}

void Game::Enterprise(Player* plr, Request& arg)
{
	int quantity = plr->m_Enterprise + arg.GetParam(1);
	if(quantity > plr->m_Resources["Factories"])
	{
		plr->Send(g_TooFewFactoryes);
	}
	else
	{
		plr->m_Enterprise = quantity;
	}

}

void Game::BuyReq(Player* plr, Request& arg) 
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

void Game::SellReq(Player* plr, Request& arg)
{
	if(arg.GetParam(1) == 0 || arg.GetParam(2) == 0)
	{
		plr->Send(g_BadRequestMsg);
		return;
	}

	int quantity = m_BankerProd[0];
	int cost = m_BankerProd[1];


	if(arg.GetParam(1) > quantity || arg.GetParam(1) < plr->m_Resources["Prod"])
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
	if(plr->m_Resources["Money"] < 2500)
	{
		plr->Send(g_InsufficientFunds);
		return;
	}
	plr->m_Resources["Money"] -= 2500;
	plr->m_ConstrFactories.push_back(m_Month); //List value is month of start construction
}

void Game::SetMarketLvl(int num)
{
	float multi{0};
	multi = 1+((num-1)*0.5);
	m_BankerRaw[0] = multi*m_Players;

	multi = 3-((num-1)*0.5);
	m_BankerProd[0] = multi*m_Players;

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
	int rnd = rand();
	int i{0};

	for(int res=0; res < rnd; i++)
		res += g_MarketLevels[m_MarketLevel][i];
	SetMarketLvl(i);
}
void Game::Cycle()
{
	Player* tmp;

	for(int i=0; i < g_MaxGamerNumber; i++)
	{
		if(m_pList[i]) 
			tmp = m_pList[i];
		else
			continue;

		//Auction previosly/////////////////////////////

////////*Cost write-off*/
		tmp->m_Resources["Money"] -= (300*tmp->m_Resources["Raw"]) +
									 (500*tmp->m_Resources["Prod"]) +
									 (1000*tmp->m_Resources["Factory"]);
		
		if(tmp->m_Resources["Money"] < 0)
		{
			RemovePlayer(tmp);
			//NEED TO SEND MESSAGE
			continue;
		}

////////*Product enterprise*/
		if(tmp->m_Enterprise*2000 > tmp->m_Resources["Money"])
		{
			int e = tmp->m_Resources["Moneey"]/2000;
			e = tmp->m_Resources["Raw"] >= e ? e : tmp->m_Resources["Raw"];
			
			//NEED TO SEND MESSAGE
			tmp->m_Resources["Money"] -= e*2000;
			tmp->m_Resources["Prod"] += e;
			tmp->m_Enterprise -= e; ///Not clear m_Enterprise counter
		}
		else if(tmp->m_Resources["Raw"] < tmp->m_Enterprise)
		{
			int e = tmp->m_Resources["Raw"];
			//NEED TO SEND MESSAGE
			tmp->m_Resources["Money"] -= e*2000;
			tmp->m_Resources["Prod"] += e;
			tmp->m_Enterprise -= e;
		}
		else
		{
			tmp->m_Resources["Money"] -= 2000 * tmp->m_Enterprise;
			tmp->m_Resources["Prod"] += tmp->m_Enterprise;
			tmp->m_Enterprise = 0;
		}

////////*Factory construction*/
		for(auto x = tmp->m_ConstrFactories.begin(); 
			x != tmp->m_ConstrFactories.end(); x++)
		{
			if(m_Month - *x == 4)
				tmp->m_Resources["Money"] -= 2500;
			else if(m_Month - *x == 5)
			{
				tmp->m_Resources["Factory"] +=1;
				tmp->m_ConstrFactories.erase(x);
			}
		}

		if(tmp->m_Resources["Money"] < 0)
		{
			RemovePlayer(tmp);
			//NEED TO SEND MESSAGE
			continue;
		}

///////*Change of market level*/
		ChangeMarketLvl();
	}
	m_Month++;
}
	

