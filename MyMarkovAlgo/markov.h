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
    std::string m_Container[NPREF];
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
    std::string m_Word;
    Suffix* next;
};

class Point{
friend class Tab;
    Prefix m_Prefix;
    Suffix* m_pSuffix;
    int m_SufCounter;
    Point* next;
public:
    Point() = delete;
    ~Point(); 
    Point(const Prefix& src) : m_Prefix(src), m_pSuffix(nullptr),
                               m_SufCounter(0),  next(nullptr) {}
    int SufSize() const {return m_SufCounter;}
    inline const Prefix& GetPref() const {return m_Prefix;}
    const std::string& GetSuf(int indx) const; 
    void AddSuffix(const std::string& src);
};

class Tab{
    Point* m_Table[TABSIZE];
public:
    Point& operator[](const Prefix& indx);
private:
    unsigned Hash(const Prefix& src);
    Point* Lookup(const Prefix& src);
};

#endif
