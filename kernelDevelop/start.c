/*
 * start.c - Illustration of multi field modules
*/

#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Konyashkin Ruslan");

int init_module(void)
{
	printk(KERN_INFO "Hello, world - this is the"
						" kernel speaking\n");
	return 0;
}
