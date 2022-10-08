#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>

MODULE_LICENSE("MRL");
MODULE_DESCRIPTION("Question 10 Driver");
MODULE_AUTHOR("Matthew R Luna");

int myinit(void) {
	//printk(KERN_CRIT "Procprint Kernel Module start:\n");
	struct task_struct* pTask;
	printk(KERN_CRIT "Procprint Kernel Module start:\n");	
	for_each_process(pTask) {
		printk(KERN_CRIT "PID: %d\n", pTask->pid);
		printk(KERN_CRIT "\tNAME: %s\n", pTask->comm);
		printk(KERN_CRIT "\tLST_CPU: %d\n", pTask->on_cpu);
		printk(KERN_CRIT "\tPERSONALITY: %d\n", pTask->personality);	
	}

	return 0;
}                
    
void myexit(void) {
	printk(KERN_CRIT "Procprint is being removed!\n");
}                       

module_init(myinit);

module_exit(myexit);


