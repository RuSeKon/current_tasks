/* print all words of file by sorted of count their meetings */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXWORD 100
#define STATEON 1
#define STATEOFF 0

#ifndef MNGSIZ
#define MNGSIZ 6
#endif

struct tnode {
	char *word;
	int count;
	struct tnode *left;
	struct tnode *right;
};

struct tnode *addtree(struct tnode *, char *);
/*void treeprint(struct tnode *); */
int getword(char *, int);
void sort_tree(struct tnode *);

main()
{
	struct tnode *root;
	char word[MAXWORD];

	root = NULL;
	while(getword(word, MAXWORD) != EOF)
		root = addtree(root, word);
	sort_tree(root);
	return 0;
}

struct tnode *talloc(void);
char *Strdup(char *);

struct tnode *addtree(struct tnode *p, char *w)
{
	int cond;
	if(p == NULL) {
		p = talloc();
		p->word = Strdup(w);
		p->count = 1;
		p->left = p->right = NULL;
	} else if((cond = strcmp(w, p->word)) == 0)
		p->count++;
	else if(cond < 0)
		p->left = addtree(p->left, w);
	else
		p->right = addtree(p->right, w);
	return p;
}

struct tnode *talloc(void)
{
	return (struct tnode*)malloc(sizeof(struct tnode));
}

char *Strdup(char *s)
{
	char *p;
	p = (char *)malloc(strlen(s)+1);
	if(p != NULL)
		strcpy(p, s);
	return p;
}

int getword(char *src, int max)
{
	int i=0, c;
	int state = STATEOFF;
	while(i < max && (c = getchar()) != EOF) {
		if((isalpha(c) || c == '_')) {
			state = STATEON;
			*src++ = c;
			i++;
		} else {
			if(state) {
				*src = '\0';
				return i;
			}
			continue;
		}
	}
	if(c == EOF)
		return c;
	return -1;
}


#define MAXARR 1000

struct tnode *tree_arr[MAXARR];
struct tnode **ptr = tree_arr;

void tree_to_array(struct tnode *);
void sort_arr(struct tnode *[], int);
void arr_print();


void sort_tree(struct tnode *p)
{
	tree_to_array(p);
	*ptr = NULL;	
	sort_arr(tree_arr,(ptr - tree_arr));
	arr_print();
	return;
}

void tree_to_array(struct tnode *p) //form string from tree tnode
{
	if(p != NULL) {			
		tree_to_array(p->left);
		if((ptr-tree_arr) < MAXARR)
			*ptr++ = p;
		else
			printf("Too much letters, "
				"have to extend size of tree or array\n");
		tree_to_array(p->right);
	}
}

void swap(struct tnode *[], int, int);

void sort_arr(struct tnode *arr[], int n)
{
	int i, last;
       	if(n == 0)
		return;
	swap(arr, 0, rand() % n);
	last = 0;
	for(i = 1; i < n; i++) {
		if((arr[i])->count < (arr[0])->count)
			swap(arr, ++last, i);
	}
	swap(arr, 0, last);
	sort_arr(arr, last);
	sort_arr(arr+last+1, n-last-1);
}

void swap(struct tnode *arr[], int l, int h)
{
	struct tnode *temp;
	temp = arr[l];
	arr[l] = arr[h];
	arr[h] = temp;
}

void arr_print()
{
	struct tnode **pointr = tree_arr;
	while(*pointr) {
		if((*pointr)->count > MNGSIZ)
			printf("%4d   %s\n", (*pointr)->count, (*pointr)->word);
		pointr++;
	}
}


/*
void treeprint(struct tnode *p)
{
	if(p != NULL) {
		treeprint(p->left);
		if(p->count > MNGSIZ)
			printf("%4d %s\n", p->count, p->word);
		treeprint(p->right);
	}
}
*/
