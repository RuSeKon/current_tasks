/*this program return position of s2 symbol in s1*/

#include <stdio.h>

#define MAXSTR 9

int any(int src[],int from[])
{
	int i, j;
	for(i = 0; from[i] != '\0'; i++) {
		j = 0;
		for(; src[j] != '\0'; j++) {
			if(src[j] == from[i])
				return j+1;
		}
	}
	return 0;
}

int makestr(int src[], int max)
{
	int i, c;
	for(i = 0; i < max && (c = getchar()) != EOF && c != '\n'; i++) 
		src[i] = c;
	if(c == '\n')
		src[i] = '\0';
	return i+1;
}


int main() //for check squezee
{
	int pos;
	int array[MAXSTR];
	int from[MAXSTR - 1];
	printf("Enter source string: ");
	
	makestr(array, MAXSTR - 1);
	printf("Now enter search symbol: ");
	makestr(from, MAXSTR - 1);

	if((pos = any(array, from))) {
		printf("POSITION: %d\n", pos);
		return 0;
	} else {
		printf("Symbol didn't find!\n");
		return 1;	
	}
}

