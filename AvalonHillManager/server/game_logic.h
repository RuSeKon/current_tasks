#ifndef GAMELOGICSENTRY
#define GAMELOGICSENTRY

#include <tuple>

static const char g_AlreadyPlayingMsg [] = {"Sorry, game is already started." 
						" You can play next one\n"};
static const char g_WelcomeMsg[] = {"Welcome to the game %s, " 
													"you play-number: %d\n"};
static const char g_WelcomeAllMsg[] = {"%s number %d joined to the game!\n"};
static const char g_GameStartSoon[] = {"The game will start soon!:)\n"};

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
	friend class GameBanker;
	GameBanker* m_pBanker;
	GameSession* m_pSession;

	int m_GamerNumber;

	int m_Factories;
	int m_Material;
	int m_Product;
	int m_Money;

	std::tuple<int, int> m_BuyApply; 	
	std::tuple<int, int> m_SellApply;

	bool m_End; 

	Gamer(GameBanker* b, GameSession* s);
	~Gamer();

	int Parse(const char* req);
	void AskToRequest();
};

class GameBanker
{
	std::vector<Gamer*> m_pList;

	int m_Cercle;
	
	//get<0> counter, get<1> cost
	std::tuple<int, int> m_Raw; 	
	std::tuple<int, int> m_Product;

	bool GameBugun;

public:

	GameBanker::GameBanker();

	void GamerAdd(GameSession* h);
	void Circle();
	void Process(int number);
	void GetInfo(int number, int arg);
	void Enterprise(int number, int arg);
	void Build(int number);
}


#endif
