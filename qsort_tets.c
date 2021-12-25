/* this program is tester of quicksort time O */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#ifndef NUMOFITER 
#define NUMOFITER 5
#endif

#define MESSAGE 20 
#define OPTSZE 4

void qusort(double *, int);
double *form_arr(char *, int, char *);

main(int argc, char **argv)
{
	double time_spent = 0.0;
	double *arr;
	int max, i, p = 0;
	clock_t cl_beg, cl_end;
	char init_data[MESSAGE];
	char opt[OPTSZE];
	
	//parce command line and form initial data
	if(argv[1][0] == '-') {
		if(strchr(argv[1], 'o')) //form in direct order 
			opt[p++] = 'o';
		if(strchr(argv[1], 'O')) //form in reverse order
			opt[p++] = 'O';
		if(strchr(argv[1], 'r')) //form by random
			opt[p++] = 'r';
	}
	opt[3] = '\0';		

	if(argc > 2 && isdigit(argv[2][0]))	//size of sorted array
		max = atoi(argv[2]);
	else 
		max = 10;
	
	if((arr = form_arr(opt, max, init_data)) == NULL) {
		printf("Error by create array\n");
		return 3;
	}

	/*main part, sorted array*/
	for(i = NUMOFITER; i > 0; i--) {
		cl_beg = clock();
		qusort(arr, max-1);
		cl_end = clock();

		/*calculate result of implementation*/
		time_spent = (double)(cl_end - cl_beg) / CLOCKS_PER_SEC;
		printf("For %d elem of %s elapsed time is (%f)\n", 
				max, init_data, time_spent);
	}
	return 0;
}

void swap(double *, int, int);

void qusort(double *arr, int peak)
{
	int last, i;
	if(peak <= 0)
		return;
	swap(arr, 0, (rand()%peak));
	last = 0;
	for(i = 1; i < peak; i++) {
		if(arr[i] < arr[0])
			swap(arr, ++last, i);
	}
	swap(arr, 0, last);
	qusort(arr, last-1);
	qusort(arr+last+1, peak-last-1);
}

void swap(double *arr, int a1, int a2)
{
	double tmp;
	tmp = arr[a1];
       	arr[a1] = arr[a2];
	arr[a2] = tmp;
}

double *form_arr(char *param, int m, char *message)
{
	int i;
	double *tmp = (double *)malloc(sizeof(double) * m);

	if(strchr(param, 'o') && strchr(param, 'O') && strchr(param, 'r')) {
		int n, p;
		for(i = 0; i < m/3; i++)
			tmp[i] = i;
		for(n = i; n < 2*(m/3); n++)
			tmp[n] = rand()/m;
		for(p = n; p < m; p++)
			tmp[p] = m - p;
		strcat(message, "combine data\0");	
		return tmp;
	}

	if(strchr(param, 'o')) {
		for(i = 0; i < m; i++)
			tmp[i] = i;
		strcat(message, "direct order data\0");
	} else if(strchr(param, 'O')) {
		for(i = 0; i < m; i++)
			tmp[i] = m - i;
		strcat(message, "reverse order data\0");
	} else if(strchr(param, 'r')) {
		for(i = 0; i < m; i++) 
			tmp[i] = rand()/m;
		strcat(message, "random init data\0");
	}
	tmp[i] = '\0';
	return tmp;
}


