#include "application.h"
#include "server.h"




//Game Avalanch{};
int main(int argc, char **argv)
{   
    EventSelector *Selector = new EventSelector;
    GameServer *Server = GameServer::ServerStart(Selector, 777);
    if(!Server)
        return 1;
    Selector->Run();
    for(;;) {
        if(Server->game_begun == true) {
          
    }
}
