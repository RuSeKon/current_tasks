#include "application.h"
#include "server.h"
#include <iostream>
#include <string>



int main(int argc, char **argv)
{
    int port{0};

    if(argc < 2) {
        std::cout << "Please input port number: ";
        std::cin >> port;
        std::cout << std::endl;
    } else {
        port = std::stoi(argv[1]);
    }

    enum const_for_port {
        VALID = 0,
        PERMITTED = 1000, 
    }

    if(port < VALID || port < PERMITTED) {
        std::cout << "Invalid port entered! Please try again.\n";
        return 1;
    }
        
    EventSelector *Selector = new EventSelector;
    GameServer *Server = GameServer::ServerStart(Selector, 777);
    if(!Server)
        return 1;
    Selector->Run();          
}
