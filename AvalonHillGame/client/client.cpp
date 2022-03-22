#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include "client.h"
#include "application.h"


/* Client Object-oriented design implementation */

bool is_addres(char *src)
{
    int res{0};
    for(int i=0; src[i]; i++)
        if(src[i] == '.') res++;
    return res == 3;
};

int Socket(int domain, int type, int protocol)
{
    int res = socket(domain, type, protocol);
    if(res == -1)
    {
        std::cerr << "Error socket creation\n";
        exit(2);
    }
    return res;
};

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    int res = connect(sockfd, addr, addrlen);
    if(res == -1)
    {
        std::cerr << "Connection error\n";
        exit(4);
    }
};

void Memove(void *dst, const void* src, int size)
{
    void *tmp;
    tmp = memmove(dst, src, size);
    if(!tmp)
    {
        std::cerr << "Error allocating memory\n";
        exit(7);
    }
};

void Write(int fd, const void *buf, size_t count)
{
    int res = write(fd, buf, count);
    if(res == -1)
    {
        std::cerr << "Error write to server\n";
        exit(8)
    }
}

ServerForClient::ServerForClient

ServerForClient* ServerForClient::Start(char *adress, int port)
{
    int sock_fd = Socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htonl(port);
    if(!inet_aton(adress, &(serv_addr.sin_addr.s_addr))) {
       std::cerr << "Error ip convertion by inet_aton\n";
       exit(3);
    }   
    
    Connect(sock_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

    return ServerForClient(fd);
}

void ServerForClient::Process(bool r, bool w)
{
    if(r) {
        buf_used = recv(sock_fd, buffer, buf_size, 0);
        if(buf_used == -1)
        {
            std::cerr << "Error reading from server\n";
            exit(5);
        }
        if(buf_used >= buf_size-1) {
            std::cerr << "Error buffer overflow\n";
            exit(6);
        } else { buffer[buf_used] = '\0';}

        int i{0}; 
        for(int b=0; buffer[i]; i++)
        {
            if(buffer[i] == '\n') {
                Write(stdout, buffer+b, i+1-b);
                b += i+1;
            }
            buf_used -= i+1;
        }
        Memove(buffer, buffer+i+1, i+1);
    
    }
    
}

