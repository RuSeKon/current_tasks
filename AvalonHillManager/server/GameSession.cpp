#include <sys/socket.h>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <memory>
#include "server.hpp"
#include <application.h>



////////////////////////////SESSIONS/////////////////////////////////////////////////////

GameSession::GameSession(GameServer *a_master, int fd, int pl_nmbr)
        : FdHandler(fd), the_master(a_master), buffer(nullptr), buf_used(0),
          play_nmbr(pl_nmbr), factories(2), rawMaterial(4), money(10000)
{
    Send("Your welcome! Enter you name...\n");
}


void GameSession::Process(bool r, bool w)
{
    if(!r)
        return;

    if(the_master->game_begun == false) {
        if(!name) 
        {
            read(GetFd(), name, max_name);
            Send(welcome_key);
        }
        write(GetFd(), game_n_beg_msg, sizeof(game_n_beg_msg));
        return;
    }




    ///Here data from player came, and we need to processe them and send to Game class///
}


char *GameSession::FormStr(int key) ///Need attantion
{
    char *res;
    switch(key) {
        welcome_key:
            char* res(new  char[sizeof(welcome)+max_name+3]); 
            sprintf(res, welcome, name, play_nmbr);
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
