/*
 * gpscdev.c: Create a char device that says how many times
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>	/* copy to user and copy from user */
#include <linux/semaphore.h>
#include <linux/cdev.h>

/*
 * (1) Create a structure for our fake device
 */
struct file_device
{
	char	data[100];
	struct	semaphore	sem;
} virtual_device;

/*
 * Global variables are declared as static, so are global within the file
*/
/*
 * (2) To later register our device we need a cdev object & some other variables
 */
struct	cdev	*mycdev;	// my char dev
int	major_number;		// extracted from dev_t using macro - mknod
int	ret;			// for return values of functions, because kernel stack is very small
				// so declaring variables all over the place in our modules eats up
				// the stack very fast
dev_t	dev_num;		// will hold major numbers that the kernel gives us
				// name ---> appears in /proc/devices
#define	DEVICE_NAME	"gpsDevice"	// Dev name as it appears in /proc/devices */
#define SUCCESS 0

/*
 * Prototypes - this would normally go into a .h file
 */
/*
static int device_open(struct inode *inode, struct file *file);
static int device_release(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *file, char __user *buf,
                         size_t len, loff_t *ppos);
static ssize_t device_write(struct file *file, const char __user *buf,
                          size_t len, loff_t *ppos);
*/
int device_open(struct inode *inode, struct file *file);
int device_release(struct inode *inode, struct file *file);
ssize_t device_read(struct file *file, char __user *buf,
                         size_t len, loff_t *ppos);
ssize_t device_write(struct file *file, const char __user *buf,
                          size_t len, loff_t *ppos);

// (6) Tell the kernel which fns to call when user operates on our device file
// file_operations is a large file in fs.h
struct file_operations fops =
{
	.owner = THIS_MODULE,	// prevent unloading of this module when
				// operations are in use
    	.open = device_open,	// points to method to call etc.. (callbacks)
    	.release = device_release,
    	.read = device_read,
    	.write = device_write
};

/*
 * This function is called when the module is loaded
 */
static	int __init driver_entry ( void )
{
	// (3) Register our device with the system: a 2-step process
	// Step 1 : use dynamic allocation to assign our device
	// extern int alloc_chrdev_region(dev_t *, unsigned, unsigned, const char *);
	ret = alloc_chrdev_region( &dev_num, 0, 1, DEVICE_NAME );

	if ( ret < 0 )
	{
		printk ( KERN_ALERT "gpscdev: Failed to allocate a major number" );
		return ret;	// propagate error
	}

	// extracts the major no and store in our variable (MACRO)
	major_number = MAJOR ( dev_num );
	printk ( KERN_INFO "gpscdev: major number is %d\n", major_number );
 
	// Step 2 : allocate our char dev structure
	// struct cdev *cdev_alloc(void);
	// struct cdev {
	//         struct kobject kobj;
	//         struct module *owner;
	//         const struct file_operations *ops;
	//         struct list_head list;
	//         dev_t dev;
	//         unsigned int count;
	//     };
	//
	mycdev = cdev_alloc();
	mycdev->ops = &fops;
	mycdev->owner = THIS_MODULE;
	// mycdev->dev = dev_num;

	// Now that we've created cdev, we have to add it into te kernel
	// int cdev_add(struct cdev *, dev_t, unsigned);
	ret = cdev_add ( mycdev, dev_num, 1 );
	if ( ret < 0 )
	{
		printk ( KERN_ALERT "gpscdev : unable to add cdev to the kernel\n" );
		return ret;
	}

	printk ( KERN_INFO "gpscdev: Perfromed cdev_add - ret is %d\n", ret );
	printk (KERN_INFO "gpscdev: Initialise our semaphore" );
	// (4) Initialise our semaphore
	sema_init ( &virtual_device.sem, 1 );	// initialise to value of 1 for init_MUTEX 
						// rather than init_MUTEX_LOCKED which is 0
	
	return SUCCESS;
}

/*
 * This function is called when the module is unloaded
 */
static void __exit driver_exit ( void )
{
	/*
 	 * (5) Unregister everything in reverse order
 	 * void cdev_del(struct cdev *);
 	 * extern void unregister_chrdev_region(dev_t, unsigned);
 	 */
	// a
	cdev_del ( mycdev );

	// b
	unregister_chrdev_region ( dev_num, 1 );
	printk ( KERN_INFO "gpscdev: unloaded module" );
}

/*
 * Methods
 */

/*
 * (7) Called when a process tries to open the device file
 * opens inode reference to file on disk & contains information on that file
 * struct represents an abstract open file
 */
int device_open(struct inode *inode, struct file *file)
{
	// allows one process to open this device by using a semaphore as mutal
	// exclusion lock-mutex
	if ( down_interruptible ( &virtual_device.sem ) != 0 )
	{
		printk ( KERN_ALERT "gpscdev: could not lock device during open" );
		return -1;
	}

	printk ( KERN_INFO "gpscdev: opened device" );

	return SUCCESS;
}

/*
 * Called wheen a process closes the device file
 */
int device_release(struct inode *inode, struct file *file)
{
	// by calling up, which is the opposite of down for semaphore,
	// we release that we obtained at device_open
	// This has the effect of allocating other process to use the device now
	up ( &virtual_device.sem );
	printk ( KERN_INFO "gpscdev: released device" );

	return SUCCESS;
}

/*
 * (8) Called when a process, which already opened the dev file, attempts to
 * read from it
 */
ssize_t device_read(struct file *file, char __user *buf,
                         size_t len, loff_t *ppos)
{
	// take data from the kernel space (device) --> user space (process)
	// copy_to_user (destn, src, sizeToTransfer );
	printk ( KERN_INFO "gpscdev: @ device_read called when the user wants to get info from the device" );
	ret = copy_to_user ( buf, virtual_device.data, len );
	return ret;
}

/*
 * (9) Called when the user wants to send information to the device
 */
ssize_t device_write(struct file *file, const char __user *buf,
                          size_t len, loff_t *ppos)
{
	// sned data from the user to the kernel
	// copy_from_user ( destn, src, len );
	ret = copy_from_user ( virtual_device.data, buf, len );
	printk ( KERN_INFO "gpscdev: @ device_write - called when the user wants to send info to the device.\n" );
	return ret;
}

module_init( driver_entry );
module_exit( driver_exit );
