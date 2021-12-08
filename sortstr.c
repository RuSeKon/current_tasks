/* this programm can sort input massive of
 * \n separated strings */
#include <stdio.h>
#include <string.h>

#define MAXLNS 50
#define MAXCHR 500


char *matrix[MAXLNS];

int input(char *[],  int max);
void qsrt(char *[], int left, int right);
void output(char *[]);

int main()
{
	int nlines;
	if((nlines = input(matrix, MAXLNS)) > 0) {
		qsrt(matrix, 0, nlines-1);
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

void swap(char *[], int l, int r);

void qsrt(char *arr[], int left, int right)
{
	int last;
	if(left >= right)
		return;
	swap(arr, left, (left+right)/2);
	last = left;
	for(int i=left+1; i <= right; i++)
		if(strcmp(arr[i], arr[left]) < 0)
			swap(arr, ++last, i);
	swap(arr, left, last);
	qsrt(arr, left, last - 1);
	qsrt(arr, last + 1, right);
}

void swap(char *arr[], int l, int r)
{
	char *tmp;
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
			
	


				
	
	


