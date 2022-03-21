/* Implementation */
#include "markov.h"
#include <string>

/////////////////////////////  Prefix  //////////////////////////////
Prefix::Prefix(std::string& src)
{
    for(int i=0; i < NPREF; i++)
        container[i] = src;
}

Prefix::Prefix(const Prefix& src)
{
    for(int i=0; i < NPREF; i++)
        container[i] = src.container[i];
}

bool Prefix::operator==(const Prefix& src) const
{
    for(int i=0; i < NPREF; i++)
        if(container[i] != src.container[i])
            return false;
    return true;
}

const std::string& Prefix::operator[](int indx) const
{
    if(indx >= NPREF || indx < 0)
        throw std::out_of_range("Prefix"); /////NEED ATENTION
    return container[indx];
}

void Prefix::push_back(const std::string& src)
{
    for(int i=0; i < NPREF-1; i++)
        container[i] = std::move(container[i+1]);
    container[NPREF-1] = src;
}

/////////////////////////////  Point  ///////////////////////////////

Point::~Point()
{
    Suffix* tmp;
    while(suf)
    {
        tmp = suf;
        suf = suf->next;
        delete tmp;
    }
}
        
void Point::add_suffix(const std::string& src)
{
    Suffix *s = new Suffix;
    s->word = src;
    s->next = suf;
    suf = s;
    sufCount++;
}

const std::string& Point::getSuf(int indx) const
{
    if(indx > sufCount || indx < 0)
        throw std::out_of_range("GetSuf"); //NEED ATENTION
    
    Suffix* tmp = suf;
    for(int i=1; i < indx; i++)
       tmp = tmp->next;
    return tmp->word;
}

/////////////////////////////  Tab  /////////////////////////////////

unsigned Tab::hash(const Prefix& src)
{
    unsigned res{0};

    for(int i=0; i < NPREF; i++)
        for(auto x : src[i])
            res = MULTIPLIER * res + x; //MULTIPLIER from Kernighan's book    
    return res % TABSIZE;               // for hashing string key
}

Point* Tab::lookup(const Prefix& src)
{
    Point* tmp;

    unsigned h = hash(src);
    for(tmp = table[h]; tmp; tmp = tmp->next)
        if(tmp->getPref() == src)
            return tmp;
    tmp = new Point(src);
    tmp->next = table[h];
    table[h] = tmp;
    return tmp;
}

Point& Tab::operator[](const Prefix& indx)
{
    return *(lookup(indx));
}
