#include <linux/cpumask.h>
#include <linux/init.h>
#include <linux/module.h>  /* Needed by all modules */
#include <linux/kernel.h>  /* Needed for KERN_ALERT */
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/jiffies.h>
MODULE_LICENSE("Dual BSD/GPL");


#define MY_INFO(str,arg...) printk(KERN_ALERT str, ## arg);


//----------------------------------------

static int my_init(void){
	
 	/* enable user-mode access to the performance counter*/
  	asm ("MCR p15, 0, %0, C9, C14, 0\n\t" :: "r"(1));

  	/* disable counter overflow interrupts (just in case)*/
	asm ("MCR p15, 0, %0, C9, C14, 2\n\t" :: "r"(0x8000000f));

	MY_INFO("COUNTERS USER-MODE INITIALIZED");
	return 0;
}

static void my_exit(void)
{
	MY_INFO("EXITING READ TEMPERATURE MODULE ...");

}

module_init(my_init);
module_exit(my_exit);	

