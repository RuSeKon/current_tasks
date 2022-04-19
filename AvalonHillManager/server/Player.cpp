#include <sys/socket.h>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <memory>
#include "errproc.h"
#include "application.h"
#include "game.h"


/* SECTION FOR CONSTANT MESSAGES */
static const char g_GreetingMsg[]={"Your welcome! Please enter your name: \n"};
static const char g_IllegalMsg[]={"Illegal request, buffer overflow...Goodbye!\n"};



static const char g_BoughtResMsg[]={"\nYour bought %d units of resources at a "
								"price of %d $.\n"};
static const char g_SellResMsg[]={"\nYour sell %d units of products at a price "
								"of %d $.\n"};

////////////////////////////PLAYER/////////////////////////////////////////////////////

Player::Player(Game *a_master, int fd, int num)
		: IFdHandler(fd), m_pTheGame(a_master), m_BufUsed(0),
		 m_Name(0), m_PlayerNumber(num), m_Enterprise(0), m_End(false)
{
	m_Resources[resFactory] = 2;
	m_Resources[resRaw] = 4;
	m_Resources[resProd] = 2;
	m_Resources[resMoney] = 10000;
	Send(g_GreetingMsg);
}

Player::~Player()
{
	if(m_Name) delete[] m_Name;
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
		m_pTheGame->RequestProc(this, req);
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

Request Player::ParseRequest()
{
	int arr[2]{0, 0};
	std::string tmp;

	for(int i=0, b=0; i < m_BufUsed; i++) //Parse
	{
		if(isalpha(m_Buffer[i]))
			tmp.push_back(m_Buffer[i]);
		if(isdigit(m_Buffer[i]))
		{
			int c;
			for(c=i; isdigit(m_Buffer[c]) && b < g_MaxParams; c++)
				arr[b] = arr[b]*10 + (static_cast<int>(m_Buffer[c])-48);
			b++;
			i = c-1;
		}
	}

	Request rq(tmp.c_str());
	for(int i=0; i < g_MaxParams; i++)
		rq.AddParam(arr[i]);
	
	return rq; //Rvalue ctor or NRVO optimization
}

int Player::ApplicationAccepted(int how, int flag)
{ 
	if(flag == Raw)
	{
		int amount{0};
		if(how == 0 || how > m_PlayerRaw[0])
			amount = m_PlayerRaw[0];
		else
			amount = how;

		m_Resources[resRaw] += amount;
		m_Resources[resMoney] -= amount * m_PlayerRaw[1];
		std::unique_ptr<char> msg(new char[strlen(g_BoughtResMsg)+8]);
		sprintf(msg.get(), g_BoughtResMsg, amount, m_PlayerRaw[1]);
		Send(msg.get());
		m_PlayerRaw[0] -= amount;
		return amount;
	}
	else if(flag == Prod)
	{
		int amount{0};
		if(how == 0 || how > m_PlayerProd[0])
			amount = m_PlayerProd[0];
		else
			amount = how;

		m_Resources[resProd] -= amount;
		m_Resources[resMoney] += amount * m_PlayerProd[1];
		std::unique_ptr<char> msg(new char[strlen(g_SellResMsg)+8]);
		sprintf(msg.get(), g_SellResMsg, amount, m_PlayerProd[1]);
		Send(msg.get());
		m_PlayerProd[0] -= amount;
		return amount;
	}
	else
		return 0;
}
