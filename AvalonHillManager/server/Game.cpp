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

Game *Game::ServerStart(EventSelector *sel, int port)
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
	m_pSelector->Remove(s);
}

int Game::VProcessing()
{
	int session_descriptor;
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);

	session_descriptor = accept(GetFd(), (struct sockaddr*) &addr, &len);
	if(session_descriptor == -1)
		return;
	
	Player *tmp = new Player(this, session_descriptor);

	int plr;
    for(plr = 0; plr < g_MaxGamerNumber; plr++)
        if(!m_pList[plr])
            break;	
	if(plr > g_MaxGamerNumber)
	{
		tmp->Send(g_AlreadyPlayingMsg);
		RemovePlayer(tmp);
	}
	else
	{
		m_pList.push_back(tmp);
		m_pSelecotr->Add(tmp);
	}
}

void Game::SendAll(const char* message, Player* except)
{
	for(auto x : m_pList)
		if(x != except)
			x->Send(message);
}


