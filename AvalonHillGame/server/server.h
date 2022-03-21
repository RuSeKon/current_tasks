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
static const char already_playing_msg[] = {"Sorry, game is already started." 
                        " You can play next one\n"};
static const char welcome[] = {"Welcome to the game %s, you play-number: %d\n"};
static const char welcome_all[] = {"%s number %d joined to the game!\n"};

//GameSession strings//
static const char game_n_beg_msg[] = {"The game haven't started yet. Please wait:)\n"};


//////////////////////////////////////////////////////////////////////////////////////
class GameSession;

enum ConstantsForServer { 
    max_gamer_number = MAXGAMERNUMBER,
    max_name = 10,
};

struct item {
        GameSession *session;
        
        item* next;

        item(int fd) : session(nullptr), next(nullptr), number(fd) {}
        item() = delete; //Because number must be initialized
        ~item() noexcept {if(session) delete session;}
        int GetNumber() const {return number;}
private:
        const int number;
};

class GameServer : public FdHandler {
    EventSelector *the_selector;
    
    item *itemHandler;

    int gamer_counter;
    bool game_begun;
    static bool serverRun;

    /*Private constructor will use on method Start. For prevent unexpected GameServer construction, 
    because Sever should be one*/
    GameServer(EventSelector *sel, int fd); 
public:
    GameServer() = delete;
    ~GameServer();
    static GameServer *ServerStart(EventSelector *sel, int port);

    void RemoveSession(GameSession *s);
    void SendAll(int key, GameSession* except);
    void SendAll(char *message, GameSession* except);
  ///  void GameLaunch(); ///////Needed parameters//////
private:
    virtual void Process(bool r, bool w);
};


//Keys for form Send massages
enum serverKeys { 
    welcome_key = 0,
    player_joined_key = 1,
};

/* SERVERS IMPLEMENTATIONS OF USER SESSION */

class GameSession : FdHandler {
    friend class GameServer;
    GameServer *the_master;

    char *buffer;
    int buf_used;
    
    int play_nmbr;

    //Game resources
    int factories;
    int rawMaterial;
    int product;
    int money;
    
    char *name;

    GameSession(GameServer *a_master, int fd, int pl_nmbr);
    ~GameSession();

    void Process(bool r, bool w);
    void Send(int key);
    void Send(char *message);
    char* FormStr(char*);
};

#endif
