#ifndef GAMELOGICSENTRY
#define GAMELOGICSENTRY

#include <tuple>
#include <vector>
#include <string>

/* Section for const messages */
static const char g_GreetingMsg[] = {"Your welcome! Enter you name:\n"};
static const char g_AlreadyPlayingMsg [] = {"Sorry, game is already started." 
						" You can play next one\n"};
static const char g_WelcomeMsg[] = {"Welcome to the game %s, " 
													"you play-number: %d\n"};
static const char g_WelcomeAllMsg[] = {"%s number %d joined to the game!\n"};
static const char g_GameNotBegunMsg[] = {"The game haven't started yet. " 
														   "Please wait:)\n"};
static const char g_GameStartSoonMsg[] = {"The game will start soon!:)\n"};
static const char g_InvalidArgumentMsg[] = {"Invalid argument, please try again!"
											" Or type help:)\n"};
static const char g_BadRequestMsg[] = {"Bad request, please try again! Or type"
									   " help:)\n"};
static const char g_UnknownReqMsg[] = {"Unknown request, please try help!\n"};
static const char g_MarketConditionMsg[] = {"            In the current month:  "
										"            \nQuantity of materials sold:"
										" %d by min price %d/unit.\nQuantity "
										"of purchaced products: %d by max price "
										"%d/unit.\n"};
static const char g_GetInfoMsg[] = {"The state of affairs(%d.%s):\n"
								"Money: %d;\nMaterials: %d;\nProducts: %d;\n"
								"Build factorie: %d;\nRegular factorie: %d;\n"
								"Automatic factorie: %d.\n"};


static const std::vector<char*> g_CommandList{"market\0", "player\0", "prod\0",
					"buy\0", "sell\0", "build\0", "turn\0", "help\0"};

enum RequestConst {
	
	Market= 1,
	AnotherPlayer = 2,
	Production = 3,
	Buy = 4,
	Sell = 5,
	Build = 6,
	Turn = 7,
	Help = 8,
};

class GameBanker;
class Gamer
{
	friend class GameBanker;
	GameBanker* m_pBanker;
	GameSession* m_pSession;

	int m_GamerNumber;
	char* m_Name;

	int m_Material;
	int m_Products;
	int m_Money;
	bool m_End;

	std::vector<int> m_Factories; //Need to solve

	std::tuple<int, int> m_BuyApply; 	
	std::tuple<int, int> m_SellApply;

	Gamer(GameBanker* b, GameSession* s, int num);
	~Gamer() noexcept;

	std::tuple<int, int> Parse(const char* req);
	void AskToRequest();
	void Send(const char* message);
};

class GameBanker
{

	std::vector<Gamer*> m_pList;

	int m_Cercle;
	
	//get<0> quantity, get<1> cost
	std::tuple<int, int> m_Raw; 	
	std::tuple<int, int> m_Products;

	bool m_GameBegun;

public:

	GameBanker();
	~GameBanker() noexcept;

	bool GameBegun() const {return m_GameBegun;}
	void SendAll(const char* message, int except);
	void GamerAdd(GameSession* h);
	void Process(int fd);
	void MarketCondition(int number);
	void GetInfo(int number, int arg);
	void Enterprise(int number, int arg);
	void Build(int number);
	
	void Circle();
};
#endif
