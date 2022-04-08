#include <sys/socket.h>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include "errproc.h"
#include "application.h"
#include "game.h"
 

/////////////////////////////////////////GAME//////////////////////////////////////

Game::Game(EventSelector *sel, int fd) : IFdHandler(fd), m_pSelector(sel) 
{
	m_pSelector->Add(this);
}

Game::~Game()
{
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
	for(auto x : m_pList)
	{
		if(x == s)
		{
			x = nullptr;
			break;
		}
	}
	m_pSelector->Remove(s);
	delete s;
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
	
	int plr;
    for(plr = 1; plr < g_MaxGamerNumber; plr++)
	{
        if(plr > m_pList.size() || !m_pList[plr])
            break;	
	}
			
	Player *tmp = new Player(this, session_descriptor, plr);
	
	if(plr > g_MaxGamerNumber)
	{
		tmp->Send(g_AlreadyPlayingMsg);
		RemovePlayer(tmp);
	}
	else
	{
		m_pList.push_back(tmp);
		m_pSelector->Add(tmp);
	}
}

void Game::SendAll(const char* message, Player* except)
{
	for(auto x : m_pList)
		if(x != except)
			x->Send(message);
}


void Game::RequestProc(Player* plr, Request& req)
{
	int res{0};
	for(int i=0; i < 8; i++) 
	{
		if(req.GetText() == g_CommandList[i])
			res = i+1;
	}
/*
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
		default:
				plr->Send(g_UnknownReqMsg);
	}

*/
	return;
}

