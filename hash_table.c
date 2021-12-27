#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define HASHSIZE 101
#define MAXNAME 100
#define MAXDEFN 100
#define ISDEF(c) (c == '#' && (c = getchar() == 'd'))
#define DEF 0

struct nlist {
	struct nlist *next;
	char *name;
	char *defn;
};

static struct nlist *hashtab[HASHSIZE];

int getchar(FILE *, char *, char *, int, int);
struct nlist *lookup(char *, int);

main(int argc, char **argv)
{	char tmp_name[MAXNAME];
	char tmp_defn[MAXDEFN];
	int res;
	FILE *fp;
	struct nlist *np;

	if(argc < 2)
		puts("Please, enter filename!\n");

	fp = fopen(argv[1], "r+");

	while((res = getchar(fp, tmp_name, tmp_defn, MAXNAME, MAXDEFN)) != EOF)
	    if(res == DEF)
	        if(install(tmp_name, tmp_defn, 0) == NULL) {
		    printf("Error of install definition on table\n");
		    return 1;
		}
	    else 
		if((np = lookup(tmp_name, 0)) == NULL)
			continue;
		lseek(fp, -res, SEEK_CUR);
		put(fp, np->name);



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

	if((np = lookup(name, del)) == NULL) {
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
			
int getchar(FILE *fp, char *name, char *defn, int max_nm, int max_def)
{
    int i;
    while((c = fgetc(fp)) != EOF) {

        if(//ISDEF(c)) {
	    while((c = fgetc(fp)) != ' ') //skip word /define/
		;

	    for(i = 0; i < max_nm && (c = getchar()) != ' '; i++)
		name[i] = c;
	    if(i >= max) //too much longer define name
		return EOF;

	    for(i = 0; i < max_def && (c = fgetc(fp)) != ' '
			    && c != '\n'; i++) //only one line define
		defn[i] = c; 
	    if(i >= max) //too much longer definition
		return EOF;
	return DEF;
	}

	if(isalpha(c)) {
	    for(i = 0;i < max_nm &&  
		(c = fgetc(fp)) != ' ' && c !=  '\n'; i++) {
		    if(isalpha(c))
			name[i] = c;
		    else if(c == '_')
			name[i] = c;
		    else {
			name[i] = '\0';
			return i;
		    }
	    }
	    name[i] = '\0';
	    return i;
	}
    }
    return EOF;
}

void put(
