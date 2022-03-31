#include <sys/socket.h>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <memory>
#include <iostream>
#include <cstring>
#include "server.h"
#include "application.h"



////////////////////////////SESSIONS/////////////////////////////////////////////////////

GameSession::GameSession(GameServer *a_master, int fd, int pl_nmbr)
        : IFdHandler(fd), m_pTheMaster(a_master), m_BufUsed(0),  
        m_PlayNumber(pl_nmbr)/*, factories(2), rawMaterial(4), money(10000)*/
{
    Send(g_Greeting);
}

//GameSession::~GameSession()


void GameSession::VProcessing(bool r, bool w) {
    if(!r)
        return;

    if(m_pTheMaster->GameBegun() == false) {
        if(!m_Name) 
        {
            recv(GetFd(), m_Name, g_MaxName, 0);
            std::unique_ptr<char> res(new char[strlen(g_WelcomeAllMsg)+g_MaxName+3]);
            sprintf(res.get(), g_WelcomeAllMsg, m_Name, m_PlayNumber);
            m_pTheMaster->SendAll(res.get(), this);
            sprintf(res.get(), g_WelcomeMsg, m_Name, m_PlayNumber);
            Send(res.get());
        } else {
            Send(g_GameNotBegunMsg);
            return;
        }
    } else {
        m_BufUsed = recv(GetFd(), m_Buffer, g_BufSize, 0);
        if(m_BufUsed == -1)
        {
            std::cerr << "Error reading from client number: " << m_PlayNumber << std::endl;
            exit(5);
        }
        if(m_BufUsed >= g_BufSize-1) {
            std::cerr << "Error m_Buffer of client " << m_PlayNumber << " overflow\n";
            exit(6);
        } else { m_Buffer[m_BufUsed] = '\0';}
        int i{0}; 
        for(int b=0; m_Buffer[i]; i++)
        {
            if(m_Buffer[i] == '\n') {
                write(1, "FROM CLIENT: ", 13);
                write(1, m_Buffer+b, i+1-b);
                b += i+1;
            }
            m_BufUsed -= i+1;
        }
        memmove(m_Buffer, m_Buffer+i+1, i+1);
    }




    ///Here data from player came, and we need to processe them and send to Game class///
}


/*char *GameSession::FormStr(int key) ///Need attantion
{
    char *res;
    switch(key) {
        welcome_key:
            std::auto_ptr<char> res(new  char[strlen(welcome)+max_name+3])
            sprintf(res, g_WelcomeMsg, m_PlayNumber, m_PlayNumber);

            return res;
        info_key:
            
        default:
        #ifdef DEBUG
            m_pTheMaster->SendAll("Unexpected behavior from server: "
                                        "unexpected key for Send\n");
        #endif
    }
    return nullptr;
}        

void GameSession::Send(int key)
{
    char *mes = FormStr(key);
    send(GetFd(), mes, strlen(mes));
    delete [] mes;
}
*/

void GameSession::Send(const char *message)
{
    int res{0};
    res = send(GetFd(), message, strlen(message), 0);
    if(res == -1) {
        std::cout << "Client if offline\n";
        return;
    }
}
