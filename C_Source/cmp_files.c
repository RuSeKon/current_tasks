/*this programm can compare two files strings 
 * and print first different string*/

#include <stdio.h>
#include <string.h>

#define MAXSTR 80

main(int argc, char **argv)
{
	FILE *f1, *f2;
	char buffer1[MAXSTR];
	char buffer2[MAXSTR];

	if(argc < 3) {
		printf("Error: please write files name\n");
		return 1;
	}

	if((f1 = fopen(argv[1], "r")) == NULL) {
		printf("Error open file\n");
		return 2;
	} else if((f2 = fopen(argv[2], "r")) == NULL) {
		printf("Error open file\n");
		return 3;
	}

	while(fgets(buffer1, MAXSTR, f1))
	     if(fgets(buffer2, MAXSTR, f2)) {
		if(strcmp(buffer1, buffer2)) {
		    printf("%s is first stranger string\n", buffer1);
		    fclose(f1);
		    fclose(f2);
		    return 0;
		}
	     } else {
		printf("Second file is empty, but first not\n");
		fclose(f1);
		fclose(f2);
		return 0;
	     }
	fclose(f1);
	fclose(f2);
	printf("Both files are identity\n");
	return 0;
}

