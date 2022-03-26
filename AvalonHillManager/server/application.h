#ifndef SERVER_SENTRY
#define SERVER_SENTRY

class FdHandler {
    int fd;
public:
    FdHandler(int a_fd) : fd(a_fd) {}
    virtual ~FdHandler();
    
    virtual void Process(bool r, bool w) = 0;
    int GetFd() const {return fd;}

    virtual bool WantRead() const {return true;}
    virtual bool WantWrite() const {return false;}
};

class EventSelector {
    FdHandler **fd_array;
    int fd_array_len;
    int max_fd;
    bool quit_flag;
public:
    EventSelector() : fd_array(nullptr), quit_flag(false) {}
    ~EventSelector();

    void Add(FdHandler *h);
    bool Remove(FdHandler *h);

    void BreakLoop() { quit_flag = true; }
    void Run();
};

#endif

