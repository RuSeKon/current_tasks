#include <sys/socket.h>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include "errproc.h"
#include "application.h"
#include "game.h"



////////////////////////////PLAYER/////////////////////////////////////////////////////

Player::Player(Game *a_master, int fd, int num)
		: IFdHandler(fd), m_pTheMaster(a_master), m_BufUsed(0),
		m_Name(nullptr), m_PlayerNumber(num), m_Resources(0), m_End(false)
{
	m_Resources["Factory"] = 2;
	m_Resources["Raw"] = 4;
	m_Resources["Production"] = 2;
	m_Resources["Money"] = 10000;
	Send(g_GreetingMsg);
}

Player::~Player()
{
	if(m_Request)
	{
		delete[] m_Request;
	}
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
		Request req = GetRequest();
		if(!req.size()) //////////////////////
		{
			Send(g_BadRequestMsg);
			return;
		}
	
		if(!m_Name.size())
		{
			m_Name = req;
			std::unique_ptr<char> res(new char[strlen(g_WelcomeMsg)+g_MaxName+3]);
			sprintf(res.get(), g_WelcomeMsg, m_Name, PlayerNumber());
			Send(res.get());
			return;
		}
		else if(!m_pGame->GameBegun())
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
		Offline();
		return;
	}
}

Request& Player::GetRequest()
{
	int arr[3];
	std::string tmp;

	for(int i=0, b=0; i < m_BufUsed; i++) //Parse
	{
		if(isalpha((m_Buffer[i]))
			tmp.push_back(m_Buffer[i]);
		if(isdigit(m_Buffer[i]))
		{
			for(int c=i; isdigit(m_Buffer[c] && b < 3; c++))
				arr[b] += arr[b]*10 + (req[c]-48);
			b++;
			i += c-1;
		}
	}

	Request rq{tmp};
	for(int i=0; i < 3; i++)
		rq.AddParam(arr[i]);
	
	return rq;
}
