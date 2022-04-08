#include <iostream>
#include <string>
#include "share/application.h"
#include "share/server.h"
#include "application.h"
#include "game.h"


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

    
	if(tmp.size() < 4 || tmp[0] == '-')  //for permitted, not entered and not valid port string
	{
		std::cerr << "Invalid port entered! Please try again.\n";
		return 1;
	}
        
	int port = std::stoi(tmp);

	EventSelector *Selector = new EventSelector;
	Game *Server = Game::GameStart(Selector, port);
	if(!Server)
		return 1;

	Selector->Run();
	std::cout << "Game over:)\n";
	return 0;     
}
