/* this programm can delete comments from file */

#include <stdio.h>

main(int argc, char **argv)
{
	int c;
	FILE *dscr;
	if(argc != 2)
	{
		printf("Invalid input\n");
		return 1;
	}
	if((dscr = fopen(argv[1], "r+")))
		printf("File open!\n");
	else 
	{
		printf("Error open file\n");
		return 2;
	}
	while((c = fgetc(dscr)) != EOF) 
	{
		if(c == '/')
		{
			if((c = fgetc(dscr)) == '/')
			{
				fseek(dscr, -2, SEEK_CUR);
				while((c = fgetc(dscr)) != '\n') //current position is after c
				{
					fseek(dscr, -1, SEEK_CUR);
					fputc(32, dscr);
				}
			}
		}
	}
	printf("Deleted successful!");
	return 0;	
}
