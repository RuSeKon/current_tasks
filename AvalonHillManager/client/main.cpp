#include <sys/socket.h>
#include <sys/types.h>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <tuple>
#include "client.h"





int main(int argc, char **argv)
{
    int port;
    std::string server_ip;

    bool quit{false};
    bool end{false};

    std::cout << "\n\n            ----Welcome to the Manager game from Avalon Hill!!!!----\n\n";
    //Here i must to process input hostname and port
    while(!end) {
        std::tuple<std::string, std::string> tmp = Parse(argc, argv);
        if(!std::get<0>(tmp).size())
        {
            std::cout << "Port field is empty, try again.\n";
            continue;
        }
        else if(!std::get<1>(tmp).size())
        {
            std::cout << "Addres field is empty, try again.\n";
            continue;
        }
        port = std::stoi(std::get<0>(tmp));
	    server_ip = std::get<1>(tmp);
        if(!IsAdress(server_ip.c_str()))
	    {
            std::cout << "\nInvalid ip adress for server! Try again!\n\n";
            continue;
        } else if(port < g_ValidPort) {
            std::cout << "\nInvalid port! Try again!\n\n";
            continue;
        }
        end = true;
    }
 
    ServerForClient* serv = ServerForClient::Start(server_ip, port);
    Console* console = Console::Start(serv, STDIN_FILENO);
    
    //main loop
    do {
        fd_set rds, wrs;
        FD_ZERO(&rds);
        FD_ZERO(&wrs);

        FD_SET(serv->GetFd(), &rds);
        FD_SET(serv->GetFd(), &wrs);
        FD_SET(console->GetFd(), &rds);
        FD_SET(console->GetFd(), &wrs);

        int res = select(serv->GetFd()+1, &rds, &wrs, 0, 0);
        if(res < 0) {
            if(errno == EINTR) //If we need to process input signals
                continue;
            else
                break; //Need to proceed!!!!!
        }
        
        //read_from_buffer
        if(FD_ISSET(serv->GetFd(), &rds))
        {
            serv->VProcessing(1, 0);
        }

        //write_to_buffer  
        if(FD_ISSET(console->GetFd(), &rds) && FD_ISSET(serv->GetFd(), &wrs))
        {
            console->VProcessing(1, 0);
        }
            
    } while(!quit);

    return 0;
}
