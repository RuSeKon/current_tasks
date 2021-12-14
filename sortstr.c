/* this programm can sort input massive of
 * \n separated strings */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLNS 50
#define MAXCHR 500
#define OPTSZE 4
#define ENABLE 1
#define DISABLE 0

char *matrix[MAXLNS];

int input(char *[],  int max);
void qsrt(void *[], int left, int right, char *optn,
		int (*comp)(void *, void *, char *));
void output(char *[]);
int numcmp(char *, char *, char *);
int strcmpr(char *, char *, char *);

int main(int argc, char **argv)
{
	int nlines, i, s;
	int numeric = DISABLE;
	char opt[OPTSZE];
	for(s = 0, i = 1; i < argc; i++) {		//CREATE AN OPTION STR
		for(int p = 0; argv[i][p]; p++) {
			if(argv[i][p] == '-')
				continue;
			else if(argv[i][p] == 'n')  //for numeric compare 
				numeric = ENABLE;
			else if(argv[i][p] == 'r') // reversive compare
				opt[s++] = 'r';
			else if(argv[i][p] == 'f')
				opt[s++] = 'f'; //for register of letter doesn't matter
			else if(argv[i][p] == 'd')
				opt[s++] = 'd'; //only letters, digits and spaces
			else {
				printf("Unknown argument (%s)\n", argv[i]);
				return 0;
			}
		}
	}
	opt[s] = '\0';
	if((nlines = input(matrix, MAXLNS)) > 0) {
		qsrt((void**) matrix, 0, nlines - 1, opt,
				(int (*)(void *, void*, char *))
				(numeric ? numcmp : strcmpr));	//Sorry for this
	       						//3-rd floor declaration		
		output(matrix);
		return 0;
	}
	else {
		printf("Input is empty\n");
		return 1;
	}
}

char *getln(int);

int input(char *arr[], int max)
{
	char *p;
	int i;
	for(i=0; i < max; i++) {
		if((p = getln(MAXCHR/MAXLNS))) {
			if(*p == EOF)
				break;
			matrix[i] = p;
		}
		else {
			i--; //Needed to change
		}
	}
	return i;
}

void swap(void *[], int l, int r);

void qsrt(void *arr[], int left, int right, char *optn, 
		int (*comp)(void *, void*, char *))
{
	int last;
	if(left >= right)
		return;
	swap(arr, left, (left+right)/2);
	last = left;
	for(int i=left+1; i <= right; i++) {
		if((*comp)(arr[i], arr[left], optn) < 0)
			swap(arr, ++last, i);
	}
	swap(arr, left, last);
	qsrt(arr, left, last - 1, optn, comp);
	qsrt(arr, last + 1, right, optn, comp);
}

void swap(void *arr[], int l, int r)
{
	void *tmp;
	tmp = arr[l];
	arr[l] = arr[r];
	arr[r] = tmp;
}

char line[MAXCHR];
char *ptr = line;

char *getln(int max)
{	
	int c = 0;
	while(c < max && (*ptr = getchar()) != EOF && *ptr !=  '\n') {
		ptr++;
		c++;
	}
	if(*ptr == EOF)
		return ptr;
	else if(c <= 0) {
		printf("Line is empty\n");
		return NULL;
	}
	else if(*ptr == '\n') {
		*ptr++ = '\0';
		c++;
	}
	return ptr - c;
}

void output(char *arr[])
{
	while(*arr)
		printf("%s\n", *arr++);
}

double counter(char *s, int lower);
int reparce(char pat, char *src);

int numcmp(char *s1, char *s2, char *optn)
{
	double val1  = reparce('f', optn) ? counter(s1, 1) : counter(s1, 0);
	double val2 = reparce('f', optn) ? counter(s2, 1) : counter(s2, 0);
	double res;
	if(val1 == val2)
		return 0;
	else {
		res = val1 - val2;
		return reparce('r', optn) ? -res : res;
	}
}	

int strcmpr(char *s1, char *s2, char *optn)
{
	int i, res;
	for(i = 0; s1[i] == s2[i]; i++) {
		if(s1[i] == '\0')
			return 0;
	}
	if(reparce('f', optn))
		res =  tolower(s1[i]) - tolower(s2[i]);
	else
		res = s1[i] - s2[i];
	return reparce('r', optn) ? -res : res;

}

double counter(char *s, int lower)		//for calculating float equ of string
{
	double res = 0.0;
	if(lower) {
		while(*s) {
			res += tolower(*s);
			s++;}
	}
	else {
		while(*s) {
			res += *s;}
	}
	return res;
}

int reparce(char pat, char *src)	//for reparcing option string
{
	while(*src)
		if(*src == pat)
			return 1;
		else
			src++;
	return 0;
}
