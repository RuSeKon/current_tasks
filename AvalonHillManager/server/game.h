/* ----Game Logic implementation---- */
#ifndef SERVERHPPSENTRY
#define SERVERHPPSENTRY

#include <unordered_map>
#include <list>
#include <string>
#include <vector>
#include "application.h"

#ifndef MAXGAMERNUMBER
#define MAXGAMERNUMBER 1
#endif


/* SECTION FOR CONSTANTS OF GAME */
static const char g_IllegalMsg[]={"\nIllegal request, buffer overflow...Goodbye!\n"};
static const char g_NotNameMsg[]={"\nYour name is too long, KISS\n"};
static const char g_GreetingMsg[]={"\nYour welcome! Please enter your name: \n"};
static const char g_AlreadyPlayingMsg[]={"\nSorry, game is already started." 
						" You can play next one\n"};
static const char g_BoughtResMsg[]={"\nYour bought %d units of resources at a price of %d $.\n"};
static const char g_WelcomeMsg[]={"\nWelcome to the game %s, " 
													"you play-number: %d\n"};
static const char g_WelcomeAllMsg[]={"\nPlayer number %d joined to the game!\n"};
static const char g_GameNotBegunMsg[]={"\nThe game haven't started yet. " 
														   "Please wait:)\n"};
static const char g_GameStartSoonMsg[]={"\nThe game will start soon!:)\n"};
static const char g_InvalidArgumentMsg[]={"\nInvalid argument, please try again!"
											" Or type help:)\n"};
static const char g_BadRequestMsg[]={"\nBad request, please try again! Or type"
									   " help:)\n"};
static const char g_UnknownReqMsg[]={"\nUnknown request, please enter help!\n"};
static const char g_MarketCondMsg[]={"\n            In the current month:  "
										"            \nQuantity of materials sold:"
										" %d, by min price $%d /unit.\nQuantity "
										"of purchaced products: %d, by max price "
										"$%d /unit.\n"};
static const char g_GetInfoMsg[]={"\n%s's state of affairs (num: %d):\n"
								"Money: %d;\nMaterials: %d;\nProducts: %d;\n"
								"Regular factorie: %d;\nBuild factorie: %d;\n"};
static const char g_HelpMsg[]={"helpMe\n"};
static const char g_PlayerListMsg[]={"\n%d. %s\n"};
static const char g_BadRawQuantMsg[]={"\nThe amount of raw materials sold by the market is less.\n"};
static const char g_BadRawCostMsg[]={"\nYour cost is less than market.\n"};
static const char g_BadProdQuantMsg[]={"\nYou don't have that many products,or bank don't buy that quantity.\n"};
static const char g_BadProdCostMsg[]={"\nYour cost is larger than market.\n"};
static const char g_TooFewFactories[]={"\nYou don't have as many factories to produce.\n"};
static const char g_InsufficientFunds[]={"\nInsufficient funds to build so many factoryes.\n"};

/*
enum StringsSize
{
	g_IllegalMsgSize = 46,
	g_NotNameMsgSize = 30,
	g_GreetingMsgSize = 41,
	g_AlreadyPlayingMsgSize = 56,
	//g_WelcomeMsgSize = 58,
	//g_WelcomeAllMsgSize = 42,
	g_GameNotBegunMsgSize = 46,
	g_GameStartSoonMsgSize = 30,
	g_InvalidArgumentMsgSize = 53,
	g_BadRequestMsgSize = 48,
	g_UnknownReqMsgSize = 38,
	//g_MarketCondMsgSize = 179,
	//g_GetInfoMsgSize = 149, 
	g_HelpMsgSize = 8,
	//g_PlayerListMsgSize = 18
};
*/
enum ConstantsForGame { 
	g_MaxGamerNumber = MAXGAMERNUMBER,
	g_MaxName = 10,
	g_BufSize = 256,
	g_MaxParams = 2,
	g_CommandListSize = 9
};

enum RequestConstants {
	
	Market= 1,
	AnotherPlayer = 2,
	Production = 3,
	Buy = 4,
	Sell = 5,
	Build = 6,
	Turn = 7,
	Help = 8,
	PlayerAll = 9,
};



/* Row - current level, Column - next level, value - probability of transition.
*  Original value (lower 1(100%)) were multiplied by 12*/
static const int g_MarketLevels[5][5] = {
	{4, 4, 2, 1, 1}, 
	{3, 4, 4, 1, 1},
	{1, 3, 4, 3, 1},
	{1, 1, 3, 4, 3},
	{1, 1, 2, 4, 4}
};


/////////////////////////////////////////////////////////////////////////////
class Game;
class Request;

enum ConstForResources {
	Raw = 0,
	Prod = 1,
	Factory = 2,
	Money = 3
};
class Player : public IFdHandler 
{
	friend class Game;

	Game *m_pTheGame;
	char m_Buffer[g_BufSize];
	int m_BufUsed;

	char* m_Name;
	int m_PlayerNumber;

	std::unordered_map<int, int> m_Resources;
    bool m_End;
	int m_Enterprise;
	std::list<int> m_ConstrFactories;
	
	int m_PlayerRaw[2];
	int m_PlayerProd[2];

	Player(Game *a_master, int fd, int num);
	~Player() noexcept;

	void VProcessing(bool r, bool w) override;
	void Send(const char *message);
	Request ParseRequest();
};


//////////////////////////////////////////////////////////////////////

class Game : public IFdHandler 
{
	EventSelector *m_pSelector;
	bool m_GameBegun;
	//Number of in-game months elapsed
	int m_Month;  
	int m_MarketLevel;

	std::vector<Player*> m_pList; //Think about unordered_map

	//Auction state
	int m_BankerRaw[2]; //[0] quantity, [1] cost
	int m_BankerProd[2];//[0] quantity, [1] cost

public:
	Game() = delete;
	~Game() noexcept;
	static Game *GameStart(EventSelector *sel, int port);

	bool GameBegun() const {return m_GameBegun;}
	void RemovePlayer(Player *s);
	void VProcessing(bool r, bool w) final;
	void RequestProc(Player* plr, Request& req);
    void SendAll(const char* message, Player* except);
	

			/*PROCESSING PLAYER REQUESTS*/
    //Get information
	void GetInfo(Player* plr, Request& arg, int res); 
    //Processing of applications for the production of products in factories 
	void Enterprise(Player* plr, Request& arg);
	//Processing applications for participation in the auction 
	void BuyReq(Player* plr, Request& arg);

	void SellReq(Player* plr, Request& arg);
	//Processing of applications for the construction of factories
    void BuildFactory(Player* plr);
	//To change current quantity/cost resources sold by the bank
	
				/* MAIN GAME ACTION */
	void Cycle();
	void Auction();
	void ChangeMarketLvl();
	void SetMarketLvl(int num);
private:
	Game(EventSelector *sel, int fd); //Use GameStart!
};

/*Type to work with player requests*/
class Request
{
	char* m_pText;
	int* m_pParams;
	size_t m_Set;
public:
	Request() = delete;
	Request(const char* in);
	~Request() noexcept;

	Request(const Request&);
	Request(Request&&);
	Request& operator=(const Request);
	Request& operator=(Request&& src);
	void swap(Request& src);

	const char* GetText() const;
	int GetParam(int num) const;
	void AddParam(int i);
};
#endif
