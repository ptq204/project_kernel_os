#include <linux/module.h>
#include <linux/random.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>

#define DRIVER_AUTHOR "Pham The Quyen <ptquyen@apcs.vn>, Nguyen Quoc Viet <>"
#define DRIVER_DESC "Random generator loadable kernel project"

static dev_t first;

static int __init init_random(void){

	int i;
	if (alloc_chrdev_region(&first, 0, 3, "Shweta") < 0)
	{
		return -1;
	}
	get_random_bytes(&i, sizeof(int));
	printk("Random number: %d\n", i);
	printk(KERN_INFO "<Major, Minor>: <%d, %d>\n", MAJOR(first), MINOR(first));
	return 0;
}

static void __exit exit_random(void){
	
	unregister_chrdev_region(first, 3);
	printk("Exit random generator module\n");
}

module_init(init_random);
module_exit(exit_random);
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_SUPPORTED_DEVICE("testdevice");

