#include <stdio.h>
#include <string.h>
#include <stdlib.h>


enum {  NPREF = 2, 
        HASHSIZE = 400,
        MULTIPLIER = 7,
        MAXGEN = 1000 
};

char NONWORD[] = "\n";

typedef struct State State;
typedef struct Sufix Sufix;

struct State {
    char *pref[NPREF];
    Sufix *suf;
    State *next;
};

struct Sufix {
    char *word;
    Sufix *next;
};

State *statetab[HASHSIZE];

void build(char *[NPREF], FILE *);
void add(char *[NPREF], char *);
void generate(int);

main()
{
    int i, nwords = MAXGEN;
    char *prefix[NPREF];
    
    for(i = 0; i < NPREF; i++)
        prefix[i] = NONWORD;
    build(prefix, stdin);
    add(prefix, NONWORD);
    generate(nwords);
    return 0;
}


State *lookup(char *[NPREF], int);

void build(char *prefix[NPREF], FILE *f)
{
    char buf[100], fmt[10];

    sprintf(fmt, "%%%lds", sizeof(buf)-1);
    while(fscanf(f, fmt, buf) != EOF)
        add(prefix, strdup(buf)); //Error handle needed
}

void addsufix(State *, char *);

void add(char *prefix[NPREF], char *sufix)
{
    State *sp;

    sp = lookup(prefix, 1);
    addsufix(sp, sufix);
    memmove(prefix, prefix+1, (NPREF-1)*sizeof(prefix[0]));
    prefix[NPREF-1] = sufix;
}

void addsufix(State *sp, char *sufix)
{
    Sufix *suf;

    suf = (Sufix *)malloc(sizeof(Sufix));
    suf->word = sufix;
    suf->next = sp->suf;
    sp->suf = suf;
}

unsigned int hash(char *s[NPREF]);

State *lookup(char *prefix[NPREF], int create)
{
    int i, h;
    State *sp;

    h = hash(prefix);
    for(sp = statetab[h]; sp != NULL; sp = sp->next) {
        for(i = 0; i < NPREF; i++)
            if(strcmp(prefix[i], sp->pref[i]) != 0)
                break;
        if(i == NPREF)
            return sp;
    }
    if(create) {
        sp = (State *)malloc(sizeof(State));
        for(i = 0; i < NPREF; i++)
            sp->pref[i] = prefix[i];
        sp->suf = NULL;
        sp->next = statetab[h];
        statetab[h] = sp;
    }
    return sp;
}

unsigned int hash(char *s[NPREF])
{
    unsigned int h;
    unsigned char *p;
    int i;

    h = 0;
    for(i = 0; i < NPREF; i++)
        for(p = (unsigned char *)s[i]; *p != '\0'; p++)
            h = MULTIPLIER * h + *p;
    return h % HASHSIZE;
}

void generate(int nwords)
{
    State *sp;
    Sufix *suf;
    char *prefix[NPREF], *w;
    int i, nmatch;

    for(i = 0; i < NPREF; i++)
        prefix[i] = NONWORD;

    for(i = 0; i < nwords; i++)
    {
        sp = lookup(prefix, 0);
        nmatch = 0;
        for(suf = sp->suf; suf != NULL; suf = suf->next)
            if(rand() % ++nmatch == 0)
                w = suf->word;
        if(strcmp(w, NONWORD) == 0)
            break;
        printf("%s\n", w);
        memmove(prefix, prefix+1, (NPREF-1)*sizeof(prefix[0]));
        prefix[NPREF-1] = w;
    }
} 
