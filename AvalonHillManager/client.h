#ifndef CLIENT_SENTRY
#define CLIENT_SENTRY

#include <sys/socket.h>
#include <sys/types.h>
#include <tuple>
#include "application.h"

bool IsAdress(const char*);
std::tuple<std::string, std::string> Parse(int, char**);

enum constants { 
		g_BufSize = 256,
		g_ValidPort = 1000
};


enum SizeOfStrings
{
	g_QuitMsgSize = 4,
	g_HelpMsgFlagSize = 6,
	g_HelpMsgSize = 654,

};

class ServerForClient : public IFdHandler {
	char m_Buffer[256];
	int m_BufUsed;
	ServerForClient(int fd) : IFdHandler(fd), m_BufUsed(0) {};
public:
	void VProcessing(bool r, bool w) override;
	static ServerForClient* Start(std::string& adress, int port);
};

class Console : public IFdHandler {
	ServerForClient* m_pTheMaster;
	char m_Buffer[256];
	int m_BufUsed;
	Console(ServerForClient* master, int fd) : IFdHandler(fd), m_pTheMaster(master), m_BufUsed(0) {};
public:
	void VProcessing(bool r, bool w) override;
	static Console* Start(ServerForClient* master, int fd);
};

#endif
