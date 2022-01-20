/* maybe ls */

#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>

int main(int argc, char **argv)
{	
	DIR *dir;
	struct dirent *str;
	const char *name = ".";
	if(argc > 1)
		name = argv[1];
	dir = opendir(name);
	if(!dir) {
		perror(name);
		return 1;
	}
	while((str = readdir(dir)) != NULL) {
		printf("%s\n", str->d_name);
	}
	closedir(dir);
	return 0;
}

	 
