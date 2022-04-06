#include <sys/socket.h>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include "share/server.h"
#include "share/errproc.h"
#include "share/application.h"



////////////////////////////SESSIONS/////////////////////////////////////////////////////

GameSession::GameSession(GameServer *a_master, int fd)
		: IFdHandler(fd), m_pTheMaster(a_master), m_BufUsed(0),  
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
	m_BufUsed = GetMessage();
	if(m_BufUsed > 0)
	{
		//Need to manage resources more clever!
		if(m_Request) delete[] m_Request;//clear last request

		m_Request = new char[m_BufUsed];
		strncpy(m_Request, m_Buffer, m_BufUsed-1);
		m_Request[m_BufUsed] = '\0';
		m_BufUsed = 0;
	}
}

int GameSession::GetMessage()
{
	int res = recv(GetFd(), m_Buffer, g_MaxName, 0);
            
	if(m_BufUsed == -1 || m_BufUsed == 0)
	{
		Delete();
		return 0;
	}
	else if(m_BufUsed >= g_BufSize-1)
	{
		VSendMsg(g_AnnoyingMsg); //need to send "Illegal request, try againg or type help!"
		Delete();
		return 0;
	}
	return res;
}

void GameSession::VSendMsg(const char *message)
{
	int res{0};
	res = send(GetFd(), message, strlen(message), 0);
	if(res == -1 || res == 0) {
		if(m_Request)
			delete[] m_Request;
		m_Request = new char[7];
		strcpy(m_Request, "offline");
		return;
	}
}

void GameSession::Delete()
{
	delete this;	
}
