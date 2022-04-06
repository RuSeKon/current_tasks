/* Implementation of Banker class */

#include <memory>
#include "share/banker.h"

Banker::Banker()
{
	m_pList.reserve(g_MaxGamerNumber);
}

void Banker::SendAll(const char* message, int except)
{
	for(auto x : m_pList)
		if(x->m_PlayerNumber != except)
			x->Send(message);
}


void Banker::VPlayerAdd(FdHandler* h)
{
	int num;
	for(num = 0; num < g_MaxGamerNumber; num++)
		if(!m_pList[num])
			break;

	Manager* tmp = new Manager(this, h, num);
	if(num >= g_MaxGamerNumber)
	{
		tmp->Send(g_AlreadyPlayingMsg);
		delete tmp;
		return;
	}	
	
	m_pList[num] = tmp;
	tmp->Send(g_GreetingMsg);
}

void Banker::VProcess(int fd)
{
	for(auto x : m_pList)
		if(x->m_pApplication->GetFd() == fd)
			x->VAskToRequest();
}

void Banker::MarketCondition(int number)
{
	Manager* tmp = m_pList[number];
	std::unique_ptr<char> res(new char[strlen(g_MarketConditionMsg)+12]);
	sprintf(res.get(), g_MarketConditionMsg, std::get<0>(m_Raw),
			std::get<1>(m_Raw), std::get<0>(m_Products), std::get<1>(m_Products));
	tmp->VSend(res.get());
}

void Banker::GetInfo(int number, int arg)
{
	Manager* tmp = m_pList[number];
	if(arg < 0 || arg > g_MaxGamerNumber)
	{
		tmp->VSend(g_InvalidArgumentMsg);
		return;
	}	
	std::unique_ptr<char> res(new char[strlen(g_GetInfoMsg)+50]);
	sprintf(res.get(), g_GetInfoMsg, tmp->m_PlayerNumber, tmp->m_Name, 
			tmp->m_Money, tmp->m_Material, tmp->m_Products, 0, 0, 0); ///Need fabrique implementation
	tmp->VSend(res.get());
}

void Banker::Enterprise(int number, int arg)
{

}

