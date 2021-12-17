/*this progarm can count deaclarator word on c programm text*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXWORD 100
#define NKEYS 13

struct key{
	char *word;
	int count;
} keytab[NKEYS] = {
	{"auto", 0},
	{"break", 0},
	{"case", 0},
	{"char", 0},
	{"const", 0},
	{"include", 0},
	{"define", 0},
	{"continue", 0},
	{"default", 0},
	{"unsigned", 0},
	{"void", 0},
	{"volatile", 0},
	{"while", 0}
};

int getword(char *, int);
int binsearch(char *, struct key *, int);

main()
{
	int n;
	char word[MAXWORD];
	while(getword(word, MAXWORD) != EOF)
		if(isalpha(word[0]))
			if((n = binsearch(word, keytab, NKEYS)) >= 0)
				keytab[n].count++;
	for(n = 0; n < NKEYS; n++)
		if((keytab[n].count))
			printf("%d %s\n", keytab[n].count, keytab[n].word);
	return 0;
}

int binsearch(char *word, struct key tab[], int n)
{
	int cond;
	int low, high, mid;
	
	low = 0;
	high = n - 1;
	while(low <= high) {
		mid = (low+high)/2;
		if((cond = strcmp(word, tab[mid].word)) < 0)
			high = mid - 1;
		else if(cond > 0)
			low = mid + 1;
		else
			return mid;
	}
	return -1;
}

#define STATEON 1
#define STATEOFF 0

int getword(char *src, int max)	//get some trubble
{
	int i=0, c;
	int state = STATEOFF;
	while(i < max && (c = getchar()) != EOF) {
		if(isalpha(c)) {
			state = STATEON;
			i++;
			*src++ = c;
		} else {
			if(state) {
				*src = '\0';
				return i;
			}
			state = STATEOFF;
			continue;
		}
	}
	if(i >= max) {
		printf("Error, letter larger 100 char\n");
		return EOF;
	}
	if(c == EOF)
		return c;
	return -1;
}

