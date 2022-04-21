#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <tuple>
#include <cstring>
#include "share/client.h"
#include "share/application.h"
#include "share/errproc.h"


static const char g_QuitMsg[] {"quit"};
static const char g_HelpMsgFlag[] {"helpMe"};
static const char g_HelpMsg[] {	"_______________________________________________________________\n"
								"|  market | info about current market condition				|\n"
								"|______________________________________________________________|\n"
								"|   info  | info about another player							|\n"
								"|______________________________________________________________|\n"
								"| infoLst | list of all remaining players						|\n"
								"|______________________________________________________________|\n"
								"|   buy   | applying for the purchase of raw materials			|\n"
								"|______________________________________________________________|\n"
								"|   sell  | submitting an application for the sale of products |\n"
								"|______________________________________________________________|\n"
								"|  build  | construction of a new factory						|\n"
								"|______________________________________________________________|\n"
								"|   turn  | completion confirmation							|\n"
								"|______________________________________________________________|\n"
								"|   help  | available commands									|\n"
								"|______________________________________________________________|\n"};
/* Client Object-oriented design implementation */

bool IsAdress(const char* src)
{
	if(!src) return false;
	int res{0};
	for(size_t x=0; x < strlen(src); x++)
		if(src[x] == '.') res++;
	return res == 3;
};



std::tuple<std::string, std::string> Parse(int cnt, char **src)
{
	std::string tmp, port;

	if(cnt < 3) {
		std::cout << std::endl;
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
		if(IsAdress(src[1])) {
			port = src[2];
			tmp = src[1];
		}
		else
		{
			tmp = src[2];
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
	if(r) 
	{
		m_BufUsed = recv(GetFd(), m_Buffer, g_BufSize, 0);
		if(m_BufUsed == -1)
		{
			std::cout << "Error server doesn't respond:(\n";
			exit(EXIT_FAILURE);
		}
		else if(m_BufUsed >= g_BufSize-1) 
		{
			std::cerr << "Error buffer overflow\n";
			exit(EXIT_FAILURE);
		}
		else if(strstr(m_Buffer, g_HelpMsgFlag))
		{
			Write(STDOUT_FILENO, g_HelpMsg, g_HelpMsgSize);
			m_BufUsed = 0;
			return;
		}
		else if(m_BufUsed == 0)
		{
			std::cout << "Game over:)\n";
			exit(EXIT_FAILURE);
		}

		Write(STDOUT_FILENO, "SERVER: ", 8);
		for(int i=0; i < m_BufUsed; i++)
		{
			if(m_Buffer[i] == '\n') 
			{
				Write(STDOUT_FILENO, m_Buffer, i+1);
				Memove(m_Buffer, m_Buffer+i+1, m_BufUsed);
				m_BufUsed -= i+1;
				i = -1;
			}
		}
	}
};

Console* Console::Start(ServerForClient* master, int fd)
{
	return new Console(master, fd);
};

void Console::VProcessing(bool r, bool w)
{
	if(r) 
	{
		m_BufUsed = Read(GetFd(), m_Buffer, g_BufSize);

		if(m_BufUsed >= g_BufSize) 
		{
			std::cerr << "Error buffer overflow\n";
			exit(EXIT_FAILURE);
		}
		else if(m_BufUsed == 0)
		{
			std::cout << "Programm terminated!\n";
			exit(EXIT_FAILURE);
		}
		else if(strstr(m_Buffer, g_QuitMsg))
		{
			exit(0);
		}

		for(int i=0; i < m_BufUsed; i++)
		{
			if(m_Buffer[i] == '\n')
			{
				int res = send(m_pTheMaster->GetFd(), m_Buffer, i+1, 0);
				if(res == -1)
				{
					std::cout << "Error sending message, server doesn't respond:(\n";
					return;
				}
				Memove(m_Buffer, m_Buffer+i+1, m_BufUsed);
				m_BufUsed -= i+1;
				i = -1;
			}
		}
		
	}
};
