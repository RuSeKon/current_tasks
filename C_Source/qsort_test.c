/* this program is simple tester of quicksort time */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#ifndef NUMOFITER 
#define NUMOFITER 5
#endif

#define MESSAGE 20 
#define OPTSZE 3


void qusort(double *, int);
void form_arr(double *, char *, int, char *);

main(int argc, char **argv)
{
	double time_spent, max_time, middle;
	time_spent = max_time = middle = 0;
	double accuracy = 0.0000000001;
	double *arr;
	int max, i, p = 0;
	clock_t cl_beg, cl_end;
	char init_data[MESSAGE];
	char opt[OPTSZE];

	//parce command line and form initial data
	if(argc < 2)
	{
		strcpy(init_data, "direct order ");
		opt[0] = '\0';
	} 
	else if(argv[1][0] == '-')
	{
		if(strchr(argv[1], 'o')) //form in direct order 
		{
			opt[p++] = 'o';
			strcpy(init_data, "direct order ");
		}
		if(strchr(argv[1], 'O')) //form in reverse order
		{
			opt[p++] = 'O';
			strcpy(init_data, "reverse order ");
		}

		opt[OPTSZE-1] = '\0';		
	}

	if(argc > 2 && isdigit(argv[2][0]))	//size of sorted array
		max = atoi(argv[2]);
	else 
		max = 10;
	
	if((arr = (double*)malloc(sizeof(double)*max)) == NULL)
	{
		printf("Error memory allocating!\n");
		return -1;
	}


	/*main part, sorting array*/
	for(i = 0; i < NUMOFITER; i++) 
	{
		form_arr(arr, opt, max, init_data);

		cl_beg = clock();
		qusort(arr, max-1);
		cl_end = clock();

		/*calculate result of implementation*/
		time_spent = (double)(cl_end - cl_beg) / CLOCKS_PER_SEC;
		if((time_spent - max_time) > accuracy)
			max_time = time_spent;
		middle += time_spent;
		
	}

	free(arr); //arr is no longer needed in this context

	middle /= (double)NUMOFITER;
	printf("For %d elem of %s elapsed time:\n"
		       	"Max = %.6f, Mid = %.6f.\n", 
				max, init_data, max_time, middle);
	return 0;
}

void swap(double *, size_t, size_t);

void qusort(double *arr, int peak)
{
	size_t last, i;
	if(peak <= 0)
		return;
	srand(time(NULL));
	swap(arr, 0, (size_t)(rand()%peak));
	last = 0;
	for(i = 1; i < peak; i++) 
	{
		if(arr[i] < arr[0])
			swap(arr, ++last, i);
	}
	swap(arr, 0, last);
	qusort(arr, last);
	qusort(arr+last+1, peak-last-1);
}

void swap(double *arr, size_t a1, size_t a2)
{
	double tmp;
	tmp = arr[a1];
    arr[a1] = arr[a2];
	arr[a2] = tmp;
}

void form_arr(double *tmp, char *param, int m, char *message)
{
	size_t i=0;

	srand(time(NULL));
	if(strchr(param, 'o') && strchr(param, 'O') && strchr(param, 'r')) {
		size_t n, p;
		for(i = 0; i < m/3; i++)
			tmp[i] = (double)i;
		for(n = i; n < 2*(m/3); n++)
			tmp[n] = (double)rand()/m;
		for(p = n; p < m; p++)
			tmp[p] = (double)m - p;
		return;
	}

	if(strchr(param, 'o')) 
		for(i = 0; i < m; i++)
			tmp[i] = (double)i;
	else if(strchr(param, 'O')) 
		for(i = 0; i < m; i++)
			tmp[i] = (double)(m - i);
	else 
		for(i = 0; i < m; i++) 
			tmp[i] = (double)(rand()%m);
}


