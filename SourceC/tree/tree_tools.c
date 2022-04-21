/* some function for work with binary tree */

#include <string.h>
#include <stdio.h>

#ifndef QUIET
#define QUIET 0
#endif


typedef struct Nameval Nameval;
struct Nameval {
	char *name;
	int value;
	Nameval *left;
	Nameval *right;
};


Nameval *insert(Nameval *root, Nameval *newp)
{
	int cmp;
	if(root == NULL)
		return newp;
	cmp = strcmp(newp->name, root->name);
	if(cmp == 0) {
		if(!QUIET)
			printf("insert: duplicate entry %s ignored\n", newp->name);
	}
	else if(cmp < 0)
		root->left = insert(root->left, newp);
	else 
		root->right = insert(root->right, newp);
	return root;
}

Nameval *lookup(Nameval *root, char *name)
{
	int cmp;
	if(root == NULL)
		return NULL;
	cmp = strcmp(name, root->name);
	if(cmp == 0)
		return root;
	else if(cmp < 0)
		return lookup(root->left, name);
	else 
		return lookup(root->right, name);
}

Nameval *nvlookup(Nameval *root, char *name)
{	
	int cmp;
	while(root != NULL)
	{
		cmp = strcmp(name, root->name);
       	if(cmp == 0)
			return root;
		else if(cmp < 0)
			root = root->left;
		else 
			root = root->right;
	}
	return NULL;
}	
