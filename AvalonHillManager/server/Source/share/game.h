/* ----Game Logic implementation---- */
#ifndef SERVERHPPSENTRY
#define SERVERHPPSENTRY

#include <unordered_map>
#include <list>
#include <string>
#include <vector>
#include "application.h"

#ifndef MAXGAMERNUMBER
#define MAXGAMERNUMBER 3
#endif

enum ConstantsForGame { 
	g_MaxGamerNumber = MAXGAMERNUMBER,
	g_MaxName = 10,
	g_BufSize = 256,
	g_MaxParams = 2,
	g_CommandListSize = 9
};

enum ApplicationConstants { //for auction
	Full = 0, //Full amount in application accepted
	Raw = 1,
	Prod = 2
};

/* 					MARKET LEVEL CHANGE TABLE
*	Row - current level, Column - next level, value - probability of transition.
*  Original values n (0 < n < 1) were multiplied by 12.
*  In this contecst probability value 12 = 100% */
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

enum ConstantsForPlayerResources {
	resRaw = 0,
	resProd = 1,
	resFactory = 2,
	resMoney = 3
};

class Player : public IFdHandler 
{
	friend class Game;

	Game *m_pTheGame;
	char m_Buffer[g_BufSize];// how to hide it lowlevel implementation?
	int m_BufUsed;

	char* m_Name;
	int m_PlayerNumber;

	std::unordered_map<int, int> m_Resources;

    //list for memorizing the month when the factory was started built 
	std::list<int> m_ConstrFactories;

	//The number of products produced in the current month
	int m_Enterprise;

	int m_PlayerRaw[2];
	int m_PlayerProd[2];

	bool m_End;
public:
	Player(Game *a_master, int fd, int num);
	~Player() noexcept;

	void VProcessing(bool r, bool w) override;
	void Send(const char *message);
	Request ParseRequest();
	int ApplicationAccepted(int how, int flag);
};


//////////////////////////////////////////////////////////////////////

/*Struct for handling applications in Auction method*/
struct Application
{
	Player* plr;
	int m_ResrsAmount;
	int m_ResrsCost;
	Application(Player* src, int q, int c) : plr(src),
											 m_ResrsAmount(q),
											 m_ResrsCost(c) {}
};


class Game : public IFdHandler 
{
	EventSelector *m_pSelector;
	bool m_GameBegun;

	//Number of in-game months elapsed
	int m_Month;
	//Current market level
	int m_MarketLevel;

	std::vector<Player*> m_List;

	//array with free Player numbers
	int* m_Numbers;

	//Auction state
	int m_BankerRaw[2]; //[0] amount, [1] cost
	int m_BankerProd[2];//[0] amount, [1] cost

public:
	Game() = delete;
	~Game() noexcept;
	static Game *GameStart(EventSelector *sel, int port);

	bool GameBegun() const {return m_GameBegun;}
	void RemovePlayer(Player *s);
	void VProcessing(bool r, bool w) final;
	void RequestProc(Player* plr, const Request& req);
    void SendAll(const char* message, Player* except);
	

			/*PROCESSING PLAYER REQUESTS*/
    //Get information
	void GetInfo(Player* plr, const Request& arg, int res); 
    //Processing of applications for the production of products in factories 
	void Enterprise(Player* plr, const Request& arg);
	//Processing applications for participation in the auction of raw resources
	void BuyReq(Player* plr, const Request& arg);
	//Processing applications for participation in the auction of products
	void SellReq(Player* plr, const Request& arg);
	//Processing of applications for the construction of factories
    void BuildFactory(Player* plr);
	
				/* MAIN GAME ACTION */
	void NextMonth();
	void Auction(std::vector<Application>& src, int flag);
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
