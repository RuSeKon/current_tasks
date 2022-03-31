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
//GameServer strings//
static const char g_AlreadyPlayingMsg[] = {"Sorry, game is already started." 
                        " You can play next one\n"};
static const char g_WelcomeMsg[] = {"Welcome to the game %s, you play-number: %d\n"};
static const char g_WelcomeAllMsg[] = {"%s number %d joined to the game!\n"};
static const char g_GameStartSoon[] = {"The game will start soon!:)\n"};

//GameSession strings//
static const char g_GameNotBegunMsg[] = {"The game haven't started yet. Please wait:)\n"};


//////////////////////////////////////////////////////////////////////////////////////
class GameSession;

enum ConstantsForServer { 
    g_MaxGamerNumber = MAXGAMERNUMBER,
    g_MaxName = 10,
    g_BufSize = 256,
};

struct item {
        GameSession* Session;
        
        item* next;

        item(int fd) : Session(nullptr), next(nullptr) {}
        item() = delete; //Because number must be initialized
};

//Keys for form Send massages
enum ServerKeys { 
    g_WelcomeKey = 0,
    g_PlayerJoinedKey = 1,
};

class GameServer : public IFdHandler {
    EventSelector *m_pSelector; //the_selector
    
    item *m_pItemHandler;

    int m_GamerCounter;
    bool m_GameBegun; //need getr function

    /*Private constructor will use on method Start. For prevent unexpected GameServer construction, 
    because Sever should be one*/
    GameServer(EventSelector *sel, int fd); 
public:
    GameServer() = delete;
    ~GameServer();
    static GameServer *ServerStart(EventSelector *sel, int port);

    void RemoveSession(GameSession *s);
    bool GameBegun() const {return m_GameBegun;}
    void SendAll(char *message, GameSession* except);
  ///  void GameLaunch(); ///////Needed parameters//////
private:
    void VProcessing(bool r, bool w);
};

/* SERVERS IMPLEMENTATIONS OF USER SESSION */

class GameSession : public IFdHandler {
    friend class GameServer;
    GameServer *m_pTheMaster;

    char m_Buffer[g_BufSize];
    int m_BufUsed;
    
    int m_PlayNumber;

    /*/Game resources
    int factories;
    int rawMaterial;
    int product;
    int money;
    */
    char m_Name[g_MaxName];

    GameSession(GameServer *a_master, int fd, int pl_nmbr);
    ~GameSession();

    void VProcessing(bool r, bool w);
    //void Send(int key);
    void Send(const char *message);
    int GetNumber() const { return m_PlayNumber; }
    const char* GetName() const { return m_Name; }
    char* FormStr(char*);
};

#endif
