/* In this part of server defined work with client sessions */

#include "server.hpp"

class ChatSession;

class HighServer : public FdHandler {
    EventSelector *the_selector;
    struct item {
        ChatSession *s;
        item *next;
    };
    item *first;

    ChatServer(EventSelector *sel, int fd);
public:
    ~ChatServer();
    static chatServer *Start(EventSelector *sel, int port);

    RemoveSession(ChatSession *s);
    SendAll(); /////////////////////////
private:
    virtual void Handle(bool r, bool w);
};

class ChatSession : FdHandler {
    friend class ChatServer;

    char buffer[max_line_length+1]; //for messages
    /* int buf_used;
    bool ignoring;

    char *name; ///////// part for send data variabels*/

    ChatServer *the_master;

    ChatSession(ChatServer *a_master, int fd);
    ~ChatSession();

    /*in this part will be function for process session*/
}; 


