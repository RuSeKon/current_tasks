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
										 m_GameBegun(false), m_Cycle(0),
										 m_Players(0), m_pList(nullptr)
{
	m_pSelector->Add(this);
	m_pList = new Item*[g_MaxGamerNumber];
	//SetMarketLvl(3); Exactly here?
}

Game::~Game()
{
	for(size_t i=0; i < g_MaxGamerNumber; i++)
	{
		if(m_pList[i])
		{
			m_pSelector->Remove(m_pList[i]->plr);
			delete m_pList[i]->plr;
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
		if(m_pList[i]->plr == s)
		{
			m_pSelector->Remove(s);
			delete s;
			delete m_pList[i];
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
		Item *tmp = new Item;
		tmp->plr = p;
		m_pSelector->Add(p);
		
		m_Players++; // NEED TO START GAME
		if(m_Players == g_MaxGamerNumber) m_GameBegun = true;
		m_pList[num] = tmp;
	}
}

void Game::SendAll(const char* message, Player* except)
{
	for(size_t i=0; i < g_MaxGamerNumber; i++)
		if(m_pList[i]->plr != except)
			m_pList[i]->plr->Send(message);
}


void Game::RequestProc(Player* plr, Request& req)
{
	int res{0};
	for(int i=0; i < 8; i++) 
	{
		if(req.GetText() == g_CommandList[i])
			res = i+1;
	}
	switch(res)
	{
		case Market:
			MarketCondition(plr);
			break;
		case AnotherPlayer:
				GetInfo(plr, req);
				break;
/*		case Production:
				Enterprise(plr, req);
				break;
		case Buy:
				
				if(std::get<0>(t) > std::get<0>(m_pBanker->Raw))
				{
					m_pSession->SendMsg("The amount of raw materials"
										" sold by the market is less\n");
					break;
				}
				else if(std::get<1>(t) < std::get<1>(m_pBanker->Raw))
				{
					m_pSession->SendMsg("Your cost is less than market\n");
					break;
				} 
			
				//m_BuyApply = t;
				break;
		case Sell:
				
				if(std::get<0>(t) > s)
				{
					m_pSession->SendMsg("You don't have that many"
										" products\n");
					break;
				}
				//Maybe it's not need
				else if(std::get<1>(t) > std::get<1>(m_pBanker->Raw))
				{
					m_pSession->SendMsg("Your cost is larger than market\n");
					break;
				} 
				//m_SellApply = t;
				break;
		case Build:
				Build(plr);
				break;
		case Turn:
				m_End = true;
				break;
		case Help:
				Send(g_HelpMsg);	
				break;
*/		default:
				plr->Send(g_UnknownReqMsg);
	}
	return;
}

void Game::MarketCondition(Player* plr)
{
	std::unique_ptr<char> msg(new char[strlen(g_MarketCondMsg)+20]);
    sprintf(msg.get(), g_MarketCondMsg, 
						m_BankerRaw[0], m_BankerRaw[1],
						m_BankerProd[0], m_BankerProd[1]);
    plr->Send(msg.get());
}

void Game::GetInfo(Player* plr, Request& req)
{
	int res = req.GetParam(1);
	if(res <= 0 || res > g_MaxGamerNumber)
	{
		plr->Send(g_BadRequestMsg);
		return;
	}
	else
	{
		Player* tmp;
		for(size_t i=0; i < g_MaxGamerNumber; i++)
		{
			if(res == m_pList[i]->plr->m_PlayerNumber)
			{
				tmp = m_pList[i]->plr;
				break;
			}
		}
		std::unique_ptr<char> msg(new char[strlen(g_GetInfoMsg)+20]);
    	sprintf(msg.get(), g_GetInfoMsg, tmp->m_PlayerNumber, 
						   tmp->m_Name, tmp->m_Resources["Money"],
						   tmp->m_Resources["Raw"], tmp->m_Resources["Products"],
						   0, tmp->m_Resources["Factories"],  0);
    	plr->Send(msg.get());
	}
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
	

