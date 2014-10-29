/*
 * Hello-5.c - Demonstrates command line argument passing to a module
 */
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>

MODULE_LICENSE( "GPL" );
MODULE_AUTHOR ( "Gerry Smyth" );

/* Add in some variables here */
static short int myshort = 1;
static int myint = 420;
static long int mylong = 9999;
static char *mystring = "Blues";
static int myintArray[2] = { -1, 1 };
static int arr_argc = 0;

/*
 * module_param ( foo, int, 0000 )
 * The first param is the parameters name
 * The second param is its data type
 * The final argument is its permissions bit
 * for exposing parameters in sysfs (if non-zero) at a later date.
 */

module_param ( myshort, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP );
MODULE_PARM_DESC ( myshort, "A Short integer" );

module_param ( myint, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH );
MODULE_PARM_DESC ( myint, "An integer" );

module_param ( mylong, long, S_IRUSR );
MODULE_PARM_DESC ( mylong, "A long integer" );

module_param ( mystring, charp, 0000 );
MODULE_PARM_DESC ( mystring, "A character string" );

/*
 * module_param_array ( name, type, num, perm )
 * The first param is the parameter's (in this case the array's) name
 * The second param is the data type of the elements of the array
 * The third argument is a pointer to the variable that will store the number
 * of elements of the array initialised by the user at module loading time
 * The 4th argument is the permission bits
 */
module_param_array ( myintArray, int, &arr_argc, 0000 );
MODULE_PARM_DESC ( myintArray, "An array of integers" );

static int __init hello5_init ( void )
{
    int		i;
    int		arraySize;

    printk ( KERN_INFO "Hello world 5 ============\n" );

    printk ( KERN_INFO "myshort is short [%hd]\n", myshort );
    printk ( KERN_INFO "myint is int [%d]\n", myint );
    printk ( KERN_INFO "mylong is long [%ld]\n", mylong );
    printk ( KERN_INFO "mystring is string [%s]\n", mystring );

    arraySize = ( sizeof myintArray / sizeof ( int ));
    for ( i = 0; i < arraySize; i++ )
    {
        printk ( KERN_INFO "myintArray[%d] = %d\n", i, myintArray[i] );
        printk ( KERN_INFO "Getting %d args for myintArray.\n", arr_argc );
    }
     printk ( KERN_INFO "Got %d args for myintArray.\n", arr_argc );

    return 0;
}


static void __exit hello5_exit ( void )
{
    printk ( KERN_INFO "Goodbye world 5\n" );
}

module_init ( hello5_init );
module_exit ( hello5_exit );
