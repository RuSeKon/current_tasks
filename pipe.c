/* pin ball */

#include <stdio.h>
#include <unistd.h>
#include <time.h>

main()
{
	int first_pipe[2];
	int second_pipe[2];

	if(pipe(first_pipe))
		return 1;
	if(pipe(second_pipe))
		return 1;

	int res, r, w;
	time_t current;
	char buf[1];

	write(first_pipe[1], "o", 1); /*enter a ball to the game */

	if((res = fork()) == 0)
	{
		r = second_pipe[0];
		close(second_pipe[1]);

		w = first_pipe[1];
		close(first_pipe[0]);

		current = time(NULL);
		int i;
		for(i=0; (time(NULL) - current) < 1; i++)
		{
			if(read(r, buf, 1))
				write(w, "o", 1);
		}
		printf("Child landed %d hits in a second!\n", i);
	}
	else
	{
		r = first_pipe[0];
		close(first_pipe[1]);

		w = second_pipe[1];
		close(second_pipe[0]);

		current = time(NULL);

		int i;
		for(i=0; (time(NULL) - current) < 1; i++)
		{	
			if(read(r, buf, 1))
				write(w, "o", 1);
		}
		printf("Parent landed %d hits in a second!\n", i);
	}
	return 0;	
}
