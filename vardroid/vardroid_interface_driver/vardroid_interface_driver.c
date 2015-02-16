

/* -------   VARDROID PROJECT MODULE FOR ANDROID/LINUX   ----------------------------
 * 
 * Author: Andrea Bartolini
 *
 * Modified by : Pietro Mercati
 * email : pimercat@eng.ucsd.edu
 * 
 * If using this code for research purposes, include 
 * references to the following publications
 * 
 * 1) P.Mercati, A. Bartolini, F. Paterna, T. Rosing and L. Benini; A Linux-governor based 
 *    Dynamic Reliability Manager for android mobile devices. DATE 2014.
 * 2) P.Mercati, A. Bartolini, F. Paterna, L. Benini and T. Rosing; An On-line Reliability 
 *    Emulation Framework. EUC 2014
 * 
	This file is part of DRM.
        DRM is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        DRM is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with DRM.  If not, see <http://www.gnu.org/licenses/>.
*/





#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/workqueue.h>
#include <asm/uaccess.h>
#include <linux/slab.h>

#include <linux/cpumask.h>

#include <linux/cdev.h>
#include <linux/device.h>  //for class_create

#define VARDROID_MAJOR 34
#define VARDROID_NAME "vardroid_interface"

#define SELECT_BUBBLE 		1
#define SELECT_BUBBLE_LENGTH 	3

extern int vardroid_select_bubble_global;
extern int vardroid_bubble_length;

/* ---- Private Constants and Types -------------------------------------- */
static char vardroidBanner[] __initdata = KERN_INFO "User Mode VARDROID INTERFACE Driver: 1.00\n";

/* ---- Private Variables ------------------------------------------------ */
static  dev_t           vardroidDrvDevNum = 0;   //P: actual device number
static  struct class   *vardroidDrvClass = NULL; 
static  struct  cdev    vardroidDrvCDev;  //P: kernel's internal structure that represent char devices

//------------------------------------------------------------------------------------------------------

long vardroid_mod_init(void){
	return 0;
}

long vardroid_mod_exit(void){
	return 0;
}

/*long vardroid_int_ready(void* _ext_buf){
	int * ext_buf = _ext_buf;
	if (copy_to_user(ext_buf,&__get_cpu_var(monitor_stats_index), sizeof(int)))
		return -EFAULT;
	return 0;
}*/

static int vardroid_open (struct inode *inode, struct file *file) {
	printk(KERN_ALERT "monitor_open\n");
	return 0;
}

/* close function - called when the "file" /dev/monitor is closed in userspace  
*	execute the monitor_mod_exit for each cpu on-line in the systems
*/
static int vardroid_release (struct inode *inode, struct file *file) {
	printk(KERN_ALERT "vardroid_release\n");
	return 0;
}


static long vardroid_ioctl(struct file *file,
		unsigned int cmd, unsigned long arg) {
	long retval = 0;

	switch ( cmd ) {
		/* Pietro : the value of seleceted_cpu should be passed from userspace and determined the location
			of all variables which are defined per cpu */
		case SELECT_BUBBLE:
			if ( copy_from_user( &vardroid_select_bubble_global, (int *)arg, sizeof(vardroid_select_bubble_global) )) {
							
				return -EFAULT;
			}
			printk(KERN_ALERT "vardroid_select_bubble_global = %u\n\n", vardroid_select_bubble_global);
			break;
			
		case SELECT_BUBBLE_LENGTH:

			if ( copy_from_user( &vardroid_bubble_length, (int *)arg, sizeof(vardroid_bubble_length) )) {
							
			printk(KERN_ALERT "vardroid_bubble_length ERROR\n\n");
				return -EFAULT;
			}
			printk(KERN_ALERT "vardroid_bubble_length = %u\n\n", vardroid_bubble_length);
			break;
			
		default:
			printk(KERN_ALERT "DEBUG: vardroid_ioctrl - You should not be here!!!!\n");
			retval = -EINVAL;
	}
	return retval;
}

// define which file operations are supported
struct file_operations vardroid_fops = {
	.owner		=	THIS_MODULE,
	.llseek		=	NULL,
	.read		=	NULL,
	.write		=	NULL,
	.readdir	=	NULL,
	.poll		=	NULL,
	.unlocked_ioctl	=	vardroid_ioctl,
	.mmap		=	NULL,
	.open		=	vardroid_open,
	.flush		=	NULL,
	.release	=	vardroid_release,
	.fsync		=	NULL,
	.fasync		=	NULL,
	.lock		=	NULL,
	//.readv		=	NULL,
	//.writev		=	NULL,
};


//module initialization
static int __init vardroid_init_module (void) {
    int     rc = 0;

    printk( vardroidBanner );

    if ( MAJOR( vardroidDrvDevNum ) == 0 )
    {
        /* Allocate a major number dynamically */
        if (( rc = alloc_chrdev_region( &vardroidDrvDevNum, 0, 1, VARDROID_NAME )) < 0 )
        {
            printk( KERN_WARNING "%s: alloc_chrdev_region failed; err: %d\n", __func__, rc );
            return rc;
        }
    }
    else
    {
        /* Use the statically assigned major number */
        if (( rc = register_chrdev_region( vardroidDrvDevNum, 1, VARDROID_NAME )) < 0 )  //P: returns 0 if ok, negative if error
        {
           printk( KERN_WARNING "%s: register_chrdev failed; err: %d\n", __func__, rc );
           return rc;
        }
    }

    cdev_init( &vardroidDrvCDev, &vardroid_fops );
    vardroidDrvCDev.owner = THIS_MODULE;

    if (( rc = cdev_add( &vardroidDrvCDev, vardroidDrvDevNum, 1 )) != 0 )
    {
        printk( KERN_WARNING "%s: cdev_add failed: %d\n", __func__, rc );
        goto out_unregister;
    }

    /* Now that we've added the device, create a class, so that udev will make the /dev entry */

    vardroidDrvClass = class_create( THIS_MODULE, VARDROID_NAME );
    if ( IS_ERR( vardroidDrvClass ))
    {
        printk( KERN_WARNING "%s: Unable to create class\n", __func__ );
        rc = -1;
        goto out_cdev_del;
    }

    device_create( vardroidDrvClass, NULL, vardroidDrvDevNum, NULL, VARDROID_NAME );

    goto done;

out_cdev_del:
    cdev_del( &vardroidDrvCDev );

out_unregister:
    unregister_chrdev_region( vardroidDrvDevNum, 1 );

done:
    return rc;
}

//module exit function
static void __exit vardroid_cleanup_module (void) {
        printk(KERN_ALERT "cleaning up module\n");
	
	device_destroy( vardroidDrvClass, vardroidDrvDevNum );
	class_destroy( vardroidDrvClass );
	cdev_del( &vardroidDrvCDev );

	unregister_chrdev_region( vardroidDrvDevNum, 1 );
}

module_init(vardroid_init_module);
module_exit(vardroid_cleanup_module);
MODULE_AUTHOR(" Pietro Mercati and Andrea Bartolini" );
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Samples voltage adn frequency and updates average values");
