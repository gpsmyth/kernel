/*
 * Illustrating the __init, __initdata and __exit macros
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/kernel.h> 	/* Needed for KERN_INFO */

static int hello3_data __initdata = 3;

static int __init hello3_init (void )
{
    printk ( KERN_INFO "TEST: Hello Gerry %d\n",  hello3_data );

    return 0;
}

static void __exit hello3_exit ( void )
{
    printk ( KERN_INFO "TEST: Good-bye 3\n" );
}

module_init ( hello3_init );
module_exit ( hello3_exit);
