#include <sys/socket.h>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include "server.h"
#include "application.h"
 

/////////////////////////////////////////SERVER//////////////////////////////////////

GameServer::GameServer(EventSelector *sel, int fd)
        : IFdHandler(fd), m_pSelector(sel), m_pItemHandler(nullptr), 
        m_GamerCounter(0), m_GameBegun(false) 
{
    m_pSelector->Add(this);
}

GameServer::~GameServer()
{
    while(m_pItemHandler) {
        item *tmp = m_pItemHandler;
        m_pItemHandler = m_pItemHandler->next;
        m_pSelector->Remove(tmp->session);
        delete tmp;
    }
    m_pSelector->Remove(this);
}

GameServer *GameServer::ServerStart(EventSelector *sel, int port)
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
    m_pSelector->Remove(s);
    for(item **tmp = &m_pItemHandler; *tmp; tmp = &((*tmp)->next)) {
        if((*tmp)->session == s) {
            item *p = *tmp;
            *tmp = p->next;
            delete p->session;
            delete p;
            m_GamerCounter--;
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
    if(key == player_joined_key) {
        char* res = new char[sizeof(g_WelcomeAllMsg)+g_MaxName+3];
        sprintf(res, g_WelcomeAllMsg, except->name, except->GetNumber());
        SendAll(res, except);
    }

    for(item *tmp = m_pItemHandler; tmp != nullptr; tmp = tmp->next)
        if(tmp->session != except)
            tmp->session->Send(key);
}

void GameServer::SendAll(char *message, GameSession* except)
{
    for(item *tmp = m_pItemHandler; tmp != nullptr; tmp = tmp->next)
        if(tmp->session != except)
            tmp->session->Send(message);
}

void GameServer::VProcessing(bool r, bool w)
{
    if(!r)  //Explantation on README
        return;
    int session_descriptor;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    session_descriptor = accept(GetFd(), (struct sockaddr*) &addr, &len);
    if(session_descriptor == -1)
        return;
    if(m_GameBegun) {       //session not creating
        write(session_descriptor, g_AlreadyPlayingMsg, sizeof(g_AlreadyPlayingMsg));
        shutdown(session_descriptor, SHUT_RDWR);
        close(session_descriptor);
    } else {
        item *tmp = new item(m_GamerCounter);
        tmp->session = new GameSession(this, session_descriptor, m_GamerCounter);
        tmp->next = m_pItemHandler;
        m_pItemHandler = tmp;
        ++m_GamerCounter;
        m_pSelector->Add(tmp->session);
        

        SendAll(player_joined_key, tmp->session);
        tmp->session->Send(welcome_key);
    }
    if(m_GamerCounter == g_MaxGamerNumber) // need attantion
        m_GameBegun = true;
}
