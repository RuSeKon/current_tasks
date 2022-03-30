#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <tuple>
#include <cstring>
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
        std::cerr << "Error write to socket\n";
        exit(8);
    }
};

int Send(int sockfd, const void *buf, size_t len, int flags)
{
    int res = send(sockfd, buf, len, flags);
    if(res == -1)
    {
        std::cerr << "Error send\n";
        exit(9);
    }
    return res;
};

int Recv(int sockfd, void *buf, size_t len, int flags);
{
    int res = send(sockfd, buf, len, flags);
    if(res == -1)
    {
        std::cerr << "Error recv\n";
        exit(10);
    }
    return res;
};

std::tuple<int, char*> Parse(int cnt, char **src)
{
    int port;
    char *serv_ip;
    std::string tmp;

    if(cnt < 3) {
        std::cout << "Please enter server ip and port to connection!\n";
        if(cnt < 2) {
            std::cout << "Enter server ip (example: 192.168.0.1): ";
            std::cin >> tmp;
            Memove(serv_ip, tmp.c_str(), tmp.size());
            std::cout << "\nEnter server port: ";
            std::cin >> port;
        } else {
            if(is_addres(src[1])) {
                Memove(serv_ip, src[1], strlen(src[1]));
                std::cout << "\nEnter server port: ";
                std::cin >> port;
            } else {
                port = std::stoi(src[1]);
                std::cout << "\nEnter server ip (example: 192.168.0.1): ";
                std::cin >> tmp;
                Memove(serv_ip, tmp.c_str(), tmp.size());
            }
        }
    } else {
        if(is_addres(src[2])) {
            port = std::stoi(src[1]);
            Memove(serv_ip, src[21], strlen(src[2]));
        }
    }
    return std::make_tuple(port, serv_ip);

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

    return new ServerForClient(fd);
};

void ServerForClient::Process(bool r, bool w)
{
    if(r) {
        buf_used = Recv(GetFd(), buffer, buf_size, 0);
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
                Write(1, "FROM SERVER: ", 13);
                Write(1, buffer+b, i+1-b);
                b += i+1;
            }
            buf_used -= i+1;
        }
        Memove(buffer, buffer+i+1, i+1);
    
    }
    
};

Console* Console::Start(ServerForClient* master, int fd)
{
    return new Console(master, fd);
};

void Console::Process(bool r, bool w)
{
    if(r) {
        buf_used = Send(the_master->GetFd(), buffer, buf_size, 0);
        if(buf_used == -1)
        {
            std::cerr << "Error sending to server\n";
            exit(11);
        }

        if(buf_used >= buf_size) {
            std::cerr << "Error buffer overflow\n";
            exit(6);
        } else { buffer[buf_used] = '\0';}

        Write(1, "FROM ME: ", 9);
        Write(1, buffer, buf_used);

        Memove(buffer, buffer+buf_used, buf_used);
    
    }
};
