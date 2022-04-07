#include <sys/socket.h>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include "server.h"
#include "errproc.h"
#include "application.h"
#include "game.h"
 

/////////////////////////////////////////SERVER//////////////////////////////////////

GameServer::GameServer(EventSelector *sel, int fd)
						: IFdHandler(fd), m_pSelector(sel) 
{
	m_pSelector->Add(this, 0);
}

GameServer::~GameServer()
{
	m_pSelector->Remove(this);
}

GameServer *GameServer::ServerStart(EventSelector *sel, int port)
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
	
	return new GameServer(sel, ls);
}

void GameServer::RemoveSession(GameSession *s)
{
	m_pSelector->Remove(s);
}

void GameServer::VProcessing(bool r, bool w)
{
	if(!r)  //Explantation on README
		return;
	int session_descriptor;
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);

	session_descriptor = accept(GetFd(), (struct sockaddr*) &addr, &len);
	if(session_descriptor == -1)
		return;
	
	GameSession *tmp = new GameSession(this, session_descriptor);
	m_pSelector->Add(tmp, 1);
	m_pGame->VPlayerAdd(tmp);
}

void GameServer::GameInteract(GameSession* h)
{
	m_pGame->VProcess(h);
}
