#ifndef GAMEHPPSENTRY
#define GAMEHPPSENTRY

#include <vector>
#include "application.h"

class Game;
class IFdHandler;

class  Player 
{
	Game* m_pGame;
	IFdHandler* m_pApplication;

	int m_PlayerNumber;

	Player(Game* b, IFdHandler* s, int num);
	virtual ~Player() noexcept;
public:
	int PlayerNumber() const;
	virtual void VRequest() = 0;
	void Send(const char* message);
};

class Game
{

	std::vector<Player*> m_pList;

	bool m_GameBegun;

public:

	Game() : m_GameBegun(false) {}
	virtual ~Game() noexcept;

	bool GameBegun() const {return m_GameBegun;}
	virtual void VPlayerAdd(IFdHandler* h) = 0;
	virtual void VProcess(int fd) = 0;
	virtual void VCircle() = 0;
};

#endif
