#include "markov.h"
#include <iostream>

std::string NONWORD{"\0"};

Tab g_Statetab;

#if DEBUG >= 2 
void print_point(const Prefix& src)
{
    std::cout << "///////POINT///////" << std::endl;
    for(int i=0; i < NPREF; i++)
        std::cout << (g_Statetab[src].GetPref())[i] << std::endl;
    std::cout << "FIRST_SUFFIX: " << g_Statetab[src].GetSuf(0) << std::endl;
    std::cout << "SUFCOUNT: " << g_Statetab[src].SufSize() << std::endl;
}
#endif

void Build(Prefix&, std::istream&);
void Add(Prefix&, std::string&);
void Generate(int);

int main()
{

    Prefix initPrefix{NONWORD};
    try {
    Build(initPrefix, std::cin);
    Add(initPrefix, NONWORD);
#if DEBUG
    std::cout<<"Creation complete!\n";
#endif
    Generate(MAXGEN);
    }
    catch(const std::out_of_range& a)
    {
        std::cerr << "OUT_OF_RANGE: " << a.what() << std::endl;
    }
    return 0;
}

void Add(Prefix&, std::string&);

void Build(Prefix& pref, std::istream& is)
{
    std::string buf;

    while(is >> buf)
        Add(pref, buf);
}

void Add(Prefix& pref, std::string& suf)
{
    g_Statetab[pref].AddSuffix(suf);
#if DEBUG >= 2 
    print_point(g_Statetab[pref].GetPref());
#endif
    pref.push_back(suf);
}

void Generate(int maxgen)
{
    Prefix pref{NONWORD};
    int i{0};
    for(; i < maxgen; i++) {
        Point& tmp = g_Statetab[pref];
        int h = rand() % tmp.SufSize();

        const std::string& s = tmp.GetSuf(h);
        if(s == NONWORD) {
#if DEBUG
            std::cout << std::endl << "--- i ---: " << i << std::endl;
#endif
            std::cout << std::endl;
            return;
        }
        std::cout << s << " "; //NEED ATENTION
        pref.push_back(s);
    }
    std::cout << std::endl;
#ifdef DEBUG
    std::cout << std::endl << "--- i ---: " << i << std::endl;
#endif 
}
