/* Implementation of Gamer class */

#include <cctype>
#include <cstring>
#include "share/server.h"
#include "game_logic.h"

Gamer::Gamer(GameBanker* b, GameSession* s) : m_pBanker(b), m_pSession(s),
							m_GamerNumber(m_pSession->GetFd()), m_Factories(4),
							m_Material(4), m_Product(2), m_Money(10000), 
							m_End(false)
{}

Gamer::~Gamer()
{
	m_pSession->DeleteMe();	
}

void Gamer::AskToRequest()
{
	const char* tmp = m_pSession->GetRequest();
	if(!tmp)
		m_pSession->SendMsg("Bad request, try again!\n");

	while(isspace(*tmp++))
		;

	int res, p1, p2;
	res=p1=p2=0;

	for(int i=0; i < 8; i++) 
	{
		if(!strncmp(req, g_CommandList[i], strlen(g_CommandList[i])))
			res = i+1;
	}
	switch(res)
	{
		case Market:
			{
				m_pBanker->GetInfo(m_GamerNumber, 0); 
				break;
			}
		case AnotherPlayer:
			{
				std::tuple<int, int> t = Parse(tmp);
				/*
				if(std::get<0>(t) < 1 || std::get<0>(t) > g_MaxGamerNumber)
				{
					m_pSession->SendMsg("Bad request argument, try again!\n");
					break;
				}
				*/
				m_pBanker->GetInfo(m_GamerNumber, p1);
				break;
			}
		case Production:
			{
				std::tuple<int, int> t = Parse(tmp);
				m_pBanker->Enterprise(m_GamerNumber, std::get<0>(t));
				break;
			}
		case Buy:
			{
				std::tuple<int, int> t = Parse(tmp);
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
				else
				{
					m_BuyApply = t;
				}
			}
		case Sell:
			{
				std::tuple<int, int> t = Parse(tmp);
				/*
				if(std::get<0>(t) > m_Product)
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
				else
				{
					m_SellApply = t;
				}
			}
		case Build:
			{
				m_pBanker->Build(m_GamerNumber);	
			}
		case Turn:
			{
				m_end = true;
			}
		case Help:
			{
				m_pSession->SendMsg(g_HelpMsg);	
			}
		default:
			{
				m_pSession->SendMsg("Unknown request, try help!\n");
			}
	}
	return;
}

int Parse(const char* req)
{
	int a[2]{0};

	for(int i=0; *tmp; i++)
	{
		while(isspace(*tmp++) || !(isdigit(*tmp)))
			;
		for(; *tmp && isdigit(*tmp); tmp++)
			a[i] = a[i]*10 + (*tmp-48);
	}
	return std::make_tuple(a[0], a[1]) ;
}

