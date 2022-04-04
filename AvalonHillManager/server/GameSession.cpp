#include <sys/socket.h>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <memory>
#include <iostream>
#include <cstring>
#include "server.h"
#include "errproc.h"
#include "application.h"



////////////////////////////SESSIONS/////////////////////////////////////////////////////

GameSession::GameSession(GameServer *a_master, int fd, int pl_nmbr)
		: IFdHandler(fd), m_pTheMaster(a_master), m_BufUsed(0),  
		  m_PlayNumber(pl_nmbr), m_Name(nullptr)
{
	SendMsg(g_GreetingMsg);
}

GameSession::~GameSession()
{
	if(m_Name) 
	{
		delete[] m_Name;
	}
	shutdown(GetFd(), SHUT_RDWR);
	close(GetFd());
}


void GameSession::VProcessing(bool r, bool w)
{
	if(!r)
		return;

	if(m_pTheMaster->GameBegun() == false) 
	{
		if(!m_Name) 
		{
			std::cout << "Message came!\n";
            
			m_BufUsed = recv(GetFd(), m_Buffer, g_MaxName, 0);
            
			if(m_BufUsed == -1 || m_BufUsed == 0)
			{
				m_pTheMaster->RemoveSession(this);
				return;
			}
			else if(m_BufUsed >= g_MaxName)
			{
				SendMsg(g_NotNameMsg);
				m_BufUsed = 0;
				return;
			}

			m_Name = new char[m_BufUsed];
			strncpy(m_Name, m_Buffer, m_BufUsed-1);
			m_Name[m_BufUsed] = '\0';
			m_BufUsed = 0;


			std::unique_ptr<char> res(new char[strlen(g_WelcomeAllMsg)+g_MaxName+3]);
			//sprintf(res.get(), g_WelcomeAllMsg, m_Name, m_PlayNumber);
			//m_pTheMaster->SendAll(res.get(), this);
			sprintf(res.get(), g_WelcomeMsg, m_Name, m_PlayNumber);
			SendMsg(res.get());
		} 
		else 
		{
			std::cout << "Message came!\n";
			m_BufUsed += recv(GetFd(), m_Buffer, g_BufSize, 0);
            
			if(m_BufUsed == -1 || m_BufUsed == 0)
			{
				m_pTheMaster->RemoveSession(this);
				return;
			}
			else if(m_BufUsed >= g_BufSize)
			{
				SendMsg(g_AnnoyingMsg);
				m_pTheMaster->RemoveSession(this);
				return;
			}

			SendMsg(g_GameNotBegunMsg);
			m_BufUsed = 0;
			return;
		}
	} 
	else 
	{
		m_BufUsed = recv(GetFd(), m_Buffer, g_BufSize, 0);///Needed attantion
        
		if(m_BufUsed >= g_BufSize-1)
		{
			std::cerr << "Error m_Buffer of client " << m_PlayNumber 
													 << " overflow\n";
			exit(EXIT_FAILURE);
		}
		else 
		//////NEED to SOLVE////
		{ m_Buffer[m_BufUsed] = '\0';}
        
		int i{0};
		for(; m_Buffer[i]; i++)
		{
			if(m_Buffer[i] == '\n') {
				Write(1, m_Buffer, i+1);
				m_BufUsed -= i+1;
			}
		}
		Memove(m_Buffer, m_Buffer+i+1, m_BufUsed);
	}

}

void GameSession::SendMsg(const char *message)
{
	int res{0};
	res = send(GetFd(), message, strlen(message), 0);
	if(res == -1 || res == 0) {
		std::cout << "Client is offline\n";
		m_pTheMaster->RemoveSession(this);
		return;
	}
}
