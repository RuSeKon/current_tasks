/* Implementation of Gamer class */

#include <cctype>
#include <cstring>
#include <memory>
#include "server.h"
#include "banker.h"
#include "game.h"

Manager::Manager(Game* b, GameSession* s, int num) : Player(b, s, num), 
					m_Name(nullptr),m_Material(4), m_Products(2), 
					m_Money(10000), m_End(false)
{}

Request Manager::VGetRequest()
{
	
	std::string req = m_pSession->GetBuffer();
	if(!req.size())
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

	int arr[3];
	std::string tmp;

	for(int i=0, b=0; i < req.size(); i++) //Parse
	{
		if(isalpha(req[i]))
			tmp.push_back(req[i]);
		if(isdigit(req[i]))
		{
			for(int c=i; isdigit(req[c] && b < 3; c++))
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
