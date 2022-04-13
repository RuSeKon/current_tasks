#include <utility>
#include <cstring>
#include <iostream>
#include "game.h"

Request::Request(const char* in) : m_Set(0)
{
	std::cout << "Request ctr!\n";
	m_pText = new char[strlen(in)+1];
	strcpy(m_pText, in);
	m_pParams = new int[g_MaxParams]{0};
}

void Request::swap(Request& src)
{
	std::swap(m_pText, src.m_pText);
	std::swap(m_pParams, src.m_pParams);
}

Request::~Request()
{
	std::cout << "Destructor Request!\n";
	delete[] m_pText;
	delete[] m_pParams;
}

Request::Request(Request&& src)
{
	std::cout << "Move constructor!\n";
	m_pText = src.m_pText;
	src.m_pText = nullptr;

	m_pParams = src.m_pParams;
	src.m_pParams = nullptr;

	m_Set = src.m_Set;
	src.m_Set = 0;
}

Request::Request(const Request& src)
{
	std::cout << "Copy constructor!\n";
	size_t len = strlen(src.m_pText)+1;
	m_pText = new char[len];
	size_t i;
	for(i=0; i < len; i++)
		m_pText[i] = src.m_pText[i];
	m_Set = src.m_Set;
	m_pParams = new int[g_MaxParams];
	for(i=0; i < m_Set; i++)
		m_pParams[i] = src.m_pParams[i];
}

Request& Request::operator=(Request in)
{
	std::cout << "Assignment operator!\n";
	swap(in);
	m_Set = in.m_Set;
	return *this;
}

Request& Request::operator=(Request&& in)
{
	std::cout << "Move assignment operator!\n";
	if(&in == this)
		return *this;
	delete[] m_pText;
	m_pText = in.m_pText;
	in.m_pText = nullptr;
	m_pParams = in.m_pParams;
	in.m_pParams = nullptr;
	m_Set = in.m_Set;
	return *this;
}

const char* Request::GetText() const
{
    return m_pText;
}

void Request::AddParam(int i)
{
	if(m_Set >= g_MaxParams || i < 0)
		return;//maybe need exception
    else
		m_pParams[m_Set++] = i;
}

int Request::GetParam(int num) const
{
	if(num > g_MaxParams || num < 1)
		return 0;
	else
		return m_pParams[num-1];
}
