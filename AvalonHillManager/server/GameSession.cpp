#include <sys/socket.h>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include "server.hpp"
#include <application.h>



////////////////////////////SESSIONS/////////////////////////////////////////////////////

GameSession::GameSession(GameServer *a_master, int fd, int pl_nmbr)
        : FdHandler(fd), the_master(a_master), buffer(nullptr), buf_used(0),
          play_nmbr(pl_nmbr), factories(2), rawMaterial(4), money(10000)
{
    Send("Your welcome! Enter you name...\n");
}


void GameSession::VProcessing(bool r, bool w)
{
    if(!r)
        return;

    if(the_master->/*m_GameBegun */  == false) {
        if(!name) 
        {
            read(GetFd(), name, g_MaxName);
            Send(welcome_key);
        }
        write(GetFd(), g_GameNotBegunMsg, sizeof(g_GameNotBegunMsg));
        return;
    }




    ///Here data from player came, and we need to processe them and send to Game class///
}


char *GameSession::FormStr(int key) ///Need attantion
{
    char *res;
    switch(key) {
        welcome_key:
            res = new  char[sizeof(g_WelcomeMsg)+g_MaxName+3]; 
            sprintf(res, g_WelcomeMsg, name, play_nmbr);
            return res;
        info_key:
            
        default:
        #ifdef DEBUG
            the_master->SendAll("Unexpected behavior from server: "
                                        "unexpected key for Send\n");
        #endif
    }
    return nullptr;
}        

void GameSession::Send(int key)
{
    char *mes = FormStr(key);
    write(GetFd(), mes, sizeof(mes));
    delete [] mes;
}

void GameSession::Send(char *message)
{
    write(GetFd(), message, sizeof(message));
}
