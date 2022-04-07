#include <utility>
#include "game.h"

Player::Player(Game* b, GameSession* s, int num) : m_pGame(b),
										m_pSession(s), 
										m_PlayerNumber(num) 
{}

Player::~Player()
{
    m_pSession->Delete();
}

void Player::Send(const char* message) 
{
    m_pSession->Send(message);
}	

Request& Request::operator=(Request src)
{
    std::swap(src);
    return *this;
}

Request& Request::operator=(Request&& src)
{
    m_Text = std::move(src.m_Text);
    m_Params = std::move(src.m_Params);
    return *this;
}

Request::Request(Request&& src) : m_Text(src.m_Text),
                                  m_Params(src.m_Params)
{}