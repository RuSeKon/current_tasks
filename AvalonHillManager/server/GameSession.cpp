#include <sys/socket.h>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <memory>
#include <iostream>
#include <cstring>
#include "share/server.h"
#include "share/errproc.h"
#include "share/application.h"



////////////////////////////SESSIONS/////////////////////////////////////////////////////

GameSession::GameSession(GameServer *a_master, int fd)
		: IFdHandler(fd), m_pTheMaster(a_master), m_BufUsed(0),  
		  m_Name(nullptr), m_Request(nullptr) {
	SendMsg(g_GreetingMsg);
}
GameSession::~GameSession()
{
	if(m_Name) 
	{
		delete[] m_Name;
	}
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
	
	if(!m_Name) 
	{
		std::cout << "Message came!\n";
           
		m_BufUsed = GetMessage();
		if(m_BufUsed > 0)
		{
			if(m_BufUsed > g_MaxName) SendMsg(g_NotNameMsg);
			m_Name = new char[m_BufUsed];
			strncpy(m_Name, m_Buffer, m_BufUsed-1);
			m_Name[m_BufUsed] = '\0';
			m_BufUsed = 0;

			std::unique_ptr<char> res(new char[strlen(g_WelcomeMsg)+g_MaxName+3]);
			sprintf(res.get(), g_WelcomeMsg, m_Name, m_PlayNumber);
			SendMsg(res.get());
		}
		return;
	} 
	else 
	{
		m_BufUsed = GetMessage();
		if(m_BufUsed > 0)
		{
			//Need to manage resources more clever!
			if(m_Request) delete[] m_Request;//clear last request

			m_Request = new char[m_BufUsed];
			strncpy(m_Request, m_Buffer, m_BufUsed-1);
			m_Request[BufUsed] = '\0';
			m_BufUsed = 0;
		}
	} 

}

int GameSession::GetMessage()
{
	int res = recv(GetFd(), m_Buffer, g_MaxName, 0);
            
	if(m_BufUsed == -1 || m_BufUsed == 0)
	{
		DeleteMe();
		return 0;
	}
	else if(m_BufUsed >= g_BufSize-1)
	{
		SendMsg(g_AnnoyingMsg); //need to send "Illegal request, try againg or type help!"
		DeleteMe();
		return 0;
	}
	return res;
}

void GameSession::SendMsg(const char *message)
{
	int res{0};
	res = send(GetFd(), message, strlen(message), 0);
	if(res == -1 || res == 0) {
		std::cout << "Client is offline\n";
		DeleteMe();
		return;
	}
}

void DeleteMe()
{
	m_pTheMaster->RemoveSession(this);
}
