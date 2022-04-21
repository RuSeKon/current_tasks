/* some test for tree_tools */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "tree.h"

#ifndef NAMENUM 
#define NAMENUM 100
#endif

Nameval *form_tree(char **, int);
int create_name(char **, int);

main(int argc, char **argv)
{
	int indx;
	if(argc > 1 && isdigit(argv[1][1]))
	{
		if((indx = atoi(argv[1])) > NAMENUM)
		{
			printf("Argument %s is too more\n", argv[1]);
			return -1;
		}
	}
	else
		indx = NAMENUM/2; 

	char *name[NAMENUM];
	int len;
	
	srand(time(0));
	if(!(len = create_name(name, 0))) //zero is a non mode(standart) creation
	{
		printf("Cannot create initial name\n");
		return -1;
	}

	char *point = name[indx];

	Nameval *tmp;
	Nameval *res;
	clock_t begin, end;
	double time_res; 
	//form initial tree
	tmp = form_tree(name, len);

	//time test for lookup functions
	begin = clock();
	if((res = nvlookup(tmp, point)) == NULL)
	{
		printf("Cannot find name (%s) on tree\n", point);
		return -1;
	}
	end = clock();
	
	//calculate and output results
	time_res = (double)(end-begin)/ CLOCKS_PER_SEC;
	printf("Search function: nvlookup()\n"
	"Searched leafs value: %d\n"
	"Tree size: %d\n" 
	"Spended time for lookup is %.8f\n"
	,res->value, len, time_res);
	return 0;
}


Nameval *form_tree(char **src, int len)
{
	Nameval *tmp, *root = NULL;
	while(len > 0)
	{
		tmp = (Nameval *)malloc(sizeof(Nameval));
		if(tmp == NULL)
			exit(EXIT_FAILURE);
		tmp->name = (char *)malloc(strlen(*src)+1);
		tmp->value = (NAMENUM+1)-len;
		strcpy(tmp->name, *src);
		root = insert(root, tmp);
		src++;
	    len--;
	}
	return root;
}	


#define MAXNAME 6


int create_name(char **src, int mod)
{
	int i;
	int rnd;
	char *tmp;
	for(i = 0; i < NAMENUM; i++) {
		if((tmp = (char *)malloc(MAXNAME+1)) == NULL) {
			perror(tmp);
			return -1;
		}
		for(int b=0; b < MAXNAME; b++)
		{
			rnd += 4;
			while(!isalpha(rnd))
			{
				rnd = rand()%123;
				if(rnd < 64)
					rnd += 58;
			}
			tmp[b] = rnd;
		}
		tmp[MAXNAME] = '\0';
		src[i] = tmp;
	}
	return i;
}
	

			







