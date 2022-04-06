/* low level server implementation. That mean work with
sockets and file descriptors directly */

#include <sys/select.h>
#include <cerrno>
#include <unistd.h>

#include "share/application.h"

EventSelector::~EventSelector()
{
	if(m_pFdArray)
		delete[] m_pFdArray;
}

void EventSelector::Add(IFdHandler *h, int to_game)
{
	int i;
	int fd = h->GetFd();
	if(!m_pFdArray) {
		m_ArrayLength = fd > 15 ? fd + 1 : 16;
		m_pFdArray = new IFdHandler*[m_ArrayLength];
		for(i = 0; i < m_ArrayLength; i++)
			m_pFdArray[i] = 0;
		m_MaxFd = -1;
	}
	if(m_ArrayLength <= fd) {
		IFdHandler **tmp = new IFdHandler*[fd+1];
		for(i = 0; i< fd; i++)
			tmp[i] =i < m_ArrayLength ? m_pFdArray[i] : 0;
		m_ArrayLength = fd+1;
		delete[] m_pFdArray;
		m_pFdArray = tmp;
	}
	if(fd > m_MaxFd)
		m_MaxFd = fd;
	m_pFdArray[fd] = h;

	if(to_game)
		m_pGame->VPlayerAdd(h);
}

bool EventSelector::Remove(IFdHandler *h)
{
	int fd = h->GetFd();
	if(fd >= m_ArrayLength || m_pFdArray[fd] != h)
		return false;
	m_pFdArray[fd] = nullptr;
	if(fd == m_MaxFd) {
		while(m_MaxFd >= 0 && !m_pFdArray[m_MaxFd])
			m_MaxFd--;
	}
	return true;
}

void EventSelector::Run()
{
	m_QuitFlag = false;
	do {
		int i;
		fd_set rds, wrs;
		FD_ZERO(&rds);
		FD_ZERO(&wrs);
		for(i=0; i < m_ArrayLength; i++){
			if(m_pFdArray[i])
			{
				if(m_pFdArray[i]->WantRead())
					FD_SET(i, &rds);
				if(m_pFdArray[i]->WantWrite())
					FD_SET(i, &wrs);
			}
		}
		int res = select(m_MaxFd+1, &rds, &wrs, 0, 0);
		if(res < 0) {
			if(errno == EINTR) //If we need to process input signals
				continue;
			else
				break; //Need to proceed!!!!!
		}
		if(res > 0) {
			for(i = 0; i < m_ArrayLength; i++) {
				if(!m_pFdArray[i])
					continue;
				bool r = FD_ISSET(i, &rds);
				bool w = FD_ISSET(i, &wrs);
				if(r || w)
				{
					m_pFdArray[i]->VProcessing(r, w);
					m_pGame->VProcess(m_pFdArray[i]->GetFd());//////
				}
			}
			m_pGame->VCircle(); ////////
		}
	} while(!m_QuitFlag);
}
