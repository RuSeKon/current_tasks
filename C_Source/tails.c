/* print n(or 10 by default) latest inputed strings */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXSTRING 100
#define MAXLINE 80

char *buffer[MAXSTRING];

int input(int);
void print_str(int how, int num);
int how(char *);


main(int argc, char **argv)
{
	int counter;
	if((counter = input(MAXSTRING)) > 0) {
		if(argc > 1) {
			print_str(how(*++argv), counter);
			return 0;
		}
		else {
			print_str(10, counter);
			return 0;
		}
	}
	return 0;	
			
}


int getlines(char *, int);
char *allocs(int);

int input(int max)
{
	char line[MAXLINE], *ptr;
	int len, cnt_str = 0;
	while((len = getlines(line, MAXLINE)) > 0) {
		if(cnt_str >= max) {
			printf("Error, too much string\n");
			return -1;
		}
		else if((ptr = allocs(len)) == NULL) {
			printf("Error, buffer allocating\n");
			return -2;
		}
		else
			buffer[cnt_str++] = ptr;
	}
	return cnt_str;
}
			
int getlines(char *dst, int max)
{
	int i;
	char c;
	for(i = 0; i < max && (c = getchar()) != EOF && c != '\n'; i++)
		dst[i] = c;
	dst[i] = '\0';
	return i;
}

void print_str(int how, int num)
{
	int start = num - how;
	if(how <= num) { 
		while(start <= num)
			printf("%s", buffer[start++]);
		return;
	}
	else  {
		printf("No much string\n");
		return;
	}
}




#define MAXHEAP 10000
char simple_heap[MAXHEAP];
char *heap_ptr = simple_heap;


char *allocs(int num)
{
	if((heap_ptr + num - simple_heap) <= MAXHEAP) {
		heap_ptr += num;
		return heap_ptr - num;
	}
	else 
		return NULL;
}

void afree(int num)
{
	if((heap_ptr - num) >= simple_heap) {
		heap_ptr -= num;
		return;
	}
	else {
		fprintf(stderr, "Simple_heap buffer is already empty\n");
		return;
	}
}

int how(char *src)
{
	int res = 0;
	while(!isdigit(*src++))
		;
	while(isdigit(*src))
		res = res*10 + (*src++ - 48);
	return res;
}
