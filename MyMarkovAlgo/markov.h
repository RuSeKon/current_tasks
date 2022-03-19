#ifndef MARKOVCHAINSENTRY
#define MARKOVCHAINSENTRY

#include <string>
#include <stdexcept>

#ifndef MAXGEN
#define MAXGEN 1000
#endif

#ifndef NPREF
#define NPREF 2
#endif

#ifndef TABSIZE
#define TABSIZE 4096
#endif

#define MULTIPLIER 7

class Prefix{
    std::string container[NPREF];
public:
    Prefix() = delete;
    Prefix(std::string& src);
    Prefix(const Prefix& src);
    const std::string& operator[](int indx) const;
    bool operator==(const Prefix& src) const;

    void push_back(const std::string& src);
};

class Suffix{
friend class Point;
    std::string word;
    Suffix* next;
};

class Point{
friend class Tab;
    Prefix pref;
    Suffix* suf;
    int sufCount;
    Point* next;
public:
    Point() = delete;
    ~Point(); 
    Point(const Prefix& src) : pref(src), suf(nullptr),
                               sufCount(0),  next(nullptr) {}
    int sufSize() const {return sufCount;}
    inline const Prefix& getPref() const {return pref;}
    const std::string& getSuf(int indx) const; 
    void add_suffix(const std::string& src);
};

class Tab{
    Point* table[TABSIZE];
public:
    Point& operator[](const Prefix& indx);
private:
    unsigned hash(const Prefix& src);
    Point* lookup(const Prefix& src);
};

#endif
