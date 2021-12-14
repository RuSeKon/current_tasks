/* this programm can sort input massive of
 * \n separated strings */

#include <stdio.h>
#include <stdlib.h>

#define MAXLNS 50
#define MAXCHR 500


char *matrix[MAXLNS];

int input(char *[],  int max);
void qsrt(void *[], int left, int right, 
		int (*comp)(void *, void *));
void output(char *[]);
int numcmp(char *, char *);
int strcmpr(char *, char *);

int main(int argc, char **argv)
{
	int nlines;
	int numeric = 0;
	if(argc > 1 && strcmpr(argv[1], "-n") == 0)
		numeric = 1;
	if((nlines = input(matrix, MAXLNS)) > 0) {
		qsrt((void**) matrix, 0, nlines - 1, (int (*)(void *, void*))
					(numeric ? numcmp : strcmpr));
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

void qsrt(void *arr[], int left, int right, int (*comp)(void *, void*))
{
	int last;
	if(left >= right)
		return;
	swap(arr, left, (left+right)/2);
	last = left;
	for(int i=left+1; i <= right; i++) {
		if((*comp)(arr[i], arr[left]) < 0)
			swap(arr, ++last, i);
	}
	swap(arr, left, last);
	qsrt(arr, left, last - 1, comp);
	qsrt(arr, last + 1, right, comp);
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

int numcmp(char *s1, char *s2)
{
	double floats1 = atof(s1);
	double floats2 = atof(s2);
	if(floats1 == floats2)
		return 0;
	else
		return floats1 - floats2;
}	

int strcmpr(char *s1, char *s2)
{
	int i;
	for(i = 0; s1[i] == s2[i]; i++) {
		if(s1[i] == '\0')
			return 0;
	}
	return s1[i] - s2[i];

}
