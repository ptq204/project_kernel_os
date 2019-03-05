#include <linux/module.h>
#include <linux/random.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define DRIVER_AUTHOR "Pham The Quyen <ptquyen@apcs.vn>, Nguyen Quoc Viet <nqviet@apcs.vn>"
#define DRIVER_DESC "Random generator loadable kernel project"

#define DEVICE_NAME "random_generator"
#define DEVICE_CLASS "random_generator_class"

static dev_t first;
static int numOpens = 0;
static struct cdev c_dev;
static int majorNumber;
static struct class* randClass = NULL; // device class variable


// Prototype functions for character driver
static int     device_open(struct inode*, struct file*);
//static int     device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
//static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

// REFERENCE: https://www.tldp.org/LDP/lkmpg/2.4/html/c577.html
// Associate callback functions with file operations
struct file_operations fops = {
       read: device_read,
       //write: device_write,
       open: device_open,
       //release: device_release
};

static int __init init_random(void){

	majorNumber = alloc_chrdev_region(&first, 0, 3, DEVICE_NAME);
	if (majorNumber < 0)
	{
		return -1;
	}

	randClass = class_create(THIS_MODULE, DEVICE_CLASS);
	if(IS_ERR(randClass)){
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_INFO "Registered device class fail");
		return -1;
	}
	printk(KERN_INFO "Registered device class successfully");
	
	if (device_create(randClass, NULL, first, NULL, "mynull") == NULL)
	{
		class_destroy(randClass);
		unregister_chrdev_region(first, 1);
		return -1;
	}
	printk(KERN_INFO "Registered device successfully");
	
	cdev_init(&c_dev, &fops);
	if (cdev_add(&c_dev, first, 1) == -1)
	{
		device_destroy(randClass, first);
		class_destroy(randClass);
		unregister_chrdev_region(first, 1);
		return -1;
	}

	printk(KERN_INFO "<Major, Minor>: <%d, %d>\n", MAJOR(first), MINOR(first));
	return 0;
}

static int device_open(struct inode* inode, struct file* file){

	return 0;
}

static ssize_t device_read(struct file *file, char *c, size_t size, loff_t *loff_t){
	
	int i;
	char buf[60];
	if(*loff_t > 3){
	
		get_random_bytes(&i, sizeof(int));
		printk("Random number: %d\n", i);
		
		sprintf(buf, "%d", i);
		if(copy_to_user(c, buf, sizeof(buf))){
			return sizeof(buf);
		}
		return -1;
	}
	
	return -1;
}

static void __exit exit_random(void){
	
	cdev_del(&c_dev);
	device_destroy(randClass, first);
	class_destroy(randClass);
	unregister_chrdev_region(first, 3);
	printk("Exit random generator module\n");
}

module_init(init_random);
module_exit(exit_random);
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_SUPPORTED_DEVICE("testdevice");
