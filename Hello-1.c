#include <linux/module.h>	/* Needed by all modules */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/kernel.h> 	/* Needed for KERN_INFO */

static int __init hello_init (void )
{
    printk ( KERN_ALERT "TEST: Hello Gerry 1\n" );

    return 0;
}

static void __exit hello_exit ( void )
{
    printk ( KERN_ALERT "TEST: Good-bye 1\n" );
}

module_init ( hello_init );
module_exit ( hello_exit);
