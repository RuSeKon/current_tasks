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
        : IFdHandler(fd), the_master(a_master), buf_used(0),  
        play_nmbr(pl_nmbr)/*, factories(2), rawMaterial(4), money(10000)*/
{
    Send("Your welcome! Enter you name...\n");
}

void GameSession::VProcessing(bool r, bool w) {
    if(!r)
        return;

    if(the_master->m_GameBegun == false) {
        if(!name) 
        {
            recv(GetFd(), name, g_MaxName, 0);
            std::auto_ptr<char> res(new char[sizeof(g_WelcomeAllMsg)+g_MaxName+3]);
            sprintf(res.get(), g_WelcomeAllMsg, name, play_nmbr);
            the_master->SendAll(res.get(), this);
            sprintf(res.get(), g_WelcomeMsg, name, play_nmbr);
            Send(res.get());
        } else {
            Send(g_GameNotBegunMsg);
            return;
        }
    } else {
        buf_used = recv(GetFd(), buffer, g_BufSize, 0);
        if(buf_used == -1)
        {
            std::cerr << "Error reading from client number: " << play_nmbr << std::endl;
            exit(5);
        }
        if(buf_used >= g_BufSize-1) {
            std::cerr << "Error buffer of client " << play_nmbr << " overflow\n";
            exit(6);
        } else { buffer[buf_used] = '\0';}
        int i{0}; 
        for(int b=0; buffer[i]; i++)
        {
            if(buffer[i] == '\n') {
                write(1, "FROM CLIENT: ", 13);
                write(1, buffer+b, i+1-b);
                b += i+1;
            }
            buf_used -= i+1;
        }
        memmove(buffer, buffer+i+1, i+1);
    }




    ///Here data from player came, and we need to processe them and send to Game class///
}


/*char *GameSession::FormStr(int key) ///Need attantion
{
    char *res;
    switch(key) {
        welcome_key:
            std::auto_ptr<char> res(new  char[sizeof(welcome)+max_name+3])
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
*/

void GameSession::Send(const char *message)
{
    write(GetFd(), message, sizeof(message));
}
