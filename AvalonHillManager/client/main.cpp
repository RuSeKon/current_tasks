#include <sys/socket.h>
#include <sys/types.h>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include "client.h"





int main(int argc, char **argv)
{
    int port;
    char *server_ip;

    std::cout << "            ----Welcome to the Manager game from Avalon Hill!!!!----"
    //Here i must to process input hostname and port
    while(!is_addres(server_ip)) {
        std::tuple<int, char*> tmp = Parse(argc, argv);
        port = std::get<0>(tmp);
        Memove(server_ip, std::get<1>(tmp), strlen(std::get<1>(tmp)));
        if(is_addres(server_ip)) break; 
        std::cout << "Invalid ip adress for server! Try again\n";
    }
 
    ServerForClient* serv = ServerForClient::Start(server_ip, port);
    Console* console = Console::Start(serv, stdin);
    
    //main loop
    do {
        fd_set rds, wrs;
        FD_ZERO(&rds);
        FD_ZERO(&wrs);

        FD_SET(serv.GetFd(), &rds);
        FD_SET(serv.GetFd(), &wrs);
        FD_SET(console.GetFd(), &rds);
        FD_SET(console.GetFd(), &wrs);

        int res = select(serv.GetFd()+1, &rds, &wrs, 0, 0);
        if(res < 0) {
            if(errno == EINTR) //If we need to process input signals
                continue;
            else
                break; //Need to proceed!!!!!
        }
        
        //read_from_buffer
        if(FD_ISSET(serv.GetFd(), &rds))
        {
            serv.Process(1, 0);
        }

        //write_to_buffer  
        if(FD_ISSET(console.GetFd(), &rds) && FD_ISSET(serv.GetFd(), &wds)
        {
            console.Process(1, 0);
        }
            
    }
    while(!quit);

    return 0;
}
