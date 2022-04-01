#include <sys/socket.h>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <memory>
#include <iostream>
#include <cstring>
#include "server.h"
#include "errproc.h"
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
        m_pSelector->Remove(tmp->Session);
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
    /*
    char buf[INET_ADDRSTRLEN]{0};
    inet_ntop(AF_INET, &addr.sin_addr, buf, INET_ADDRSTRLEN); //need reper 
    std::cout << "Server adress: " << buf << std:: endl;
    */

    return new GameServer(sel, ls);
}

void GameServer::RemoveSession(GameSession *s)
{
    m_pSelector->Remove(s);
    for(item **tmp = &m_pItemHandler; *tmp; tmp = &((*tmp)->next)) {
        if((*tmp)->Session == s) {
            item *p = *tmp;
            *tmp = p->next;
            delete p->Session;
            delete p;
            m_GamerCounter--;
            return;
        }
    }
}

void GameServer::SendMsgAll(const char *message, GameSession* except)
{
    for(item *tmp = m_pItemHandler; tmp != nullptr; tmp = tmp->next)
        if(tmp->Session != except)
            tmp->Session->SendMsg(message);
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
        send(session_descriptor, g_AlreadyPlayingMsg, strlen(g_AlreadyPlayingMsg), 0);
        shutdown(session_descriptor, SHUT_RDWR);
        close(session_descriptor);
    } else {
        item *tmp = new item(m_GamerCounter);
        tmp->Session = new GameSession(this, session_descriptor, m_GamerCounter);
        tmp->next = m_pItemHandler;
        m_pItemHandler = tmp;
        ++m_GamerCounter;
        m_pSelector->Add(tmp->Session);
        
        ///SendMsg message about joined new player
        /*std::unique_ptr<char> res(new char[strlen(g_WelcomeAllMsg)+g_MaxName+3]);
        sprintf(res, g_WelcomeAllMsg, except->GetName(), except->GetNumber());
        SendMsgAll(res, tmp->Session);
        sprintf(res, g_WelcomeMsg, tmp->Session->GetName(), tmp->Session->GetNumber());
        tmp->Session->SendMsg(res); */
    }
    if(m_GamerCounter == g_MaxGamerNumber) { // need attantion
        m_GameBegun = true;
        SendMsgAll(g_GameStartSoon, nullptr);
    }
}
