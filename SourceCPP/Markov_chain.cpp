#include <iostream>
#include <vector>
#include <map>
#include <deque>

enum {
    MAXGEN = 1000,
    NPREF = 2,

};

std::string NONWORD = {"\n"};

typedef std::deque<std::string> Prefix;
std::map<Prefix, std::vector<std::string>> statetab; //Maybe unordered map instead map

void add(Prefix&, const std::string&);
void build(Prefix&, std::istream&);
void generate(int);

int main()
{
    int nwords = MAXGEN;
    Prefix prefix;

    for(int i = 0; i < NPREF; i++)
        add(prefix, NONWORD);
    build(prefix, std::cin);
    add(prefix, NONWORD);
    generate(nwords);
    return 0;
}

void build(Prefix& prefix, std::istream& in)
{
    std::string buf;
    
    while(in >> buf)
        add(prefix, buf);
}

void add(Prefix& prefix, const std::string& s)
{
    if(prefix.size() == NPREF) {
        statetab[prefix].push_back(s);
        prefix.pop_front();
    }
    prefix.push_back(s);
}

void generate(int nwords)
{
    Prefix prefix;
    int i;

    for(i = 0; i < NPREF; i++)
        add(prefix, NONWORD);

    for(i = 0; i < nwords; i++) {
        std::vector<std::string>& suf = statetab[prefix];
        const std::string& w = suf[rand() % suf.size()];
        if(w == NONWORD)
            break;
        std::cout << w << std::endl;
        prefix.pop_front();
        prefix.push_back(w);
    }
} 
