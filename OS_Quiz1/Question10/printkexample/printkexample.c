#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>

// Standard information about the module license, description, and author
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A cool demo of print k");
MODULE_AUTHOR("Mikhail's Cat");

// This function is called when the module is first inserted
int myinit(void)          /*    Init Module    */
{
    int someInt = 1;
    char str[] = "Kernel cats!";

    printk(KERN_CRIT "Hello, I am a module that was just inserted! Here is an integer %d and string %s\n", someInt, str);

    return 0;

}                
    
// This function is called when the module is removed from the kernel
void myexit(void)        /*    Exit Module    */
{
	printk(KERN_CRIT "Ok, I am being removed! Bye!\n");
}                       

// Specifies the function that should be invoked when the module is inserted
// i.e., the myinit() function above
module_init(myinit);    /*    Load Module MACRO    */

// Specifies the function to call 
module_exit(myexit);    /*    Remove Module MACRO    */


