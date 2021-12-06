/* this calc can use infix notation */

#include <stdio.h>

#define MAXSTR 100


int makestr(char [], int);
void sortstation(char [], char []);




int main()
{
	int c;
	char infstr[MAXSTR];
	char pinstr[MAXSTR];


	makestr(infstr, MAXSTR);
	sortstation(infstr, pinstr);

	switch() {
		case:
		default:

	return;
}


int makestr(char src[], int max)
{
	int i, c;
	for(i = 0; i < max && (c = getchar()) != EOF && c != '\n'; i++) 
		src[i] = c;
	if(c == '\n')
		src[i] = '\0';
	return i;
}




#include <ctype.h>

#define MLT 4
#define DIV 3
#define SUM 2
#define INC 1

void push(char);
char pop();

void sortstation(char inf[], char pin[])
{
	int val, i, t;
	val=i=t=0;

	while(inf[i]) {
		while(inf[i] == ' ' || inf[i] == '\t')
			i++;
		if(isdigit(inf[i])) {
			while(isdigit(pin[t++] = inf[i++]) || inf[i] == '.');
			pin[--t] = ' ';
			i--;
		}
		else if(inf[i] == '.') {
			while(isdigit(pin[t++] = inf[i++]) || inf[i] == '.');
			pin[--t] = ' ';
			i--;
		}
		else {
			switch(inf[i++]) {
				case '-':
				     if(isdigit(inf[i+1]) || inf[i+1] == '.') {
					     pin[t++] = inf[i];
					     break;
				     }
				     else {
					     while(INC > VAL
					
				case ' ':
					continue;
				case '+':

	}
	}
	}

}













#define MAXSTACK 60

char stack[MAXSTACK];
int esp = 0;

void push(char s)
{
	if(esp < MAXSTACK)
		stack[esp++] = s; else
		printf("Failed: stack is fully\n");
	return;
}
	
char pop()
{	
	if(esp > 0)
		return stack[--esp];
	else
		printf("Failed: stack is empty\n");
	return 0.0;
}












int getop(char str[])
{}
