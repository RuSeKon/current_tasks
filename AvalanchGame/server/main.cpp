#include "socket.hpp"
#include "server.hpp"


EventSelector *Selector = new EventSelector;
Selector->Run();

GameServer *Server = GameServer::Start(Selector, 777);

//Game Avalanch{};

for(;;) {
    if(Server->game_begun == true) {
        //processing;
        //Avalanch.start;
    }
}
