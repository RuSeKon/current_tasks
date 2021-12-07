/* this program can check file for () {} "" '' */

#include <stdio.h>

#define RBRACKET 41
#define RBRACE 125
#define RQUOTES 39
#define RDQUOTES 34


int srchbrk(FILE *descr, int line, int rchr, int lchr)
{
	int c, left = 1, right = 0, pos = 1;  
	while((c = fgetc(descr)) != '\n') {
		pos++;
		if(c == lchr)
			left++;
		if(c == rchr)
			right++;
	}
	fseek(descr, -pos, SEEK_CUR);
	if(left == right)
		return 1;
	else if(left > right) {
		printf("Missing %c on line %d\n", rchr, line);
		return 1;
	}
	else if(left < right) {
		printf("Missing %c on line %d\n", lchr, line);
		return 1;
	}
	return 1; 	
}

int srchbrc(FILE *descr, int line, int rchr, int lchr)
{
	int c, lchr = pos = 0;  
	while((c = fgetc(descr)) != EOF) {
		pos++;
		if(c == rchr)
			break;
		if(c == lchr) {
			printf("Missing %c after line %d\n", rchr, line);
			//NEED TO CHANGE SOLUTION
			fseek(descr, -pos, SEEK_CUR);
			return 1;
		}

	}
	fseek(descr, -pos, SEEK_CUR);
	return 0;
}

int srchquo(FILE *descr, int line, int chr)
{
	int c, cnt = 1, pos = 1;  
	while((c = fgetc(descr)) != '\n') {
		pos++;
		if(c == chr)
			cnt++;
	}
	fseek(descr, -pos, SEEK_CUR);
	if((cnt%2) == 0 )
		return 0;
	else
		printf("Missing %c on line %d\n", chr, line);
	return 1;
}

int main(int argc, char **argv)
{
	int c, ln = 1, quo, dqu;
	quo=dqu=1;
	enum left {bracket=40, brace=123, quotes=39, dquotes=34, line=10};
	FILE *d;
	if(argc != 2) {
		printf("Invalid input\n");
		return 1;
	}
	if((d = fopen(argv[1], "r")))
		printf("File open!\n...\n");
	else {
		printf("Error open file\n");
		return 2;
	}	
	while((c = fgetc(d)) != EOF) {
		switch(c){
			case line:
				ln++;
				quo=dqu=1;
				break;
			case bracket:
				srchbrk(d, ln, RBRACKET, bracket);
				break;
			case brace:
				srchbrc(d, ln, RBRACE, brace);
				break;
			case quotes:
				if(quo) {
					quo = srchquo(d, ln, RQUOTES);
					break;
				}
				else
					break;
			case dquotes:
				if(dqu) {
					dqu = srchquo(d, ln, RDQUOTES);
					break;
				}
				else
					break;
			default:
				;
		}
	}
	return 0;
}
