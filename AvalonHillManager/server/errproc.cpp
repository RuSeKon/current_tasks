#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "errproc.h"

int Socket(int domain, int type, int protocol)
{
    int res = socket(domain, type, protocol);
    if(res == -1)
    {
        std::cerr << "Error socket creation\n";
        exit(EXIT_FAILURE);
    }
    return res;
};

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    int res = connect(sockfd, addr, addrlen);
    if(res == -1)
    {
        std::cerr << "Connection error\n";
        exit(EXIT_FAILURE);
    }
};

void Memove(void *dst, const void* src, int size)
{
    void *tmp;
    tmp = memmove(dst, src, size);
    if(!tmp)
    {
        std::cerr << "Error allocating memory\n";
        exit(EXIT_FAILURE);
    }
};

int Write(int fd, const void *buf, size_t count)
{
    int res = write(fd, buf, count);
    if(res == -1)
    {
        std::cerr << "Error write to socket\n";
        exit(EXIT_FAILURE);
    }
    return res;
};

int Read(int fd, void *buf, size_t count)
{
    int res = read(fd, buf, count);
    if(res == -1)
    {
        std::cerr << "Error reading\n";
        exit(EXIT_FAILURE);
    }
    return res;
};

/*
int Send(int sockfd, const void *buf, size_t len)
{
    int res = send(sockfd, buf, len, 0);
    if(res == -1)
    {
        std::cerr << "Error send\n";
        exit(EXIT_FAILURE);
    }
    return res;
};
*/

int Recv(int sockfd, void *buf, size_t len)
{
    int res = recv(sockfd, buf, len, 0);
    if(res == -1)
    {
        std::cerr << "Error recv\n";
        exit(EXIT_FAILURE);
    }
    return res;
};