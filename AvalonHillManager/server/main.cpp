#include "application.h"
#include "server.h"
#include <iostream>
#include <string>



int main(int argc, char **argv)
{
    std::string tmp;

    if(argc < 2) {
        std::cout << "Please input port number: ";
        std::getline(std::cin, tmp);
        std::cout << std::endl;
    } else {
        tmp = argv[1];
    }

    enum const_for_port {
        VALID = 0,
        //PERMITTED = 1000, 
    };

    
    if(tmp.size() < 4)  //for permitted and not entered port string
    {
        std::cout << "Invalid port entered! Please try again.\n";
        return 1;
    }
        
    int port = std::stoi(tmp);

    EventSelector *Selector = new EventSelector;
    GameServer *Server = GameServer::ServerStart(Selector, port);
    if(!Server)
        return 1;
    Selector->Run();
    std::cout << "I am here\n";
    return 0;     
}
