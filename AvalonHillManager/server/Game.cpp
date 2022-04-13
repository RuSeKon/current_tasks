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
										 m_Players(0), m_pList(nullptr),
										 m_BankerRaw{0, 0}, m_BankerProd{0, 0}
{
	m_pSelector->Add(this);
	m_pList = new Player*[g_MaxGamerNumber]{0};
	//SetMarketLvl(3); Exactly here?
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
		if(m_Players == g_MaxGamerNumber) m_GameBegun = true;
		m_pList[num] = p;
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
	for(int i=0; i < 8; i++) 
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
			MarketCondition(plr);
			break;
		case AnotherPlayer:
				GetInfo(plr, req);
				break;
		case Production:
				Enterprise(plr, req);
				break;
		case Buy:
		case Sell:
				/*
				
					"You don't have that many"
										" products\n");

					"Your cost is larger than market\n");
				*/
				AuctionReq(plr, req, res);
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
	return;
}

void Game::MarketCondition(Player* plr)
{
	std::unique_ptr<char> msg(new char[g_MarketCondMsgSize]);
    sprintf(msg.get(), g_MarketCondMsg, 
						m_BankerRaw[0], m_BankerRaw[1],
						m_BankerProd[0], m_BankerProd[1]);
    plr->Send(msg.get());
}

void Game::GetInfo(Player* plr, Request& req)
{
	int res = req.GetParam(1);
	if(res <= 0 || res > m_Players)
	{
		plr->Send(g_BadRequestMsg);
		return;
	}
	else
	{
		Player* tmp;
		for(size_t i=0; i < g_MaxGamerNumber; i++)
		{
			if(res == m_pList[i]->m_PlayerNumber)
			{
				tmp = m_pList[i];
				break;
			}
		}
		std::unique_ptr<char> msg(new char[g_GetInfoMsgSize]);
    	sprintf(msg.get(), g_GetInfoMsg, tmp->m_Name, 
						   tmp->m_PlayerNumber, tmp->m_Resources["Money"],
						   tmp->m_Resources["Raw"], tmp->m_Resources["Products"],
						   static_cast<int>(tmp->m_ConstrFactories.size()), 
						   tmp->m_Resources["Factories"]);
    	plr->Send(msg.get());
	}
}

void Game::Enterprise(Player* plr, Request& arg)
{
	int quantity = plr->m_Enterpise + arg.GetParam(1);
	if(quantity > plr->m_Resources["Factories"])
	{
		plr->Send("You don't have as many factories to produce.\n");
	}
	else
	{
		plr->m_Enterpise = quantity;
	}

}

// Stupid
void Game::AuctionReq(Player* plr, Request& req, int res) 
{
	if(req.GetParam(1) == 0 || !req.GetParam(2) == 0)
	{
		plr->Send(g_BadRequestMsg);
		return;
	}

	int quantity;
	int cost;

	if(res == Buy)
	{	
		quantity = m_BankerRaw[0];
		cost = m_BankerRaw[0];	
	}
	else
	{
		quantity = m_BankerProd[0];
		cost = m_BankerProd[1];
	}

	if(req.GetParam(1) > quantity)
	{
			plr->Send("The amount of raw materials" // need check for res == Buy
										" sold by the market is less\n");
			return;
	}
	else if(req.GetParam(2) < cost)
	{
			plr->Send("Your cost is less than market\n");
			return;
	}
	
	for(int i=0; i < 2; i++)
	{
		if(res == Buy)
			plr->m_PlayerRaw[i] = req.GetParam(i+1);
		else
			plr->m_PlayerProd[i] = req.GetParam(i+1);
		
	}
}

void Game::BuildFactory(Player* plr)
{
	if(plr->m_Resources["Money"] < 2500)
	{
		plr->Send("Low money");
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
}
	

