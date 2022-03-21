#include "markov.h"
#include <iostream>



std::string NONWORD{"\0"};

Tab statetab;

#if DEBUG==2 
void print_point(const Prefix& src)
{
    std::cout << "///////POINT///////" << std::endl;
    for(int i=0; i < NPREF; i++)
        std::cout << (statetab[src].getPref())[i] << std::endl;
    std::cout << "FIRST_SUFFIX: " << statetab[src].getSuf(0) << std::endl;
    std::cout << "SUFCOUNT: " << statetab[src].sufSize() << std::endl;
}
#endif

void build(Prefix&, std::istream&);
void add(Prefix&, std::string&);
void generate(int);

int main()
{

    Prefix initPrefix{NONWORD};

    build(initPrefix, std::cin);
    add(initPrefix, NONWORD);
#if DEBUG
    std::cout<<"Creation complete!\n";
#endif
    generate(MAXGEN);
    std::cout << std::endl;
    return 0;
}

void add(Prefix&, std::string&);

void build(Prefix& pref, std::istream& is)
{
    std::string buf;

    while(is >> buf)
        add(pref, buf);
}

void add(Prefix& pref, std::string& suf)
{
    statetab[pref].add_suffix(suf);
#if DEBUG==2 
    print_point(statetab[pref].getPref());
#endif
    pref.push_back(suf);
}

void generate(int maxgen)
{
    Prefix pref{NONWORD};
    int i{0};
    for(; i < maxgen; i++) {
        Point& tmp = statetab[pref];
        int h = rand() % tmp.sufSize();

        const std::string& s = tmp.getSuf(h);
        if(s == NONWORD) {
#if DEBUG
            std::cout << std::endl << "--- i ---: " << i << std::endl;
#endif
            return;
        }
        std::cout << s << " "; //NEED ATENTION
        pref.push_back(s);
    }
#ifdef DEBUG
    std::cout << std::endl << "--- i ---: " << i << std::endl;
#endif 
}
