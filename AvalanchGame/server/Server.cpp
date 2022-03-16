/* 
----In this part of server defined work with client sessions----
*/


#include <sys/socket.h>
#include "server.hpp"
 

/////////////////////////////////////////SERVER//////////////////////////////////////

GameServer::GameServer(EventSelector *sel, int fd)
        : FdHandler(fd), the_selector(sel), itemHandler(nullptr), 
        gamer_counter(0), game_begun(false) 
{
    the_selector->Add(this);
}

GameServer::~GameServer()
{
    while(itemHandler) {
        item *tmp = itemHandler;
        itemHandler = itemHandler->next;
        the_selector->Remove(tmp->session);
        delete tmp;
    }
    the_selector->Remove(this);
}

GameServer *GameServer::Start(EventSelector *sel, int port)
{
    struct sockaddr_in addr;

    int ls = socket(AF_INET, SOCK_STREAM, 0);
    if(ls == -1)
        return nullptr;

    int opt = 1;
    setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    int res = bind(ls, (struct sockaddr*) &addr, sizeof(addr));
    if(res == -1)
        return nullptr;

    res = listen(ls, 16);
    if(res == -1)
        return nullptr;
    return new GameServer(sel, ls);
}

void GameServer::RemoveSession(GameSession *s)
{
    the_selector->Remove(s);
    for(item **tmp = &first; *tmp; tmp = &((*tmp)->next)) {
        if((*tmp)->session == s) {
            item *p = *tmp;
            *tmp = p->next;
            delete p->session;
            delete p;
            gamer_counter--;
            return;
        }
    }
}

/*
In this implementation I use a key parametr for methods like Send. It provide
me opportunity to form output string consist from string, int values,and other
variables (after cast to *str) implicity on body of Send. */
void GameServer::SendAll(int key, GameSession* except)
{
    for(item tmp = itemHandler; tmp != nullptr; tmp = tmp->next)
        if(tmp->session != except)
            tmp->session->Send(key);
}

void GameServer::SendAll(char *message, GameSession* except)
{
    for(item tmp = itemHandler; tmp != nullptr; tmp = tmp->next)
        if(tmp->session != except)
            tmp->session->Send(message);
}

void GameServer::Process(bool r, bool w)
{
    if(!r)  //Explantation on README
        return;
    int session_descriptor;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    session_descriptor = accept(GetFd(), (struct sockaddr*) &addr, &len);
    if(session_descriptor == -1)
        return;
    if(++gamer_counter >= max_gamer_number) {       //session not creating
        write(session_descriptor, already_playing_msg, sizeof(already_playing_msg));
        shutdown(session_descriptor, SHUT_RDWR);
        close(session_descriptor);
    } else {
        item *tmp = new item(gamer_counter);
        tmp->session = new GameSession(this, sd, gamer_counter);
        tmp->next = itemHandler;
        itemHandler = tmp;
        the_selector->Add(tmp->session);

        SendAll(player_joined_key, tmp->session);
        tmp->session->Send(welcome_key);
    }
    if(gamer_counter == max_gamer_number)
        game_begun = true;
}

////////////////////////////SESSIONS///////////////////////////////////////////

GameSession::GameSession(GameServer *a_master, int fd, int pl_nmbr)
        : FdHandler(fd), buffer(nullptr), buf_used(0), play_nmbr(pl_nmbr), 
        name(nullptr), the_master(a_master) 
{
    Send("Your welcome! Enter you name...");
}


void GameSession::Process(bool r, bool w)
{
    if(!r)
        return;
    /*
    if(the_master->game_begun == false) {
        write(GetFd(), game_n_beg_msg, sizeof(game_n_beg_msg));
        return;
    }
    ///Here data from player came, and we need to processe them and send to Game class///
    */
    
}


char *GameSession::FormStr(int key) ///Need attantion
{
    char *res;
    switch(key) {
        welcome_key:
            res = new char[sizeof(welcome)+max_name+3]; 
            sprintf(res, welcome, name, play_nmbr);
            return res;
        player_joined_key:
            res = new char[sizeof(welcome_all)+max_name+3];
            sprintf(res, welcome_all, name, play_nmbr);
            return res;
        default:
            the_master->SendAll("Unexpected behavior from server: "
                                        "unexpected key for Send\n");
    }
}        

void GameSession::Send(int key)
{
    char *mes = FormStr(key);
    write(GetFd(), mes, sizeof(mes));
    delete mes[];
}

void GameSession::Send(char *message)
{
    write(GetFd(), message, sizeof(message));
}
