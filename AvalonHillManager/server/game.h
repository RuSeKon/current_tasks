/* 
----Game Logic implementation----
*/
#ifndef SERVERHPPSENTRY
#define SERVERHPPSENTRY

#include <vector>
#include <string>
#include <unordered_map>
#include "application.h"

#ifndef MAXGAMERNUMBER
#define MAXGAMERNUMBER 10
#endif


/* Section for const messages */
static const char g_IllegalMsg[] = {"Illegal request, buffer overflow...Goodbye!\n"};
static const char g_NotNameMsg[] = {"Your name is too long, KISS\n"};
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
/////////////////////////////////////////////////////////////////////////////

using Apply = std::tuple<int, int>; // <0> quantity,  <1> cost

enum ConstantsForGame { 
	g_MaxGamerNumber = MAXGAMERNUMBER,
	g_MaxName = 10,
	g_BufSize = 256,
	g_MaxParams = 3
};

class Game;
class Request;

class Player : public IFdHandler 
{
	friend class Game;

	Game *m_pTheGame;
	char m_Buffer[g_BufSize];
	int m_BufUsed;

	std::string m_Name;
	int m_PlayerNumber;

	std::unordered_map<std::string, int> m_Resources;
    bool m_End;

	Player(Game *a_master, int fd, int num);
	~Player() noexcept;

	void VProcessing(bool r, bool w) override;
	void Send(const char *message);
	Request& ParseRequest();
};

class Game : public IFdHandler 
{
	EventSelector *m_pSelector;
	bool m_GameBegun;
	int m_Cycle;

	std::vector<Player*> m_pList;
	bool Begun;
    
public:
	Game() = delete;
	~Game() noexcept;
	static Game *GameStart(EventSelector *sel, int port);

	bool GameBegun() const {return Begun;}
	void RemovePlayer(Player *s);
	void VProcessing(bool r, bool w) override;
	void RequestProc(Player* plr, Request& req);
    void SendAll(const char* message, Player* except);
    void MarketCondition(Player* plr);
    void GetInfo(Player* plr, Request& arg);
    void Enterprise(Player* plr, Request& arg);
    void Build(Player* plr);
    void Cycle();
private:
	Game(EventSelector *sel, int fd); 
};

class Request
{
	std::string m_Text;
	std::vector<int> m_Params;
public:
	Request(std::string in) : m_Text(in) { m_Params.reserve(g_MaxParams);}
	~Request() noexcept;

	Request& operator=(Request&) = delete;
	Request& operator=(Request&& src);
	std::string& GetText() const {return m_Text;}
	void AddParam(int i) {m_Params.push_back(i);}
};
#endif