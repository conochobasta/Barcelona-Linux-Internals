/*  hello-5.c - Demonstrates command line argument passing to a module. */

/* Kernel Programming */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Peter Jay Salzman");

// These global variables can be set with command line arguments when you insmod
// the module in. 
//
static u8             mybyte = 'A';
static unsigned short myshort = 1;
static int            myint = 20;
static long           mylong = 9999;
static char           *mystring = "blah";
static int            myintArray[2] = { 0, 420 };

/*  Now we're actually setting the mechanism up -- making the variables command
 *  line arguments rather than just a bunch of global variables.
 */
MODULE_PARM(mybyte, "b");
MODULE_PARM(myshort, "h");
MODULE_PARM(myint, "i");
MODULE_PARM(mylong, "l");
MODULE_PARM(mystring, "s");
MODULE_PARM(myintArray, "1-2i");

MODULE_PARM_DESC(mybyte, "This byte really does nothing at all.");
MODULE_PARM_DESC(myshort, "This short is *extremely* important.");
// You get the picture.  Always use a MODULE_PARM_DESC() for each MODULE_PARM().


static int __init hello_5_init(void)
{
   printk(KERN_ALERT "mybyte is an 8 bit integer: %i\n", mybyte);
   printk(KERN_ALERT "myshort is a short integer: %hi\n", myshort);
   printk(KERN_ALERT "myint is an integer: %i\n", myint);
   printk(KERN_ALERT "mylong is a long integer: %li\n", mylong);
   printk(KERN_ALERT "mystring is a string: %s\n", mystring);
   printk(KERN_ALERT "myintArray is %i and %i\n", myintArray[0], myintArray[1]);
   return 0;
}


static void __exit hello_5_exit(void)
{
   printk(KERN_ALERT "Goodbye, world 5\n");
}


module_init(hello_5_init);
module_exit(hello_5_exit);