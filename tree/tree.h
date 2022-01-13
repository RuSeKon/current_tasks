
#ifndef TREE_HEADER
#define TREE_HEADER


typedef struct Nameval Nameval;
struct Nameval {
	char *name;
	int value;
	Nameval *left;
	Nameval *right;
};

Nameval *insert(Nameval *, Nameval *);
Nameval *lookup(Nameval *, char *);
Nameval *nvlookup(Nameval *, char *);

#endif
