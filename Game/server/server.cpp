/* In this part of server defined work with client sessions */

#include <list>
#include <sys/socket.h>
#include "socket.hpp"

#ifndef MAXGAMERNUMBER
#define MAXGAMERNUMBER 10
#endif 


class GameSession;

enum constants_to_server { no = 0, yes = 1, 
    max_gamer_number = MAXGAMERNUMBER };

///section for constant message initialization
static const char already_playing_msg[] = {"Sorry, game is already started." 
                        " You can play next round\n"}
static const char welcome[] = {"Welcome to the game, you play-number: "};
static const char player_joined[] = 

class GameServer : public FdHandler {
    EventSelector *the_selector;
    std::list<GameSession*, int num> item;
    int ready_to_game;
    int state;

    GameServer(EventSelector *sel, int fd);
public:
    ~GameServer();
    static GameServer *Start(EventSelector *sel, int port);

    RemoveSession(GameSession *s);
    void SendAll(char *message, ChatSession* except);
    void FormMessage(int templ, 
private:
    virtual void Process(bool r, bool w);
};


GameServer::GameServer(EventSelector *sel, int fd)
        : FdHandler(fd), the_selector(sel)
{
    the_selector->Add(this);
}

GameServer::~GameServer()
{
    item.clear();
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
    item.remove(s);
}

void GameServer::SendAll(char *message, GameSession* except)
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
        std::string mes = player_joined + 
        SendAll(mes, p);
        p->Send(form();
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

    void Send(std::string message);

    /*in this part will be function for process session*/
}; 

GameSession::GameSession(GameServer *a_master, int fd)
        : FdHandler(fd), play_number(fd),  ..... ////////
{}

