/* 
----Game Logic implementation----
*/
#ifndef SERVERHPPSENTRY
#define SERVERHPPSENTRY

#include "application.h"

#ifndef MAXGAMERNUMBER
#define MAXGAMERNUMBER 10
#endif

//////////Player strings///////////////////////

static const char g_IllegalMsg[] = {"Illegal request, buffer overflow...Goodbye!\n"};
static const char g_NotNameMsg[] = {"Your name is too long, KISS\n"};
static const char g_AlreadyPlayingMsg [] = {"Sorry, game is already started."
						                        " You can play next one\n"};

/////////////////////////////////////////////////////////////////////////////

using Apply = std::tuple<int, int>; // <0> quantity,  <1> cost

enum ConstantsForServer { 
	g_MaxGamerNumber = MAXGAMERNUMBER,
	g_MaxName = 10,
	g_BufSize = 256
};

class Game;

/* SERVERS IMPLEMENTATIONS OF USER SESSION */

class Player : public IFdHandler 
{
	friend class Game;

	Game *m_pTheMaster;
	char m_Buffer[g_BufSize];
	int m_BufUsed;

	std::string m_Name;
	int m_PlayerNumber;

    int m_Material;
    int m_Products;
    int m_Money;
    bool m_End;

    //std::vector<int> m_Factories; //Need to solve

    //std::tuple<int, int> m_BuyApply;
    //std::tuple<int, int> m_SellApply;
    
	Player(Game *a_master, int fd);
	~Player() noexcept;

	void VProcessing(bool r, bool w) override;
	void Send(const char *message);
	const char* GetBuffer() const {return m_Request;}
	void Offline();
};

class Game : public IFdHandler 
{
	EventSelector *m_pSelector;
	bool m_GameBegun;
	int m_Cycle;

	std::vector<Player*> m_pList;
    
public:
	Game() = delete;
	~Game() noexcept;
	static Game *ServerStart(EventSelector *sel, int port);

	void RemoveSession(Player *s);
	void VProcessing(bool r, bool w) override;
	void RequestProc(Manager* plr, Request& req);
    void SendAll(const char* message, Manager* except);
    void MarketCondition(Manager* plr);
    void GetInfo(Manager* plr, Request& arg);
    void Enterprise(Manager* plr, Request& arg);
    void Build(Manager* plr);
    void Cycle();
private:
	Game(EventSelector *sel, int fd); 
};

#endif
