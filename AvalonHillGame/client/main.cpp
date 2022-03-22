#include <sys/socket.h>
#include <sys/types.h>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include "client.h"





int main(int argc, char **argv)
{
    int port;
    char *serv_ip;

    //Here i must to process input hostname and port
    if(argc < 3) {
        std::cout << "Please enter server ip and port to connection!\n";
        if(argc < 2) {
            std::cout << "Enter server ip (example: 192.168.0.1): ";
            std::cin >> tmp;
            Memove(serv_ip, tmp.c_str(), tmp(size));
            std::cout << "Enter server port: ";
            std::cin >> port;
        } else {
            if(is_addres(argv[1])) {
                Memove(serv_ip, argv[1], strlen(argv[1]));
                std::cout << "Enter server port: ";
                std::cin >> port;
            } else {
                port = atoi(argv[1]);
                std::cout << "Enter server ip (example: 192.168.0.1): ";
                std::cin >> tmp;
                Memove(serv_ip, tmp.c_str(), tmp(size));
            }
        }
    }

    if(!is_addres(serv_ip)) {
        std::cout << "Invalid ip adress for server!\n";
        return 1;
    }

    ServerForClient* serv = ServerForClient::Start(serv_ip, port);
    Console* console = Console::Start(stdin);

    
    //main loop
    do {
        fd_set rds, wrs;
        FD_ZERO(&rds);
        FD_ZERO(&wrs);

        FD_SET(serv.GetFd(), &rds);
        FD_SET(serv.GetFd(), &wrs);
        FD_SET(console.GetFd(), &rds);
        FD_SET(console.GetFd(), &wrs);

        res = select(serv.GetFd()+1, &rds, &wrs, 0, 0);
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
        if(FD_ISSET(sock_fd, &wds))
            


    }
    while(!quit);

    return 0;
}