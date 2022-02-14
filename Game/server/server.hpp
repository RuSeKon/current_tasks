
#ifndef SERVERHPPINCLUDE
#define SERVERHPPINCLUDE

#include <socket.hpp>

enum int_constants_for_server { 
    max_gamer_number = MAXGAMERNUMBER,
    max_name = 10,
    welcome_key = 0,
    player_joined_key = 1,

    //another key for send
};

//section for constant message initialization
static const char alrdy_playing_msg[] = {"Sorry, game is already started." 
                        " You can play next round\n"};
static const char game_n_beg_msg[] = {"The game haven't started yet. Please wait:)\n"};
static const char welcome[] = {"Welcome to the game %s, you play-number: %d\n"};
static const char welcome_all[] = {"%s number %d joined to the game!\n"};
//static const char player_joined[] = 


class GameSession;

class GameServer : public FdHandler {
    EventSelector *the_selector;
    struct item {
        GameSession *s;
        int number;
        item* next;
    }
    item *first;
    int gamer_count;
    
    bool game_begun;

    GameServer(EventSelector *sel, int fd);
public:
    ~GameServer();
    static GameServer *Start(EventSelector *sel, int port);

    RemoveSession(GameSession *s);
    void SendAll(int key, ChatSession* except);
    void SendAll(char *message);
    void GameStart(); ///////Needed parametrs//////
private:
    virtual void Process(bool r, bool w);
};

class GameSession : FdHandler {
    friend class GameServer;

    char *buffer; //for accumulate user sended data
    int buf_used;
    
    int play_nmbr;
    
    char *name;  /* gamer name */
    GameServer *the_master;

    GameSession(GameServer *a_master, int fd, int pl_nmbr);
    ~GameSession();

    void Process(bool r, bool w);
    void Send(int key);
    void Send(char *message);
    char *FormStr(char *);
    /*in this part will be function for processing session*/
}; 

#endif
