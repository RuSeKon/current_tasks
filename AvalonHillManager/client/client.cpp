#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <tuple>
#include <cstring>
#include "client.h"
#include "application.h"


/* Client Object-oriented design implementation */

bool IsAdress(const char* src)
{
    if(!src) return false;
    int res{0};
    for(size_t x=0; x < strlen(src); x++)
        if(src[x] == '.') res++;
    return res == 3;
};

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

int Send(int sockfd, const void *buf, size_t len, int flags)
{
    int res = send(sockfd, buf, len, flags);
    if(res == -1)
    {
        std::cerr << "Error send\n";
        exit(EXIT_FAILURE);
    }
    return res;
};

int Recv(int sockfd, void *buf, size_t len, int flags)
{
    int res = recv(sockfd, buf, len, flags);
    if(res == -1)
    {
        std::cerr << "Error recv\n";
        exit(EXIT_FAILURE);
    }
    return res;
};

std::tuple<std::string, std::string> Parse(int cnt, char **src)
{
    std::string tmp, port;

    if(cnt < 3) {
        std::cout << "                 Please enter server ip and port for connection!" << std::endl << std::endl;
        if(cnt < 2) {
            std::cout << "Enter server ip (example: 192.168.0.1): ";
	    std::getline(std::cin, tmp);
	    std::cout << std::endl;
	    std::cout << "Enter server port: ";
            std::getline(std::cin, port);
	    std::cout << std::endl;
        } else {
            if(IsAdress(src[1])) {
                tmp = src[1];
                std::cout << "\nEnter server port: ";
                std::getline(std::cin, port);
            } else {
                port = src[1];
                std::cout << "\nEnter server ip (example: 192.168.0.1): ";
                std::getline(std::cin, tmp);
            }
        }
    } else {
        if(IsAdress(src[2])) {
            port = src[1];
        }
    }
    return std::make_tuple(port, tmp);

};


ServerForClient* ServerForClient::Start(std::string& adress, int port)
{
    int sock_fd = Socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if(!inet_aton(adress.c_str(), &(serv_addr.sin_addr))) {
       std::cerr << "Error ip convertion by inet_aton\n";
       exit(EXIT_FAILURE);
    }   
    
    Connect(sock_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

    return new ServerForClient(sock_fd);
};

void ServerForClient::VProcessing(bool r, bool w)
{
    if(r) {
        m_BufUsed = Recv(GetFd(), m_Buffer, g_BufSize, 0);
        if(m_BufUsed == -1)
        {
            std::cerr << "Error reading from server\n";
            exit(EXIT_FAILURE);
        }
        if(m_BufUsed >= g_BufSize-1) {
            std::cerr << "Error m_Buffer overflow\n";
            exit(EXIT_FAILURE);
        }

        int b{0}; 
        for(int i=0; m_Buffer[i]; i++)
        {
            if(m_Buffer[i] == '\n') {
                Write(STDOUT_FILENO, "FROM SERVER: ", 13);
                Write(STDOUT_FILENO, m_Buffer+b, i+1-b);
                b += i+1;
                m_BufUsed -= i+1;
            }
        }
        Memove(m_Buffer, m_Buffer+b, b);
    
    }
    
};

Console* Console::Start(ServerForClient* master, int fd)
{
    return new Console(master, fd);
};

void Console::VProcessing(bool r, bool w)
{
    if(r) {
        m_BufUsed = Send(m_pTheMaster->GetFd(), m_Buffer, g_BufSize, 0);
        if(m_BufUsed == -1)
        {
            std::cerr << "Error sending to server\n";
            exit(EXIT_FAILURE);
        }

        if(m_BufUsed >= g_BufSize) {
            std::cerr << "Error m_Buffer overflow\n";
            exit(EXIT_FAILURE);
        } else { m_Buffer[m_BufUsed] = '\0';}

	    if(strstr(m_Buffer, "quit"))
		    exit(0);

        Write(1, "FROM ME: ", 9);
        Write(1, m_Buffer, m_BufUsed);

        Memove(m_Buffer, m_Buffer+m_BufUsed, m_BufUsed);
    
    }
};
