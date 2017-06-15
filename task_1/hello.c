#include <linux/module.h>
#include <linux/init.h>

MODULE_AUTHOR("03fe7baeaee0 lightplay8@gmail.com");
MODULE_DESCRIPTION("This is Task 01 of the Eudyptula Challenge");


static int __init hello_module_init(void) 
{
    printk(KERN_DEBUG "Hello World!\n");
    return 0;
}

static void __exit hello_module_exit(void) {
    /* Do nothing special */
}

module_init(hello_module_init);
module_exit(hello_module_exit);