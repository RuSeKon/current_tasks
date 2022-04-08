#include <utility>
#include "game.h"

Request::Request(std::string in) : m_Text(in)
{
    m_Params.reserve(g_MaxParams);
}

Request::Request(Request&& src)
{
	m_Text = std::move(src.m_Text);
    m_Params = std::move(src.m_Params);
}

Request::Request(const Request& src)
{
	m_Text = src.m_Text;
	m_Params = src.m_Params;
}

const std::string& Request::GetText() const
{
    return m_Text;
}

void Request::AddParam(int i)
{
    m_Params.push_back(i);
}
