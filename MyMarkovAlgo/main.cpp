#include "markov.hpp"

int main()
{
    HashTable table;
    Prefix pref;
    for(int i = 0; i < PREFSZ; i++)
        pref.push_back("\n");
    build(table, std::cin);
    //terminate
    generate(HashTable);
    return 0;
}

void build(HashTable tab, std::istream& in)
{
    std::string buf;

    while(in >> buf)
        tab.add(prefix, buf);
}
    
