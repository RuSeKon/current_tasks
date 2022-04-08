#include <sys/socket.h>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include "server.h"
#include "errproc.h"
#include "application.h"



////////////////////////////SESSIONS/////////////////////////////////////////////////////

GameSession::GameSession(GameServer *a_master, int fd)
		: IFdHandler(fd), m_pTheMaster(a_master), m_BufUsed(0),
		m_Request(nullptr)  
{}

GameSession::~GameSession()
{
	if(m_Request)
	{
		delete[] m_Request;
	}
	shutdown(GetFd(), SHUT_RDWR);
	close(GetFd());
}


void GameSession::VProcessing(bool r, bool w)
{
	if(!r)
		return;
	m_BufUsed = recv(GetFd(), m_Buffer, g_BufSize, 0);
            
	if(m_BufUsed == -1 || m_BufUsed == 0)
	{
		Offline();
		return 0;
	}
	else if(m_BufUsed >= g_BufSize-1)
	{
		Send(g_IllegalMsg);
		Offline();
		return 0;
	}
	else	
	{
		//Need to manage resources more clever!
		if(m_Request) delete[] m_Request;//clear last request

		m_Request = new char[m_BufUsed];
		strncpy(m_Request, m_Buffer, m_BufUsed-1);
		m_Request[m_BufUsed] = '\0';
		m_BufUsed = 0;
	}
}

void GameSession::Send(const char *message)
{
	int res{0};
	res = send(GetFd(), message, strlen(message), 0);
	if(res == -1 || res == 0) {
		Offline();
		return;
	}
}

void GameSession::Delete()
{
	m_pTheMaster->RemoveSession(this);
	delete this;	
}

void GameSession::Offline()
{
	if(m_Request)
			delete[] m_Request;
	m_Request = new char[7];
	strcpy(m_Request, "offline");
}
