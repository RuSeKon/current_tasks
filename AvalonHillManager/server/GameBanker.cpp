/* Implementation of Banker class */

GameBanker::GameBanker() : ////

GameBanker::~GameBanker()
{
	for(auto x : m_pList)
		delete x;
}

void GameBanker::GamerAdd(GameSession* h)
{
	Game* tmp = new Game(this, h);
	m_pList.push_bakc(tmp);
}
