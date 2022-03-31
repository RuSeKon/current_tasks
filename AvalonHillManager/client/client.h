#ifndef CLIENT_SENTRY
#define CLIENT_SENTRY

#include <sys/socket.h>
#include <sys/types.h>
#include <tuple>
#include "application.h"

//Function wrappers
bool IsAdress(const char*);
int Socket(int, int, int);
void Connect(int, const struct sockaddr*, socklen_t);
void Memove(void *dst, const void* src, int size);
int Write(int, const void*, size_t);
int Read(int, void*, size_t);
int Send(int, const void*, size_t, int);
int Recv(int, void*, size_t, int);
std::tuple<std::string, std::string> Parse(int, char**);

enum constants { 
        g_BufSize = 256,
        g_ValidPort = 1000
};

class ServerForClient : public IFdHandler {
    char m_Buffer[256];
    int m_BufUsed;
    ServerForClient(int fd) : IFdHandler(fd), m_BufUsed(0) {};
public:
    void VProcessing(bool r, bool w);
    static ServerForClient* Start(std::string& adress, int port);
};

class Console : public IFdHandler {
    ServerForClient* m_pTheMaster;
    char m_Buffer[256];
    int m_BufUsed;
    Console(ServerForClient* master, int fd) : IFdHandler(fd), m_pTheMaster(master), m_BufUsed(0) {};
public:
    void VProcessing(bool r, bool w);
    static Console* Start(ServerForClient* master, int fd);
};

#endif
