#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include <linux/errno.h>


#define NAME_MODULE "eudyptula"
#define BUFFER_LEN 80

#define MY_ID "03fe7baeaee0"
#define MY_ID_LEN 13	/* ID length including the final NULL */

static int Major;

static ssize_t my_read(struct file *f, char *buffer,
			size_t buffer_size, loff_t *loff)
{
	char *my_id = MY_ID;

	pr_info("Return my assign Id: %s", MY_ID);
	copy_to_user(buffer, my_id, MY_ID_LEN);

	*loff += buffer_size;

	return MY_ID_LEN;
}

static ssize_t my_write(struct file *f, const char *buffer,
			size_t buffer_size, loff_t *loff)
{
	char *my_id = MY_ID;
	char tmp[MY_ID_LEN];

	if (buffer_size != MY_ID_LEN) {
		pr_info("The user send more data than expected");
		return -EINVAL;
	}

	if (copy_from_user(tmp, buffer, MY_ID_LEN)) {
		pr_info("Can\'t read the send by the user");
		return -EINVAL;
	}

	if (strncmp(my_id, buffer, MY_ID_LEN - 1)) {
		pr_info("That doesn't match the ID: %s", MY_ID);
		return -EINVAL;
	}

	pr_info("That match the ID: %s", MY_ID);
	return buffer_size;
}

static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = my_read,
	.write = my_write,
};

/**
 *Use this structure for allocate a dynamic minor number at the device
 *The major number is always 10 (see the notes for more information)
 */
static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = NAME_MODULE,
	.fops = &fops,
};

static int __init hello_init(void)
{
	Major = misc_register(&misc);

	if (Major < 0) {
		pr_err("Error for register %s module with Major %d",
		NAME_MODULE, Major);
		return Major;
	}

	// task 0 :)
	pr_info("Hello World!\n");
	return 0;
}

static void __exit hello_exit(void)
{
	pr_info("remove %s module\n", NAME_MODULE);
	misc_deregister(&misc);
}


module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("03fe7baeaee0");
MODULE_DESCRIPTION("char module task 6");
