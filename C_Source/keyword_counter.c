/*this progarm can count declared word on c programm text*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXWORD 100

struct key{
	char *word;
	int count;
} keytab[] = {
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

#define NKEYS (sizeof(keytab)/sizeof(struct key))

int getword(char *, int);
int binsearch(char *);

main()
{
	int n;
	char word[MAXWORD];
	while(getword(word, MAXWORD) != EOF)
	{
		if(isalpha(word[0]))
			if((n = binsearch(word)) >= 0)
				keytab[n].count++;
	}
	for(n = 0; n < NKEYS; n++)
		if((keytab[n].count))
			printf("%d of %s\n", keytab[n].count, keytab[n].word);
	return 0;
}

int binsearch(char *word)
{
	int cond;
	int low, high, mid;
	low = 0;
	high = NKEYS - 1;
	while(low <= high)
	{
		mid = (low+high)/2;
		if((cond = strcmp(word, keytab[mid].word)) < 0)
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
	while(i < max && (c = getchar()) != EOF)
	{
		if(isalpha(c))
		{
			state = STATEON;
			i++;
			*src++ = c;
		} 
		else
		{
			if(state)
			{
				*src = '\0';
				return i;
			}
			state = STATEOFF;
		}
	}

	if(i >= max) 
	{
		printf("Error, letter larger 100 characters\n");
		return EOF;
	}
	if(c == EOF)
		return c;
	return -1;
}
