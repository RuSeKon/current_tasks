/* Implementation of Gamer class */

#include <cctype>
#include <cstring>
#include <memory>
#include "server.h"
#include "banker.h"
#include "game.h"

Manager::Manager(Game* b, GameSession* s, int num) : Player(b, s, num), 
					m_Name(nullptr),m_Material(4), m_Products(2), 
					m_Money(10000), m_End(false)
{}


