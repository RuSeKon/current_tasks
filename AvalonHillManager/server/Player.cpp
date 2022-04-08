#include <sys/socket.h>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <memory>
#include "errproc.h"
#include "application.h"
#include "game.h"



////////////////////////////PLAYER/////////////////////////////////////////////////////

Player::Player(Game *a_master, int fd, int num)
		: IFdHandler(fd), m_pTheGame(a_master), m_BufUsed(0),
		 m_PlayerNumber(num), m_End(false)
{
	m_Resources["Factory"] = 2;
	m_Resources["Raw"] = 4;
	m_Resources["Production"] = 2;
	m_Resources["Money"] = 10000;
	Send(g_GreetingMsg);
}

Player::~Player()
{
	shutdown(GetFd(), SHUT_RDWR);
	close(GetFd());
}


void Player::VProcessing(bool r, bool w)
{
	if(!r)
		return;

	m_BufUsed = recv(GetFd(), m_Buffer, g_BufSize, 0);
            
	if(m_BufUsed == -1 || m_BufUsed == 0)
	{
		m_pTheGame->RemovePlayer(this);
		return;
	}
	else if(m_BufUsed >= g_BufSize-1)
	{
		Send(g_IllegalMsg);
		m_pTheGame->RemovePlayer(this);
		return;
	}
	else	
	{
		Request req = ParseRequest();
		if(!req.GetText().size()) //////////////////////
		{
			Send(g_BadRequestMsg);
			return;
		}
	
		if(!m_Name.size())
		{
			if(req.GetText().size() > g_MaxName)
			{
				Send(g_NotNameMsg);
				return;
			}
			m_Name = req.GetText();
			std::unique_ptr<char> res(new char[strlen(g_WelcomeMsg)+g_MaxName+3]);
			sprintf(res.get(), g_WelcomeMsg, m_Name.c_str(), m_PlayerNumber);
			Send(res.get());
			return;
		}
		else if(!m_pTheGame->GameBegun())
		{
			Send(g_GameNotBegunMsg);
			return;
		}
		else
		{	
			m_pTheGame->RequestProc(this, req);

		}
	}
}

void Player::Send(const char *message)
{
	int res{0};
	res = send(GetFd(), message, strlen(message), 0);
	if(res == -1 || res == 0) {
		m_pTheGame->RemovePlayer(this);
		return;
	}
}

Request& Player::ParseRequest()
{
	int arr[3];
	std::string tmp;

	for(int i=0, b=0; i < m_BufUsed; i++) //Parse
	{
		if(isalpha(m_Buffer[i]))
			tmp.push_back(m_Buffer[i]);
		if(isdigit(m_Buffer[i]))
		{
			int c;
			for(c=i; isdigit(m_Buffer[c]) && b < 3; c++)
				arr[b] += arr[b]*10 + (m_Buffer[c]-48);
			b++;
			i += c-1;
		}
	}

	Request rq{tmp};
	for(int i=0; i < 3; i++)
		rq.AddParam(arr[i]);
	
	return rq;
}
