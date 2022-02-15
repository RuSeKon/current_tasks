/* 
----In this part of server defined work with client sessions----

    In this implementation I use a key parametr for method "Send". In this case
it provide me opportunity to form output string consist from string, int values,
and other variables (after cast to *str) implicity on body of Send.
*/

#include <sys/socket.h>
#include "server.hpp"

#ifndef MAXGAMERNUMBER
#define MAXGAMERNUMBER 10
#endif 


GameServer::GameServer(EventSelector *sel, int fd)
        : FdHandler(fd), the_selector(sel), game_begun(false),
        first(0), gamer_count(0)
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
            gamer_counter--;

        }
    }
}

void GameServer::SendAll(int key, GameSession* except)
{
    for(const auto& a : item)
        if(*a != except)
        a->Send(key);
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
    if(++gamer_counter >= max_gamer_number) {//session in this case not creating
        write(sd, already_playing_msg, sizeof(already_playing_msg));
        shutdown(sd, SHUT_RDWR);
        close(sd);
    } else {
        item *tmp = new item;
        item->s = new GameSession(this, sd, gamer_counter);
        item->next = first;
        first = item;
        the_selector->Add(tmp->s);
        SendAll(player_joined_key);
        //p->Send();////need change///// */
    }
    if(game_begun == false)
        if(gamer_counter >= max_gamer_number)
            GameStart();    //Needed implementation!!!!!
}

////////////////////////////SESSIONS///////////////////////////////////////////
GameSession::GameSession(GameServer *a_master, int fd, int pl_nmbr)
        : FdHandler(fd), the_master(a_master), play_nmbr(pl_nmbr),
        name(nullptr)
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
    */
    
}


char *GameSession::FormStr(int key)
{
    char *res;
    switch(key) {
        welcome_key:
            if((res = (char *)malloc(sizeof(welcome)+max_name+3)) == NULL) 
                return NULL;
            sprintf(res, welcome, name, play_nmbr);
            return res;
        player_joined_key:
            if((res = (char *)malloc(sizeof(welcome_all)+max_name+3)) == NULL)
                return NULL;
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
}

void GameSession::Send(char *message)
{
    write(GetFd(), message, sizeof(message));
}
