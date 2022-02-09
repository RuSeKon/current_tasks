/* In this part of server defined work with client sessions */

#include <list>
#include <sys/socket.h>
#include "server.hpp"

class GameSession;

class GameServer : public FdHandler {
    EventSelector *the_selector;
    std::list<GameSession*> item;

    GameServer(EventSelector *sel, int fd);
public:
    ~GameServer();
    static GameServer *Start(EventSelector *sel, int port);

    RemoveSession(GameSession *s);
    SendAll(); /////////////////////////
private:
    virtual void Handle(bool r, bool w);
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
    item.erase(s);
}

//void GameServer::SendAll//////////////

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
    GameSession *p = new GameSession(this, sd);
    item.push_back(p);

    the_selector->Add(p);
}


///////////////////////////////////////////////////////////////////////
class GameSession : FdHandler {
    friend class GameServer;

    char buffer[max_line_length+1]; //for messages
    /* int buf_used;
    bool ignoring;

    char *name; ///////// part for send data variabels*/

    GameServer *the_master;

    GameSession(GameServer *a_master, int fd);
    ~GameSession();

    /*in this part will be function for process session*/
}; 

GameSession::GameSession(GameServer *a_master, int fd)
        : FdHandler(fd), ..... ////////
{}

