#ifndef BANKERHPPSENTRY
#define BANKERHPPSENTRY

#include <tuple>
#include <vector>
#include <string>
#include "game.h"

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


static const std::vector<std::string> g_CommandList{"market\0", "player\0", "prod\0",
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

class Banker;
class Manager : public Player 
{
	friend class Banker;

	char* m_Name;

	int m_Material;
	int m_Products;
	int m_Money;
	bool m_End;

	std::vector<int> m_Factories; //Need to solve

	std::tuple<int, int> m_BuyApply; 	
	std::tuple<int, int> m_SellApply;

	Manager(Game* b, FdHandler* s, int num);
	~Manager() noexcept;

	std::tuple<int, int> Parse(const char* req);
	void VAskToRequest() override;
	void VSend(const char* message) override;
};

class Banker : public Game
{
	int m_Circle;
	
	//get<0> quantity, get<1> cost
	std::tuple<int, int> m_Raw; 	
	std::tuple<int, int> m_Products;

public:
	Banker() : m_Circle(0) {};
	~Banker() noexcept;

	void VPlayerAdd(FdHandler* h) override;
	void VProcess(int fd) override;
	void VCircle() override;

	void SendAll(const char* message, int except);
	void MarketCondition(int number);
	void GetInfo(int number, int arg);
	void Enterprise(int number, int arg);
	void Build(int number);
	
};
#endif
