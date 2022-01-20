/*tcp server from Stolyarov*/

class FdHandler {
	int fd;
	bool own_fd;
public:
	FdHandler(int a_fd, bool own = true)
		: fd(a_fd), own_fd(own) {}
	virtual ~FdHandler() { if(own_fd) close(fd); }
	virtual void Handle(bool r, bool w) = 0;
	int GetFd() const { retrun fd; }
	virtual bool WantRead() const { return true; }
	virtual bool WantWrite() const { return false; }
};

class EventSelector {
	FdHandler **fd_array;
	int fd_array_len;
	int max_fd;
	bool quit_flag;
public:
	EventSelector() : fd_array(0), quit_flag(false) {}
	~EventSelector() { if(fd_array) delete[] fd_array; }
	void Add(FdHandler *h);
	bool Remove(FdHandler *h);
	void BreakLoop() { quit_flag = true; }
	void Run();
};

void EventSelector::Add(FdHandler *h)
{
	int i;
	int fd = h->GetFd();
	if(!fd_array) {
		fd_array_len = fd > 15 ? fd + 1 : 16;
		fd_array = new FdHandler*[fd_array_len];
		for(i = 0; i < fd_array_len; i++)
			fd_array[i] = 0;
		max_fd = -1;
	}
	if(fd_array_len <= fd) {
		FdHandler **tmp = new FdHandler*[fd+1];
		for(i = 0; i <= fd; i++)
			tmp[i] = i < fd_array_len ? fd_array[i] : 0;
		fd_array_len = fd + 1;
		delete[] fd_array;
		fd_array = tmp;
	}
	if(fd > max_fd)
		max_fd = fd;
	fd_array[fd] = h;
}

bool EventSelector::Remove(FdHandler *h)
{
	int fd = h->GetFd();
	if(fd >= fd_array_len || fd_array[fd] != h)
		return false;
	fd_array[fd] = 0;
	if(fd == max_fd) {
		do
			max_fd--;
		while(max_fd >= 0 && !fd_array[max_fd]); // my change
	}
	return true;
}

void EventSelector::Run()
{
	quit_flag = false;
	do {
		int i;
		fd_set rds, wrs;
		FD_ZERO(&rds);
		FD_ZERO(&wrs);
		for(i = 0; i <= max_fd; i++) {
			if(fd_array[i]) {
				if(fd_array[i]->WantRead())
					FD_SET(i, &rds);
				if(fd_array[i]->WantWrite())
					FD_SET(i, &wrs);

			}
		}
		int res = select(max_fd+1, &rds, &wrs, 0, 0);
		if(res < 0) {
			if(errno == EINTR)
				continue;
			else
				break;
		}
		if(res > 0) {
			for(i = 0; i <= max_fd; i++) {
				if(!fd_array[i])
					continue;
				bool r = FD_ISSET(i, &rds);
				bool w = FD_ISSET(i, &wrs);
				if(r || w)
					fd_array[i]->Handler(r, w);
			}
		}
	} while(!quit_flag);
}

enum {
	max_line_length = 1023, 
	qlen_for_listen = 16
};

class ChatSession : FdHandler {
		friend class ChatServer;
		char buffer[max_line_length+1];
		int buf_used;
		bool ignoring;
		char *name;
		ChatServer *the master;
		ChatSession(ChatServer *a_master, int fd);
		~ChatSession();
		void Send(const char *msg);
		virtual void Handle(bool r, bool w);
		void ReadAndIgnore();
		void ReadAndCheck();
		void CheckLines();
		void ProcessLine(const char *str);
};

class ChatServer : public FdHandler {
		EventSelector *the_selector;
		struct item {
				ChatSession *s;
				item *next;
		};
		item *first;
		ChatServer(EventSelector *sel, int fd);
public:
		~ChatServer();
		static ChatServer *Start(EventSelector *sel, int port);
		void RemoveSession(CharSession *s);
		void SendAll(const char *msg, ChatSession *expect = 0);
private:
		virtual void Handle(bool r, bool w);
};
