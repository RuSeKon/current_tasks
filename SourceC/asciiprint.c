/* this program can print ascii value of char */

#include <stdio.h>

#define MAXARRLEN 7
#define MAXASCODE 3
int main()
{
	int c;
	char arr[MAXARRLEN];

	printf("What do you want(code or symbol): ");
	
	for(int i = 0;i < MAXARRLEN && (c = getchar()) != '\n'; i++) {
		arr[i] = c;
		if(i == MAXARRLEN -1) {
			printf("Only CODE or SYMBOL stupid...");
			return 1;
		}
	}

	c = arr[0];

	if(c == 'c') {
		printf("Write your symbol: ");
		c = getchar();
		printf("ASII code: %d", c);
		return 0;
	}
	else if(c == 's') {
		printf("Write your code: ");
		scanf("%d", &c);
		printf("Your symbol: %c", c);
		return 0;
	}
}

