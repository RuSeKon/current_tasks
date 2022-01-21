#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define HASHSIZE 101
#define MAXNAME 100
#define MAXDEFN 100
#define ISDEF(c) (c == '#' && (c = getchar() == 'd'))
#define DEF 0
#define SPACE 32

struct nlist {
	struct nlist *next;
	char *name;
	char *defn;
};

static struct nlist *hashtab[HASHSIZE];

int getchr(FILE *, FILE *, char *, char *, int, int);
struct nlist *lookup(char *, int);
struct nlist *install(char *, char *);

main(int argc, char **argv)
{
	char name[MAXNAME];
	char defn[MAXDEFN];
	int res;
	FILE *inp_f, *outp_f;
	struct nlist *np;

	if(argc < 2) {
		puts("Please, enter filename!\n");
		return 1;
	}
	inp_f = fopen(argv[1], "r");

	if(argc > 2 && isalpha(argv[2][0])) //name for processed file
		outp_f = fopen(argv[2], "w");
	else
		outp_f = fopen("processed.c", "w");

	while((res = getchr(outp_f, inp_f, name, defn, MAXNAME, MAXDEFN)) != EOF)
    	    if(res == DEF) {
	        if(install(name, defn) == NULL) {
		    printf("Error of install definition on table\n");
		    return 1;
		}
	    } else { 		//getchar returned name
		if((np = lookup(name, 0)) == NULL) { //if name is not in hashtable 
			fputs(name, outp_f);
			fputc(res, outp_f);	//put name to outfile without cast
			continue;
		}
	   fputs(np->defn, outp_f);	//if name is in table put define val
	   fputc(res, outp_f);
	   }
	fclose(inp_f);
	fclose(outp_f);
	return 0;
}



unsigned hash(char *s)
{
	unsigned hashval;

	for(hashval = 0; *s != '\0'; s++)
		hashval = *s + 31 *hashval;
	return hashval % HASHSIZE;
}

int del_node(char *);

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


struct nlist *install(char *name, char *defn)
{
	struct nlist *np;
	unsigned hashval;

	if((np = lookup(name, 0)) == NULL) {
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
    for(np = hashtab[hash(nm)]; np != NULL; np = np->next) { 
	if(np->next == NULL && 
			(strcmp(np->name, nm) == 0)){ //if np is ones or last 
	    free((void*)np);			      //in list, and .name = nm
	    return 0;
	} else if(strcmp(np->next->name, nm) == 0) { //if .name = nm,
	    tmp = np->next;			     //and np not last or ones
	    np->next = tmp->next ? tmp->next : NULL;
	    return 0;
	} else 
	    return 1;	//nm is missing in hashtable	
	}
    return -1;
}
			
int getchr(FILE *out, FILE *in,  char *nm, char *dfn, int max_nm, int max_dfn)
{
    int i;
    char c;
    while((c = fgetc(in)) != EOF) {
	if(c == '#') {
	     fputc(c, out);
	     while((c = fgetc(in)) != SPACE)
		fputc(c, out);
	     fputc(c, out);
	     for(i = 0; i < max_nm && (c=fgetc(in)) != SPACE; i++) {
		     fputc(c, out);
		     nm[i] = c;
	     }
	     fputc(c, out);
	     nm[i] = '\0';
	     for(i = 0; i < max_dfn && 
			     (c=fgetc(in)) != SPACE && c != '\n'; i++) {
		     fputc(c, out);
		     dfn[i] = c;
	     }
	     fputc(c, out);
	     dfn[i] = '\0';
	     return DEF;
	} else if(isalpha(c)) {
	     nm[0] = c;
	     for(i = 1; i < max_nm && (isalpha(c=fgetc(in)) || c == '_'); i++)
		nm[i] = c;
	     nm[i] = '\0';
	     return c;
	} else
	     fputc(c, out);
    }
    return EOF;
}
