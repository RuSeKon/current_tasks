/* Implementation of Gamer class */

#include <cctype>
#include <cstring>
#include "share/server.h"
#include "game_logic.h"

Gamer::~Gamer()
{
	m_pSession->DeleteMe();	
}

void Gamer::AskToRequest()
{
	const char* tmp = m_pSession->GetRequest();

	res = Parse(tmp);

	switch(res)
	{
		case Market:
			{}
		case AnotherPlayer:
			{}
		case Production:
			{}
		case Buy:
			{}
		case Shell:
			{}
		case Build:
			{}
		case Turn:
			{}
		case Help:
			{}
		default:
			{}
	}
}

int Parse(const char* req)
{
	while(isspace(req++))
		;

	for(int i=0; i < 8; i++) 
	{
		if(!strncmp(req, g_CommandList[i], strlen(g_CommandList[i])))
			return i+1;
	}
	return 0;
}

