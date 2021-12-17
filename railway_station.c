/* This is my implementation of Sort station by Deykstra */

#include <stdio.h>

#define MAXSTR 100

int makestr(char src[], int max)
{
	int i, c;
	for(i = 0; i < max && (c = getchar()) != EOF && c != '\n'; i++) 
		src[i] = c;
	if(c == '\n')
		src[i] = '\0';
	return i;
}

void sortstation(char dst[], char src[], int max)
{
	int i;
	char stack[max];

	for(i = 0; src[i]; i++) {
		switch(src[i])
}

int main()
{
	char inp[MAXSTR];
	char outp[MAXSTR];

	if(!makestr(inp, MAXSTR))
		printf("Failed: string did't make\n");
