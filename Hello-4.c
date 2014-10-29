/*
 * Hello-4.c - Demonstrates module documentation
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#define DRIVER_AUTHOR 	"Gerry Smyth"
#define DRIVER_DESC	"A sample driver"

static int __init init_hello_4 ( void )
{
    printk ( KERN_INFO "Hello world no 4\n" );
    return 0;
}

static void __exit cleanup_hello_4 ( void )
{
    printk ( KERN_INFO "Goodbye world 4\n" );
}

module_init ( init_hello_4 );
module_exit ( cleanup_hello_4 );

/*
 * You can use strings kije this
 */

/*
 * Get rid of taint messages by declaring code as GPL
 */
MODULE_LICENSE ( "GPL" );

/*
 * Or with defines like this
 */
MODULE_AUTHOR ( DRIVER_AUTHOR );
MODULE_DESCRIPTION ( DRIVER_DESC );

/*
 * This module uses /dev/testdevice. The MODULE_SUPPORTED_DEVICE macro might be used
 * in the future to help automatic configuration of modules, but is currently unused
 * other than for documentation purposes
 */
MODULE_SUPPORTED_DEVICE ( "testdevice" );
