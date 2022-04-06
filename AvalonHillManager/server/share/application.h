#ifndef SERVER_SENTRY
#define SERVER_SENTRY

class IFdHandler {
	int m_Fd;
public: 
	IFdHandler(int a_fd) : m_Fd(a_fd) {} 
	virtual ~IFdHandler() noexcept {close(m_Fd);}
    
	virtual void VProcessing(bool r, bool w) = 0;
	int GetFd() const {return m_Fd;}

	virtual bool WantRead() const {return true;}
	virtual bool WantWrite() const {return false;}
};

class EventSelector {
	GameLogic* m_pGame;
	IFdHandler **m_pFdArray;
	int m_ArrayLength;
	int m_MaxFd;
	bool m_QuitFlag;
public:
	EventSelector(GameLogic* src) : m_pGame(src), m_pFdArray(nullptr),
								    m_QuitFlag(false) {}
	~EventSelector();

	void Add(IFdHandler *h, int to_game);
	bool Remove(IFdHandler *h);

	void BreakLoop() { m_QuitFlag = true; }
	void Run();
};

#endif

