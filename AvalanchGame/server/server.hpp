#ifndef SERVERHPPSENTRY
#define SERVERHPPSENTRY

#include <socket.hpp>

#ifndef MAXGAMERNUMBER
#define MAXGAMERNUMBER 10
#endif



/* Section for constant message initialization!!! */

static const char alrdy_playing_msg[] = {"Sorry, game is already started." 
                        " You can play next round\n"};
static const char game_n_beg_msg[] = {"The game haven't started yet. Please wait:)\n"};
static const char welcome[] = {"Welcome to the game %s, you play-number: %d\n"};
static const char welcome_all[] = {"%s number %d joined to the game!\n"};


class GameSession;

enum ConstantsForServer { 
    max_gamer_number = MAXGAMERNUMBER,
    max_name = 10,
};
struct item {
        GameSession *session;
        const int number; //Gamer number should not be changed
        item* next;

        item(int fd) : session(nullptr), number(fd), next(nullptr) {}
        item() = delete; //Because number must be initialized
        ~item() noexcept {if(session) delete session;}
}
class GameServer : public FdHandler {
    EventSelector *the_selector;
    
    item *itemHandler;

    int gamer_counter;
    bool game_begun;

    /*Private constructor will use on method Start. For prevent unexpected GameServer construction, 
    because Sever should be one*/
    GameServer(EventSelector *sel, int fd); 
public:
    GameServer() = delete;
    ~GameServer();
    static GameServer *Start(EventSelector *sel, int port);

    RemoveSession(GameSession *s);
    void SendAll(int key, GameSession* except);
    void SendAll(char *message, GameSession* except);
  ///  void GameLaunch(); ///////Needed parameters//////
private:
    virtual void Process(bool r, bool w);
};


//Keys for Send methods
enum keys { 
    welcome_key = 0,
    player_joined_key = 1,
   //another key for send
}

/* SERVERS IMPLEMENTATIONS OF USER SESSION */
class GameSession : FdHandler {
    friend class GameServer;

    char *buffer;
    int buf_used;
    
    int play_nmbr;
    
    char *name;
    GameServer *the_master;

    GameSession(GameServer *a_master, int fd, int pl_nmbr);
    ~GameSession();

    void Process(bool r, bool w);
    void Send(int key);
    void Send(char *message);
    char *FormStr(char *);
};

/* Game is the implementation of high level protocol
for organization logic of the Game */

#endif
