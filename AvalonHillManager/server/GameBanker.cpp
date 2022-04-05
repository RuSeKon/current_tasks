/* Implementation of Banker class */

#include <memory>
#include "share/server.h"
#include "game_logic.h"

GameBanker::GameBanker()
{
	m_pList.reserve(g_MaxGamerNumber);


}

GameBanker::~GameBanker()
{
	for(auto x : m_pList)
		delete x;
}

void GameBanker::SendAll(const char* message, int except)
{
	for(auto x : m_pList)
		if(x->m_GamerNumber != except)
			x->Send(message);
}


void GameBanker::GamerAdd(GameSession* h)
{
	int num;
	for(num = 0; num < g_MaxGamerNumber; num++)
		if(!m_pList[num])
			break;

	Gamer* tmp = new Gamer(this, h, num);
	if(num >= g_MaxGamerNumber)
	{
		tmp->Send(g_AlreadyPlayingMsg);
		delete tmp;
		return;
	}	
	
	m_pList[num] = tmp;
	tmp->Send(g_GreetingMsg);
}

void GameBanker::Process(int fd)
{
	for(auto x : m_pList)
		if(x->m_pSession->GetFd() == fd)
			x->AskToRequest();
}

void GameBanker::MarketCondition(int number)
{
	Gamer* tmp = m_pList[number];
	std::unique_ptr<char> res(new char[strlen(g_MarketConditionMsg)+12]);
	sprintf(res.get(), g_MarketConditionMsg, std::get<0>(m_Raw),
			std::get<1>(m_Raw), std::get<0>(m_Products), std::get<1>(m_Products));
	tmp->Send(res.get());
}

void GameBanker::GetInfo(int number, int arg)
{
	Gamer* tmp = m_pList[number];
	if(arg < 0 || arg > g_MaxGamerNumber)
	{
		tmp->Send(g_InvalidArgumentMsg);
		return;
	}	
	std::unique_ptr<char> res(new char[strlen(g_GetInfoMsg)+50]);
	sprintf(res.get(), g_GetInfoMsg, tmp->m_GamerNumber, tmp->m_Name, 
			tmp->m_Money, tmp->m_Material, tmp->m_Products, 0, 0, 0); ///Need fabrique implementation
	tmp->Send(res.get());
}

void Enterprise(int number, int arg)
{

}

