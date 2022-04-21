/* This function can chnge hex digit */

#include <stdio.h>

#define MAXCHAR 10


int makestr(int src[], int max)
{
	int i, c;
	for(i = 0; i < max && (c = getchar()) != EOF && c != '\n'; i++) 
	{
		if(c >= 'A' && c <= 'F')
			src[i] = c;
		else if(c == 'x' || (c >= 'a' && c <= 'f'))
			src[i] = c;
		else if(c >= '0' && c <= '9')
			src[i] = c;
		else 
			return 0;
	}
	if(c == '\n')
		src[i + 1] = '\0';
	return i;
}

int is_str(int src[])
{
	if(src[0] == '0' && (src[1] == 'x' || src[1] == 'X'))
		return 0;
	else
		return 1;
}

int change(int src[])
{
	int accum = 0;
	for(int i = 2; src[i] != '\0'; i++) 
	{
		if(src[i] >= '0' && src[i] <= '9')
			accum = (accum*16) + src[i] - 48;
		else if(src[i] >= 'A' && src[i] <= 'F')
			accum = (accum*16) + src[i] - 55;
		else if(src[i] >= 'a' && src[i] <= 'f')
			accum = (accum*16) + src[i] - 87;
	}
	return accum;
}


int main()
{
	int integer, len; int array[MAXCHAR + 1];
	if(!(len = makestr(array, MAXCHAR))) 
	{
		printf("Invalid input\n");
		return 1;
	}
	if(is_str(array)) 
	{
		printf("Not a hex digits\n");
		return 1;
	}

	printf("Number %d\n", integer = change(array));
	return 0;
}
