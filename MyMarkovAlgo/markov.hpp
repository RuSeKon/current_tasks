#ifndef MARKOVSCHAINHPP
#define MARKOVSCHAINHPP


#ifndef HASHSIZE
#define HASHSIZE 4096
#endif


#ifndef PREFSZ 
#define PREFSZ 2
#endif

using std::string;

////////////////////PREFIX//////////////////////
class Prefix {
    string arr[];
    int size;
public:
    Prefix() = delete;
    Prefix(int sz) : size(sz) {arr = new string[size];}

    ~Prefix() {delete[] arr;}

    Prefix(Prefix const& src);
    Prefix(Prefix&& src) = delete;

    Prefix& operator=(Prefix& src);
    Prefix& operator=(Prefix&& src) = delete;

    string& operator[](int index);
    void push_back(string suf); //with erase last element of arr 
};

///////////////////////SUFFIX////////////////////////////////

struct ListStr {
    string suf;
    ListStr *next;
};

class Suffix {
    ListStr *node;
public:
    Suffix() = delete;
    Suffix(Suffix* s) : node(s) {}

    Suffix operator=(Suffix& src);
    Suffix operator+=(string& src);


};

/////////////////////////STATE/////////////////////////
class State {
    Prefix pref;
    Suffix suf;
    State *next;
public:
    State() : pref(PREFSZ), next(nullptr) {}
    void addsuffix(string suf);
}

////////////////////////////HASHTABLE///////////////////
class HashTable {
   State *hash_table[];
   int size;
public:
    HashTable() = delete;
    ~HashTable();
    HashTable(int sze) : size(sze) {hashtable = new State[size];}
    
    HashTable *operator[](const int index);
    
    Prefix& lookup(Prefix& pref, int create);
    int HashFunc(Prefix& pref, int size);
    void add(Prefix& pref, string suf);
}

#endif

