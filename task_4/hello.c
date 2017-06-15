#include <linux/module.h>
#include <linux/init.h>

static __init int hello_init(void)
{
    pr_debug("Hello World!\n");
    return 0;
}

static __exit void hello_exit(void)
{
    return;
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("03fe7baeaee0 lightplay8@gmail.com");
MODULE_DESCRIPTION("This is Task 01 of the Eudyptula Challenge");
