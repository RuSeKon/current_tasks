/* this programm can sort input massive of
 * \n separated strings */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLNS 50
#define MAXCHR 500
#define OPTSZE 5
#define ENABLE 1
#define DISABLE 0
#define FLDSZE 4
#define SEPRTR '.'
#define EMPTY "0"

char *matrix[MAXLNS];

int input(char *[],  int max);
void qsrt(void *[], int left, int right, char *optn,
		int (*comp)(void *, void *, char *));
void output(char *[]);
int numcmp(char *, char *, char *);
int strcmpr(char *, char *, char *);

int main(int argc, char **argv)
{
	int nlines, i, s, p, f, d;
	int numeric = DISABLE;
	char opt[OPTSZE];
	char fld[FLDSZE];
	for(s = 0, i = 1; i < argc; i++) {	//CREATE AN OPTION STR
		for(p = 0; argv[i][p]; p++) {	//run on oneself argument
			if(s >= OPTSZE) {
				printf("Too many arguments\n");
				return 0; }

			if(argv[i][p] == '-') {
			    if(argv[i][p + 1] == '-') {   //Write field name to fld[]
			        for(f=0, d=p+2; f < FLDSZE && argv[i][d]; f++, d++) {
				    fld[f] = argv[i][d]; }
			    	fld[FLDSZE-1] = '\0';
				if(strcmpr(fld, "txt", EMPTY) && strcmpr(fld, "num", EMPTY)){
					printf("Wrong field name %s\n", fld);	
					return -1; }
				break;
			    }
			    else
			        continue;
		        }	    
			else if(argv[i][p] == 'n')	//for numeric compare  
				numeric = ENABLE;
			else if(argv[i][p] == 'r')	//reversive compare
				opt[s++] = 'r';
			else if(argv[i][p] == 'l') 	//for register of letter doesn't matter
				opt[s++] = 'l';
			else if(argv[i][p] == 'd') 	//only letters, digits and spaces
				opt[s++] = 'd';
			else if(argv[i][p] == 'f') 	//sort by fields of strings
				opt[s++] = 'f';
			else {
				printf("Unknown argument (%s)\n", argv[i]);
				return -2;
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

char line[MAXCHR];
char *ptr = line;

char *getln(int max)
{	
	int i;
	char c;
	for(i = 0; i < max && (c = getchar()) != EOF && c !=  '\n'; i++) { 
		*ptr++ = c;
		i++;
	}
	if(*ptr == EOF)
		return ptr;
	else if(i <= 0) {
		printf("Line is empty\n");
		return NULL;
	}
	else if(*ptr == '\n') {
		*ptr++ = '\0';
		c++;
	}
	return ptr - i;
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

void output(char *arr[])
{
	while(*arr)
		printf("%s\n", *arr++);
}

double counter(char *s, int lower);
int reparce(char pat, char *src);

int numcmp(char *s1, char *s2, char *optn)
{
	double val1  = reparce('l', optn) ? counter(s1, 1) : counter(s1, 0);
	double val2 = reparce('l', optn) ? counter(s2, 1) : counter(s2, 0);
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
	if(reparce('l', optn))
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
