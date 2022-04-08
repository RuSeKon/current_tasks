/* Implementation of Banker class */

#include <memory>
#include <cstring>
#include "game.h"

Banker::Banker() : m_Cycle(0)
{
	m_pList.reserve(g_MaxGamerNumber);
}

Banker::~Banker()
{
	for(auto x : m_pList)
		delete x;
}

void VProcessing(bool r, bool w)
{
	if(!r)
		return;
	GameServer::VProcessing(1,0);
	if(m_GamerCount+1 >= g_MaxGamerNumber)

void Banker::SendAll(const char* message, Manager* except)
{
	for(auto x : m_pList)
		if(x != except)
			x->Send(message);
}


void Banker::VPlayerAdd(GameSession* h)
{
	int plr;
	for(plr = 0; plr < g_MaxGamerNumber; plr++)
		if(!m_pList[plr])
			break;

	Manager* tmp = new Manager(this, h, plr);
	if(plr >= g_MaxGamerNumber)
	{
		tmp->Send(g_AlreadyPlayingMsg);
		delete tmp;
		return;
	}	
	
	m_pList[plr] = tmp;
	tmp->Send(g_GreetingMsg);
}

void Banker::VProcess(GameSession* h)
{	
	Request tmp;
	for(auto x : m_pList)
		if(x->m_pSession == h)
		{
			tmp = std::move(x->VGetRequest());
			RequestProc(x, tmp);
		}
}

void Banker::RequestProc(Manager* plr, Request& req)
{
	int res{0};
	for(int i=0; i < 8; i++) 
	{
		if(req.GetText() == g_CommandList[i])
			res = i+1;
	}

	switch(res)
	{
		case Market:
				MarketCondition(plr); 
				break;
		case AnotherPlayer:
				GetInfo(plr, req);
				break;
/*		case Production:
				Enterprise(plr, req);
				break;
		case Buy:
				
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
				} 
			
				//m_BuyApply = t;
				break;
		case Sell:
				
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
				} 
				//m_SellApply = t;
				break;
		case Build:
				Build(plr);
				break;
		case Turn:
				m_End = true;
				break;
		case Help:
				Send(g_HelpMsg);	
				break;
		default:
				plr->Send(g_UnknownReqMsg);
*/
	}
	return;
}

void Banker::MarketCondition(Manager* tmp)
{
	std::unique_ptr<char> res(new char[strlen(g_MarketConditionMsg)+12]);
	sprintf(res.get(), g_MarketConditionMsg, std::get<0>(m_Raw),
			std::get<1>(m_Raw), std::get<0>(m_Products), std::get<1>(m_Products));
	tmp->Send(res.get());
}

void Banker::GetInfo(Manager* tmp, Request& req)
{
	int num = req.GetParam(0);
	if(num < 0 || num > g_MaxGamerNumber)
	{
		tmp->Send(g_InvalidArgumentMsg);
		return;
	}	
	std::unique_ptr<char> res(new char[strlen(g_GetInfoMsg)+50]);
	sprintf(res.get(), g_GetInfoMsg, tmp->PlayerNumber(), tmp->m_Name.c_str(), 
			tmp->m_Money, tmp->m_Material, tmp->m_Products, 0, 0, 0); ///Need fabrique implementation
	tmp->Send(res.get());
}

/*
void Banker::Enterprise(Manager* plr, Request& req)
{

}
*/

