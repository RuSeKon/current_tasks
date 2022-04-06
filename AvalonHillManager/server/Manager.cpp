/* Implementation of Gamer class */

#include <cctype>
#include <cstring>
#include <memory>
#include "share/server.h"
#include "share/banker.h"

Manager::Manager(Game* b, FdHandler* s, int num) : Player(b, s, num), 
					m_Name(nullptr),m_Material(4), m_Products(2), 
					m_Money(10000), m_End(false)
{}

Manager::~Manager()
{
	if(m_Name)
		delete[] m_Name;
}

void Manager::VSend(const char* message)
{
	m_pApplication->SendMsg(message);
}

void Manager::VAskToRequest()
{
	if(!m_pBanker->GameBegun())
	{
		VSend(g_GameNotBegunMsg);
		return;
	}
	const char* req = m_pApplication->GetBuffer(); //NeedAttantion
	if(!req)
	{
		m_pApplication->SendMsg(g_BadRequestMsg);
		return;
	}
	else if(!m_Name)
	{
		m_Name = new char[strlen(req)];
		strncpy(m_Name, req, strlen(req));
		std::unique_ptr<char> res(new char[strlen(g_WelcomeMsg)+g_MaxName+3]);
		sprintf(res.get(), g_WelcomeMsg, m_Name, PlayerNumber());
		VSend(res.get());
		return;
	}

	while(isspace(*req++))
		;

	int res, p1, p2;
	res=p1=p2=0;

	for(int i=0; i < 8; i++) 
	{
		if(!strncmp(req, g_CommandList[i].c_str(), g_CommandList[i].size()))
			res = i+1;
	}
	std::tuple<int, int> t = Parse(req);

	switch(res)
	{
		case Market:
				m_pBanker->MarketCondition(PlayerNumber()); 
				break;
		case AnotherPlayer:
				m_pBanker->GetInfo(PlayerNumber(), std::get<0>(t));
				break;
		case Production:
				m_pBanker->Enterprise(PlayerNumber(), std::get<0>(t));
				break;
		case Buy:
				/*
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
				} */
			
				m_BuyApply = t;
				break;
		case Sell:
				/*
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
				} */
				m_SellApply = t;
				break;
		case Build:
				m_pBanker->Build(PlayerNumber());
				break;
		case Turn:
				m_End = true;
				break;
		case Help:
				m_pApplication->VSendMsg(g_HelpMsg);	
				break;
		default:
				m_pApplication->VSendMsg(g_UnknownReqMsg);
	}
	return;
}

std::tuple<int, int> Parse(const char* req)
{
	int a[2]{0};

	for(int i=0; *req; i++)
	{
		while(isspace(*req++) || !(isdigit(*req)))
			;
		for(; *req && isdigit(*req); req++)
			a[i] = a[i]*10 + (*req-48);
	}
	return std::make_tuple(a[0], a[1]) ;
}
