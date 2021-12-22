#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#define MAXNAME 15

struct str {
  char *name;
  int win_num;
};


int fnames_num(char *ar)
{
        int a = 1;
        while(*ar != '\0')
                if(*ar++ == ',')
                        a++;
        return a;
}

void form_nmstr(char *str, struct str node[], int len)
{
  int i, num;
  char *tmp;
    for(i = 0; i < len; i++) {
       node[i].name = (char *)malloc(MAXNAME);
       tmp = node[i].name;
       node[i].win_num = 0;
       num = 0;
       while(*str != ',' && *str != '\0') {
          *tmp++ = *str;
	  num++;
          node[i].win_num += toupper(*str++) - 64;
       }
       node[i].win_num += num;
       *tmp = '\0';
       str++;
    }
}

void calc_wn(int *w, struct str node[], int len)
{
        int i;
        for(i = 0; i < len; i++)
          node[i].win_num *= *w++;
          
}

void swap(struct str [], int, int);

void sort(struct str arr[], int high)
{
  int last, i;
  if(high <= 0)
    return;
  swap(arr, 0, rand() % high);
  last = 0;
  for(i = 1; i < high; i++) {
      if(arr[i].win_num > arr[last].win_num)
              swap(arr, last, i);
      else if(arr[i].win_num == arr[last].win_num)
              if(*(arr[i].name) > *(arr[last].name))
                        swap(arr, last, i);
  }
  swap(arr, 0, last);
  sort(arr, last - 1);
  sort(arr+last+1, high-last-1);
}

void swap(struct str arr[], int i, int j)
{
        struct str tmp;
        tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
}


char *nthRank(char* st, int* we, int n)
{ 
  if(!*st)
    return "No participants";
  int nms_num = fnames_num(st);
  struct str m[nms_num];
  form_nmstr(st, m, nms_num);
  calc_wn(we, m, nms_num);
  sort(m, nms_num);
  return m[n-1].name;
 }

main()
{
	char names[] = "Elijah,Chloe,Elizabeth,Matthew,Natalie,Jayden";
	int we[] = {1, 3, 5, 5, 3, 6};
	int n = 2;
	char *res;
	res = nthRank(names, we, n);
	printf("%s is winner\n", res);
}
