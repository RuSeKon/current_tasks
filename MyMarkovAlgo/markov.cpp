#include "markov.hpp"

using std::string;

/////////////////////////////////////////////////
Prefix::Prefix(Prefix const& src)
        : size(src.size)
{
    arr = new string[size];
    for(int i = 0; i < size; i++)
        arr[i] = src.arr[i];
}

Prefix& operator=(Prefix& src)
{
    for(int i = 0; i < size; i++) {
        if(i < src.size) {
            arr[i] = src.arr[i];
        } else
            delete arr[i];
    }
    return this;
}

string& Prefix::operator[](int index)
{
    if(index >= size)
        return arr+(size-1); //return last element
    return arr+index; 
}

void Prefix::push_back(string suf)
{
    delete arr[0];

    for(int i=0; i < size-1; i++)
        arr[i] = std::move(arr[i+1]);
    arr[size-1] = suf;
}


/////////////////////////////////////////////////

Suffix::~Suffix()
{




/////////////////////////////////////////////



void HashTable::add(Prefix& pref, string suf)
{
    State *p;

    p = lookup(pref, 1);
    p->addsuffix(suf);
    pref.push_back(suf);
}


