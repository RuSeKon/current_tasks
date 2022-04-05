/* Implementation of Gamer class */

#include <cctype>
#include <cstring>
#include <memory>
#include "share/server.h"
#include "game_logic.h"

Gamer::Gamer(GameBanker* b, GameSession* s, int num) : m_pBanker(b), 
					m_pSession(s), m_GamerNumber(num), m_Name(nullptr),
					m_Material(4), m_Products(2), m_Money(10000), m_End(false)
{}

Gamer::~Gamer()
{
	if(m_Name)
		delete[] m_Name;
	m_pSession->DeleteMe();	
}

void Gamer::Send(const char* message)
{
	m_pSession->SendMsg(message);
}

void Gamer::AskToRequest()
{
	if(!m_pBanker->GameBegun())
	{
		Send(g_GameNotBegunMsg);
		return;
	}
	const char* req = m_pSession->GetRequest();
	if(!req)
	{
		m_pSession->SendMsg(g_BadRequestMsg);
		return;
	}
	else if(!m_Name)
	{
		m_Name = new char[strlen(req)];
		strncpy(m_Name, req, strlen(req));
		std::unique_ptr<char> res(new char[strlen(g_WelcomeMsg)+g_MaxName+3]);
		sprintf(res.get(), g_WelcomeMsg, m_Name, m_GamerNumber);
		Send(res.get());
		return;
	}

	while(isspace(*req++))
		;

	int res, p1, p2;
	res=p1=p2=0;

	for(int i=0; i < 8; i++) 
	{
		if(!strncmp(req, g_CommandList[i], strlen(g_CommandList[i])))
			res = i+1;
	}
	std::tuple<int, int> t = Parse(req);

	switch(res)
	{
		case Market:
				m_pBanker->MarketCondition(m_GamerNumber); 
				break;
		case AnotherPlayer:
				m_pBanker->GetInfo(m_GamerNumber, std::get<0>(t));
				break;
		case Production:
				m_pBanker->Enterprise(m_GamerNumber, std::get<0>(t));
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
				m_pBanker->Build(m_GamerNumber);
				break;
		case Turn:
				m_End = true;
				break;
		case Help:
				m_pSession->SendMsg(g_HelpMsg);	
				break;
		default:
				m_pSession->SendMsg(g_UnknownReqMsg);
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