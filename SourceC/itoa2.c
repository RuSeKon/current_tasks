/* This program can integer to array */

#include <stdio.h>
#include <string.h>

#define SHWIDTH 8  //To good practic must multiple of two

void reverse(char s[])
{
	int c, i, j;
	for(i = 0, j = strlen(s)-1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

int itoa(char arr[], int n, int shield)
{
	int sign, i;
	char tmp;
	if((sign = n) < 0)
		n = (~n) + 1;
	i = 0;
	do {
		tmp = (n > 0 ? n % 10 : -(n % 10));
		arr[i++] = tmp + 48;
	} while ((n /= 10) != 0);

	while(i < shield) {
		if((i + 1 == shield) && sign < 0) {
			arr[i++] = '-';
			break; }	
		arr[i++] = '0';
	}
	arr[i] = '\0';
	reverse(arr);
	printf("%s is array\n", arr);
	return 0;
}	


int main()
{
	char str[SHWIDTH];
	int numb;
	printf("Enter you numbr:");
	scanf("%d", &numb);
	printf("\n intered numb = %d\n", numb);
	itoa(str, numb, SHWIDTH);
	return 0;

}
