/* print all words of file with count their meetings */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXWORD 100
#define STATEON 1
#define STATEOFF 0
#define MNGSIZ 5

struct tnode {
	char *word;
	int count;
	struct tnode *left;
	struct tnode *right;
};

struct tnode *addtree(struct tnode *, char *);
void treeprint(struct tnode *);
int getword(char *, int);

main(int argc, char **argv)
{
	struct tnode *root;
	char word[MAXWORD];

	int mng_cmp, res;
	
	mng_cmp = argc > 1 ? atoi(argv[1]) : 6;
	root = NULL;
	while((res = getword(word, mng_cmp)) != EOF)
		if(res)
			root = addtree(root, word);
	treeprint(root);
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
	while((c = getchar()) != EOF) {
		if((isalpha(c) || c == '_')) {
			state = STATEON;
			if(i < max) {
				*src++ = c;
				i++;
			}
			else
				continue;
		} else {
			if(state) {
				state = STATEOFF;
				return i<max ? 0 : i;
			} 
			continue;
		}
	}
	if(c == EOF)
		return c;
	return -1;
}

void treeprint(struct tnode *p)
{
	if(p != NULL) {
		treeprint(p->left);
		if(p->count > MNGSIZ)
			printf("%4d %s\n", p->count, p->word);
		treeprint(p->right);
	}
}
