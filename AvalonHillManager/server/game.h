#ifndef GAMEHPPSENTRY
#define GAMEHPPSENTRY

#include "server.h"

class Game;
class Request;

class  Player 
{
	friend class Game;
	Game* m_pGame;
	GameSession* m_pSession;

	int m_PlayerNumber;

public:
	Player(Game* b, GameSession* s, int num);
	virtual ~Player() noexcept;

	int PlayerNumber() const {return m_PlayerNumber;}
	void Send(const char* message);
	virtual Request VGetRequest() = 0;
};

class Game
{
	bool m_GameBegun;

public:

	Game() : m_GameBegun(false) {}
	virtual ~Game() noexcept;

	bool GameBegun() const {return m_GameBegun;}
	virtual void VPlayerAdd(GameSession* h) = 0;
	virtual void VProcess(GameSession* h) = 0;
	void RequestProc(Request& req);
};

class Request
{
	std::string m_Text;
	std::vector<int> m_Params;
public:
	Request(const char* s) : m_Text(s) {m_Params.reserve(3);}
	Request() : m_Text(nullptr) {m_Params.reserve(3);}
	~Request();

	Request& operator=(Request src) = delete;
	Request& operator=(Request&& src);
	Request(Request& src) = delete;
	Request(Request&& src);
	void AddParam(int p) {m_Params.push_back(p);}
	int GetParam(int n) const {return m_Params[n];}
	const std::string& GetText() const {return m_Text;}
}
#endif
