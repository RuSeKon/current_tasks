/*Implementation of game logic layer*/
#include "share/game.h"

Player::Player(Game* b, FdHandler* s, num) : m_pGame(b),
										m_pApplication(s), 
										m_PlayerNumber(num)
{}

Player::~Player()
{
	delete m_pApplication;
}

int Player::PlayerNumber() const
{
	return m_PlayerNumber;
}

Game::~Game
{
	for(auto x : m_pList)
		delete x;
}
