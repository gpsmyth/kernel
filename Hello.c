#include <linux/init.h>
#include <linux/module.h> /* Needed by all modules */

static int hello_init (void )
{
    printk ( KERN_ALERT "TEST: Hello Gerry\n" );

    return 0;
}

static void hello_exit ( void )
{
    printk ( KERN_ALERT "TEST: Good-bye\n" );
}

module_init ( hello_init );
module_exit ( hello_exit);
