#ifndef GAMELOGICSENTRY
#define GAMELOGICSENTRY

#include <tuple>

static const char g_CommandsList[][] = {"market", "player", "prod",
					"buy", "sell", "build", "turn", "help"};


enum RequestConst {
	
	Market= 1;
	AnotherPlayer = 2;
	Production = 3;
	Buy = 4;
	Sell = 5;
	Build = 6;
	Turn = 7;
	Help = 8;
};

class Gamer
{
	GameSession* m_pSession;

	int m_Factories;
	int m_Material;
	int m_Product;
	int m_Money;

public:
	Gamer(GameSession* src) : m_pSession(src), (0) {}
	~Gamer();

	int Parse(const char* req);
	void AskToRequest();
};

class Banker
{
	std::vector<Gamer*> m_pList;

	int m_Cercle;
	
	//get<0> counter, get<1> cost
	std::tuple<int, int> m_Raw; 	std::tuple<int, int> m_Product;

	bool GameBugun;

public:

	Banker::Banker();
	Banker::~Banker();

	void GamerAdd(GameSession* h);


}


#endif
