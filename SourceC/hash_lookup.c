#include <stdlib.h>
#include <string.h>

#define HASHSZE 100


typedef struct node node;

struct node {
    char *name;
    int value;
    node *next;
};

node *table[HASHSZE];


int hash(char *);

node *lookup(char *name, int create, int value)
{
    node *tmp;
    int n;
    
    n = hash(name);
    for(tmp = table[n]; tmp != NULL; tmp = tmp->next)
        if(strcmp(name, tmp->name) == 0)
            return tmp;
    if(create) {                //Inserting to start of list!!!!!
        tmp = (node *)malloc(sizeof(node));
        tmp->value = value;
        tmp->name = strdup(name);
        tmp->next = table[n];
        table[n] = tmp;
    }
    return tmp;
}




