/* this programm can change a-d to abcd */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAXSTR 60

int makestr(char src[], int max)
{
	int i, c;
	for(i = 0; i < max && (c = getchar()) != EOF && c != '\n'; i++) 
		src[i] = c;
	if(c == '\n')
		src[i] = '\0';
	return i;
}

int expand(char dst[],char src[])
{
	assert(src);
	int s, d;
	for(s=d=0; src[s] != '\0'; s++) {
		if(s == 0 || src[s] != '-') { //Frst char or not - char
			dst[d] = src[s];      //or lst char will already write
			if(!(src[s+1]))	      //If 
				break;
			else {
				d++;	      //If not last char
				continue;
			}
		}
		else if(src[s] == '-') {
			char temp = src[s-1];
			for(; dst[d-1] < src[s+1]; d++) {
				assert(d < MAXSTR);
				dst[d] = ++temp;
			}
			s++;
			
		}
	}
	dst[d] = '\0';
	return d;
}

int main()
{
	char array[MAXSTR];
	char dest[MAXSTR];
	if(!(makestr(array, MAXSTR))) {
		fprintf(stderr, "String did't make\n");
		exit(EXIT_FAILURE);
	}
	if(!(expand(dest, array))) {
		fprintf(stderr, "Did't expand\n");
		exit(EXIT_FAILURE);
	}
	printf("%s is input, %s is output\n", array, dest);
	return 0;
}
