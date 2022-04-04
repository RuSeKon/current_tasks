/* 
----In this part of server defined work with client sessions----
*/
#ifndef SERVERHPPSENTRY
#define SERVERHPPSENTRY

#include "application.h" 
#ifndef MAXGAMERNUMBER
#define MAXGAMERNUMBER 10
#endif



/* Section for constant message initialization!!! */
//////////GameServer strings////////////////////////
static const char g_AlreadyPlayingMsg [] = {"Sorry, game is already started." 
						" You can play next one\n"};
static const char g_WelcomeMsg[] = {"Welcome to the game %s, " 
													"you play-number: %d\n"};
static const char g_WelcomeAllMsg[] = {"%s number %d joined to the game!\n"};
static const char g_GameStartSoon[] = {"The game will start soon!:)\n"};

//////////GameSession strings///////////////////////
static const char g_GameNotBegunMsg[] = {"The game haven't started yet. " 
														   "Please wait:)\n"};

static const char g_GreetingMsg[] = {"Your welcome! Enter you name:\n"};
static const char g_AnnoyingMsg[] = {"You are very annoying... Goodbye!\n"};
static const char g_NotNameMsg[] = {"Your name is too long, KISS\n"};


/////////////////////////////////////////////////////////////////////////////

enum ConstantsForServer { 
	g_MaxGamerNumber = MAXGAMERNUMBER,
	g_MaxName = 10,
	g_BufSize = 256
};

class GameServer;

/* SERVERS IMPLEMENTATIONS OF USER SESSION */

class GameSession : public IFdHandler {
	friend class GameServer;
	friend class Gamer;

	GameServer *m_pTheMaster;

	char m_Buffer[g_BufSize];
	int m_BufUsed;
    
	char *m_Name;
	char *m_Request;

	GameSession(GameServer *a_master, int fd);
	~GameSession();

	void VProcessing(bool r, bool w) override;
	void SendMsg(const char *message);
	int GetMessage();
public:
	const char* GetRequest() const {return m_Request;}
	void DeleteMe();
};

class GameServer : public IFdHandler {
	EventSelector *m_pSelector;
    
	/*Private constructor will use on method Start. For prevent unexpected GameServer construction, 
	because Sever should be one*/
	GameServer(EventSelector *sel, int fd); 
public:
	GameServer() = delete;
	~GameServer();
	static GameServer *ServerStart(EventSelector *sel, int port);

	void RemoveSession(GameSession *s);
private:
	void VProcessing(bool r, bool w) override;
};

#endif
