/* 
----In this part of server defined work with client sessions----

    In this implementation I use a key parametr for method "Send". In this case
it provide me opportunity to form output string consist from string, int values,
and other variables (after cast to *str) implicity on body of Send.
*/

#include <sys/socket.h>
#include "socket.hpp"

#ifndef MAXGAMERNUMBER
#define MAXGAMERNUMBER 10
#endif 


class GameSession;

enum int_constants_for_server { 
    max_gamer_number = MAXGAMERNUMBER,
    welcome_key = ,
    player_joined_key =
    //another key for send
};


//section for constant message initialization
static const char alrdy_playing_msg[] = {"Sorry, game is already started." 
                        " You can play next round\n"};
static const char game_n_beg_msg[] = {"The game haven't started yet. Please wait\n"};
static const char welcome[] = {"Welcome to the game, you play-number: "};
//static const char player_joined[] = 
 

class GameServer : public FdHandler {
    EventSelector *the_selector;
    std::list<GameSession*, int num> item;  //maybe native C list?
    struct item {
        GameSession *s;
        int number;
        item* next;
    }
    item *first;
    bool game_begun;

    GameServer(EventSelector *sel, int fd);
public:
    ~GameServer();
    static GameServer *Start(EventSelector *sel, int port);

    RemoveSession(GameSession *s);
    void SendAll(int key, ChatSession* except);
    void SendAll(char *message);
    void FormMessage(int templ, 
private:
    virtual void Process(bool r, bool w);
};


GameServer::GameServer(EventSelector *sel, int fd)
        : FdHandler(fd), the_selector(sel), game_begun(false),
        first(0),
{
    the_selector->Add(this);
}

GameServer::~GameServer()
{
    while(first) {
        item *tmp = first;
        first = first->next;
        the_selector->Remove(tmp->s);
        delete tmp->s;
        delete tmp;
    }
    the_selector->Remove(this);
}

GameServer *GameServer::Start(EventSelector *sel, int port)
{
    int ls, opt, res;
    struct sockaddr_in addr;

    ls = socket(AF_INET, SOCK_STREAM, 0);
    if(ls == -1)
        return 0;

    opt = 1;
    setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    res = bind(ls, (struct sockaddr*) &addr, sizeof(addr));
    if(res == -1)
        return 0;

    res = listen(ls, 16);
    if(res == -1)
        return 0;
    return new GameServer(sel, ls);
}

void GameServer::RemoveSession(GameSession *s)
{
    the_selector->Remove(s);
    item **p;
    for(p = &first; *p; p = &((*p)->next)) {
        if((*p)->s == s) {
            item *tmp = *p;
            *p = tmp->next;
            delete tmp->s;
            delete tmp;
        }
    }
}

void GameServer::SendAll(int key, GameSession* except)
{
    for(const auto& a : item)
        if(*a != except)
        a->Send(key);
}

void GameServer::SendAll(char *message)
{
    for(const auto& a : item)
        if(*a != except)
        a->Send(message);
}

void GameServer::Process(bool r, bool w)
{
    if(!r)
        return;
    int sd;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    sd = accept(GetFd(), (struct sockaddr*) &addr, &len);
    if(sd == -1)
        return;
    if(item.size() >= max_gamer_number) {
        write(sd, already_playing_msg, sizeof(already_playing_msg));
        shutdown(sd, SHUT_RDWR);
        close(sd);
    } else {
        GameSession *p = new GameSession(this, sd);
        item.push_back(p);
        the_selector->Add(p);
        /* SendAll(mes, p);
        p->Send(form();////need change///// */
    bool ignoring;
    }
}


////////////////////////////SESSIONS///////////////////////////////////////////
class GameSession : FdHandler {
    friend class GameServer;

    std::string buffer; //for accumulate user sended data
    int play_nmbr;
    std::string name;  /* gamer name */
    GameServer *the_master;

    GameSession(GameServer *a_master, int fd);
    ~GameSession();

    void Process(bool r, bool w);
    void Send(int key);
    void Send(char *message);

    /*in this part will be function for process session*/
}; 

GameSession::GameSession(GameServer *a_master, int fd)
        : FdHandler(fd), play_number(fd),  ..... ////////
{}


void GameSession::Process(bool r, bool w)
{
    if(!r)
        return;
    if(the_master->game_begun == false) {
        write(GetFd(), game_n_beg_msg, sizeof(game_n_beg_msg));
        return;
    }
}

void FormStr(char *src)
{}

void GameSession::Send(int key)
{
    switch(key) {
        welcome_key:
            break; 
        player_joined_key:
            break;
        default:
            the_master->SendAll("Unexpected behavior from server: 
                                        unexpected key for Send\n");
    }
}        

void Send(char *message)
{
    write(GetFd(), message, sizeof(message));
}
