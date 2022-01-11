/* this program is about tools for 
 * work with lists */

#include <stdlib.h>


typedef struct node {
	int value;
	struct node *next;
} lst_node;

int list_copy(lst_node *src_frst, int len)
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
	if(i != len)
		return -1;
	return 0;
}
	
