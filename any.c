/*this program return position of each s2 symbol in s1*/

#include <stdio.h>

#define MAXSTR 9
\\its a test

int any(int src[],int from[])
{
	int i, j;
	for(i = 0; from[i] != '\0'; i++) {
		j = 0;
		for(; src[j] != '\0'; j++) {
			if(src[j] == from[i])
				return ++j;
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
		src[i + 1] = '\0';
	return i;
}


int main() //for check squezee
{
	int pos;
	int array[MAXSTR];
	int from[MAXSTR - 1];
	if(!(makestr(array, MAXSTR - 1))) {
		printf("Error array create\n");
		return 1;
	}
	printf("Now print from: ");
	if(!(makestr(from, MAXSTR - 2))) {
		printf("Error from create\n");
		return 1;
	}
	if((pos = any(array, from))) {
		printf("POSITION: %d\n", pos);
		return 0;
	}
	else {
		printf("Some error\n");
		return 0;	
	}
}

