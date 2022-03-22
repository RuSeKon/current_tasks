#ifndef CLIENT_SENTRY
#define CLIENT_SENTRY

#include <sys/socket.h>
#include <sys/types.h>
#include "application.h"

//Function wrappers
bool is_addres(char *);
int Socket(int, int, int);
void Connect(int, const struct sockaddr*, socklen_t);
void Memove(void*, const void*, int);
void Write(int, const void*, size_t);

enum constants { 
        buf_size = 256
};

static const mesg[] = ("Do you want to say something?\n");

class ServerForClient : public FdHandler {
    char buffer[256];
    int buf_used;
    ServerForClient(int fd) : FdHandler(fd), buf_used(0) {};
public:
    ~ServerForClient() = default;
    void Process(bool r, bool w);
    static ServerForClient* Start(char *adress, int port);
};

class Console : public FdHandler {
    char buffer[256];
    int buf_used;
    Console(int fd) : FdHandler(fd), buf_used(0) {};
public:
    ~Console() = default;
    void Process(bool r, bool w);
    static Console* Start(int fd);
};

#endif