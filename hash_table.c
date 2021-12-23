#include <stdlib.h>
#include <string.h>

#define HASHSIZE 101

struct nlist {
	struct nlist *next;
	char *name;
	char *defn;
};

static struct nlist *hashtab[HASHSIZE];



main()
{}








unsigned hash(char *s)
{
	unsigned hashval;

	for(hashval = 0; *s != '\0'; s++)
		hashval = *s + 31 *hashval;
	return hashval % HASHSIZE;
}

struct nlist *lookup(char *s, int del)
{
	struct nlist *np;
	for(np = hashtab[hash(s)]; np != NULL; np = np->next) {
		if(strcmp(s, np->name) == 0) {
			if(del)
				del_node(s); //lookup may not check del_node
			else		     //return value (look impl of del_node)
				return np;
		}
	}
	return NULL;
}


struct nlist *install(char *name, char *defn, int del)
{
	struct nlist *np;
	unsigned hashval;

	if((np = lookup(name)) == NULL) {
		np = (struct nlist *)malloc(sizeof(*np));
		if(np == NULL || (np->name = strdup(name)) == NULL)
			return NULL;
		hashval = hash(name);
		np->next = hashtab[hashval];
		hashtab[hashval] = np;
	} else 
		free((void *) np->defn);
	if((np->defn = strdup(defn)) == NULL)
		return NULL;
	return np;
}

int del_node(char *nm)		
{
	struct nlist *np, *tmp;

	/* search node with field.name = nm in hashtable */
	for(np = hashtable[hash(nm)]; np != NULL; np = np->next) { 
		if(np->next == NULL && (strcmp(np->name, nm) == 0)){ //if np is ones or last 
			free((void*)np);			    //in list, and .name = nm
			return 0;
		} else if(strcmp(np->next->name, nm) == 0) { 	    //if .name = nm,
			tmp = np->next;				    //and np not last or ones
			np->next = tmp->next ? tmp->next : NULL;
			return 0;
		} else 
			return 1;	//nm is missing in hashtable	
	}
}
			
			
