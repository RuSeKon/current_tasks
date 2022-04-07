/* 
----In this part of server defined work with client sessions----
*/
#ifndef SERVERHPPSENTRY
#define SERVERHPPSENTRY

#include "application.h"
#include "game.h"

#ifndef MAXGAMERNUMBER
#define MAXGAMERNUMBER 10
#endif

//////////GameSession strings///////////////////////

static const char g_IllegalMsg[] = {"Illegal request, buffer overflow...Goodbye!\n"};
static const char g_NotNameMsg[] = {"Your name is too long, KISS\n"};


/////////////////////////////////////////////////////////////////////////////

enum ConstantsForServer { 
	g_MaxGamerNumber = MAXGAMERNUMBER,
	g_MaxName = 10,
	g_BufSize = 256
};

class GameServer;

/* SERVERS IMPLEMENTATIONS OF USER SESSION */

class GameSession : public IFdHandler 
{
	friend class GameServer;
	friend class Player;

	GameServer *m_pTheMaster;

	char m_Buffer[g_BufSize];
	int m_BufUsed;
    
	char *m_Request;

	GameSession(GameServer *a_master, int fd);
	~GameSession() noexcept;

	void VProcessing(bool r, bool w) override;
	void Send(const char *message);
	int GetMessage();
	const char* GetBuffer() const {return m_Request;}
	void Delete();
	void Offliene();
};

class GameServer : public IFdHandler 
{
	EventSelector *m_pSelector;
	Game* m_pGame;
    
	/*Private constructor will use on method Start. For prevent unexpected GameServer construction, 
	because Sever should be one*/
	GameServer(EventSelector *sel, int fd); 
public:
	GameServer() = delete;
	~GameServer() noexcept;
	static GameServer *ServerStart(EventSelector *sel, int port);

	void RemoveSession(GameSession *s);
	void GameInteract(GameSession *s);
private:
	void VProcessing(bool r, bool w) override;
};

#endif
