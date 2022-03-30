/* Implementation */
#include "markov.h"
#include <string>

/////////////////////////////  Prefix  //////////////////////////////
Prefix::Prefix(std::string& src)
{
    for(int i=0; i < NPREF; i++)
        m_Container[i] = src;
}

Prefix::Prefix(const Prefix& src)
{
    for(int i=0; i < NPREF; i++)
        m_Container[i] = src.m_Container[i];
}

bool Prefix::operator==(const Prefix& src) const
{
    for(int i=0; i < NPREF; i++)
        if(m_Container[i] != src.m_Container[i])
            return false;
    return true;
}

const std::string& Prefix::operator[](int indx) const
{
    if(indx >= NPREF || indx < 0)
        throw std::out_of_range("Invalid index in Prefix");
    return m_Container[indx];
}

void Prefix::push_back(const std::string& src)
{
    for(int i=0; i < NPREF-1; i++)
        m_Container[i] = std::move(m_Container[i+1]);
    m_Container[NPREF-1] = src;
}

/////////////////////////////  Point  ///////////////////////////////

Point::~Point()
{
    Suffix* tmp;
    while(m_pSuffix)
    {
        tmp = m_pSuffix;
        m_pSuffix = m_pSuffix->next;
        delete tmp;
    }
}
        
void Point::AddSuffix(const std::string& src)
{
    Suffix *tmp = new Suffix;
    tmp->m_Word = src;
    tmp->next = m_pSuffix;
    m_pSuffix = tmp;
    m_SufCounter++;
}

const std::string& Point::GetSuf(int indx) const
{
    if(indx > m_SufCounter || indx < 0)
        throw std::out_of_range("Invalid index in Suffix");
    
    Suffix* tmp = m_pSuffix; 
    for(int i=1; i < indx; i++)
       tmp = tmp->next;
    return tmp->m_Word;
}

/////////////////////////////  Tab  /////////////////////////////////

unsigned Tab::Hash(const Prefix& src)
{
    unsigned res{0};

    for(int i=0; i < NPREF; i++)
        for(auto x : src[i])
            res = MULTIPLIER * res + x; //MULTIPLIER from Kernighan's book    
    return res % TABSIZE;               // for hashing string key
}

Point* Tab::Lookup(const Prefix& src)
{
    Point* tmp;

    unsigned h = Hash(src);
    for(tmp = m_Table[h]; tmp; tmp = tmp->next)
        if(tmp->GetPref() == src)
            return tmp;
    tmp = new Point(src);
    tmp->next = m_Table[h];
    m_Table[h] = tmp;
    return tmp;
}

Point& Tab::operator[](const Prefix& indx)
{
    return *(Lookup(indx));
}
