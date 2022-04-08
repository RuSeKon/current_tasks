#include <utility>
#include "game.h"

Request::Request(std::string in) : m_Text(in)
{
    m_Params.reserve(g_MaxParams);
}
/*
Request& Request::operator=(Request src)
{
    std::swap(src);
    return *this;
}
*/
Request& Request::operator=(Request&& src)
{
    m_Text = std::move(src.m_Text);
    m_Params = std::move(src.m_Params);
    return *this;
}

std::string& Request::GetText() const
{
    return m_Text;
}

void Request::AddParam(int i)
{
    m_Params.push_back(i);
}