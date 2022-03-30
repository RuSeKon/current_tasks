#ifndef CLIENT_SENTRY
#define CLIENT_SENTRY

#include <sys/socket.h>
#include <sys/types.h>
#include <tuple>
#include "application.h"

//Function wrappers
bool is_addres(const char*);
int Socket(int, int, int);
void Connect(int, const struct sockaddr*, socklen_t);
void Memove(void *dst, const void* src, int size);
int Write(int, const void*, size_t);
int Send(int, const void*, size_t, int);
int Recv(int, void*, size_t, int);
std::tuple<std::string, std::string> Parse(int, char**);

enum constants { 
        buf_size = 256,
        valid_port = 1000
};

static const char mesg[] = ("Do you want to say something?\n");

class ServerForClient : public IFdHandler {
    char buffer[256];
    int buf_used;
    ServerForClient(int fd) : IFdHandler(fd), buf_used(0) {};
public:
    void VProcessing(bool r, bool w);
    static ServerForClient* Start(std::string& adress, int port);
};

class Console : public IFdHandler {
    ServerForClient* the_master;
    char buffer[256];
    int buf_used;
    Console(ServerForClient* master, int fd) : IFdHandler(fd), the_master(master), buf_used(0) {};
public:
    void VProcessing(bool r, bool w);
    static Console* Start(ServerForClient* master, int fd);
};

#endif
