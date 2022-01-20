/* this programm can delete comments from file */

#include <stdio.h>

int main(int argc, char **argv)
{
	int c;
	FILE *d;
	if(argc != 2) {
		printf("Invalid input\n");
		return 1;
	}
	if((d = fopen(argv[1], "r+")))
		printf("File open!\n");
	else {
		printf("Error open file\n");
		return 2;
	}
	while((c = fgetc(d)) != EOF) {
		if(c == '/') {
			if((c = fgetc(d)) == '/') {
				fseek(d, -2, SEEK_CUR);
				while((c = fgetc(d)) != '\n') {
					fseek(d, -1, SEEK_CUR);
					fputc(32, d);
				}
			}
		}
		else
		{}
	}
	printf("Deleted successful!");
	return 0;	
}
	
	

	
