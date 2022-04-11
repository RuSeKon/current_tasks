/*
 * stop.c - Illustration of multi field modules
*/

#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Konyaskin Ruslan");

void cleanup_module()
{
	printk(KERN_INFO "Short is the life of a"
						" kernel module\n");
}

