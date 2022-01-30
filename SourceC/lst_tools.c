/* this program is about tools for 
 * work with lists */

#include <stdlib.h>


typedef struct node {
	int value;
	struct node *next;
} lst_node;

void lst_free(lst_node *); 

lst_node *list_copy(lst_node *src_frst, int len)
{
	lst_node *first, *last, *tmp;
	int i;
	first = (lst_node *)malloc(sizeof(lst_node));
	first->value = src_frst->value;
	tmp = (lst_node *)malloc(sizeof(lst_node));
	first->next = tmp;
	last = tmp;
	src_frst = src_frst->next;
	for(i = 1; i < len; i++) {
		last->value = src_frst->value;
		tmp = (lst_node *)malloc(sizeof(lst_node));
		last->next = tmp;
		last = tmp;
		src_frst = src_frst->next;
	}
	if(i != len) {
		lst_free(first);
		return NULL;
	}
	return first;
}

lst_node *lst_copy2(lst_node *src_frst, int len)
{
	lst_node *tmp;
	if(!len)
		return NULL;
	tmp = (lst_node *)malloc(sizeof(lst_node));
	tmp->value = src_frst->value;
	tmp->next = lst_copy2(src_frst->next, len--);
	return tmp;
}

lst_node *lst_merge(lst_node *frst_1, lst_node *frst_2)
{
	lst_node *tmp = frst_1;
	while((tmp = tmp->next) != NULL)
		;
	tmp = frst_2;
	return frst_1;
}


lst_node *lst_split(lst_node *frst_1, int point)
{
	lst_node *tmp, *frst_2;
	tmp = frst_1;
	while(tmp->value != point)
		tmp = tmp->next;
	frst_2 = tmp->next;
	tmp->next = NULL;
	return frst_2;
}

int lst_insrt_prev(lst_node *frst, lst_node *insrt, int point)
{
	lst_node *tmp;
	while(frst->next->value != point)
		frst = frst->next;
	if(frst == NULL)
		return -1;
	tmp = frst->next;
	frst->next = insrt;
	while(insrt->next != NULL)
		insrt = insrt->next;
	insrt->next = tmp;
	return 0;
}

int lst_insrt_after(lst_node *frst, lst_node *insrt, int point)
{
	lst_node *tmp;
	while(frst->value != point)
		frst = frst->next;
	if(frst == NULL)
		return -1;
	tmp = frst->next;
	frst->next = insrt;
	while(insrt->next != NULL)
		insrt = insrt->next;
	insrt->next = tmp;
	return 0;
}



void lst_free(lst_node *frst)
{
	lst_node *tmp;
	while(frst) {
		tmp = frst;
		frst = frst->next;
		free(tmp);
	}
}

void lst_reverse(lst_node *src)
{
	lst_node *first = NULL, *tmp = NULL, *fordel;
	while(src) {
		tmp = (lst_node *)malloc(sizeof(lst_node));
		tmp->value = src->value;
		tmp->next = first;
		first = tmp;
		fordel = src;
		src = src->next;
		free(fordel);
	}
	return;
}

lst_node *lst_reverse2(lst_node *frst, lst_node *last)
{
	if(frst == last) {
		frst->next = NULL;
		return NULL;
	}
	lst_node *tmp;
	tmp = frst;
	while(tmp->next != last)
		tmp = tmp->next;
	last->next = tmp;
	lst_reverse2(frst, tmp);
	return last;
}
