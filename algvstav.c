/* solution of intrace algortm */

#include <stdio.h>

#define MAXCHAR 100

int makestring(char string[], int max)
{	
	int c, i;
	for(i = 0; i < max && (c = getchar()) != EOF && c != '\n'; i++) 
		string[i] = c;
	if(c == '\n') {
		string[i] = c;
		i++;
	}
	string[i] = '\0';
	return --i;
} 
		

int reverse(char string[], int length)
{
	int i, j;
	for(j = 1; j < length; j++){
		char key = string[j];
		i = j - 1;
		while(i >= 0  && string[i] < key){
			string[i + 1] = string[i];
			i--;
		}
		string[i + 1] = key;
	}
	return 0;
}
		



int main()
{
	int l;
	char string[MAXCHAR];
	if((l = makestring(string, MAXCHAR))) {
		reverse(string, l);  
		printf("%s", string);
	} 
	return 0;
}
