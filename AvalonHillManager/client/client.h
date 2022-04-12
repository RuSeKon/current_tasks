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

static const char g_QuitMsg[] {"quit"};
static const char g_HelpMsgFlag[] {"helpMe"};
static const char g_HelpMsg[] {""
"\n		Used commands:		\n\n"
"market - recieving the info about current market condition"
								" (without parametrs);\n\n"
"player - recieving the info about another player"
								" (one parametr - number of player);\n\n"
"buy - submitting an application for the purchase of raw materials"
" (two parameters - quantity and cost);\n\n"
"sell - submitting an application for the sale of products"
" (two parameters - quantity and cost);\n\n"
"build - construction of a new factory (without parametrs);\n\n"
"turn - confirmation of readiness for the auction"
								" (without parametrs);\n\n"
"help - information about available commands (without parametrs);\n\nENTER: "};

enum SizeOfStrings
{
	g_QuitMsgSize = 4,
	g_HelpMsgFlagSize = 6,
	g_HelpMsgSize = 594,

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
