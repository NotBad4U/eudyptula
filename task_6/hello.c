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

static ssize_t my_read(struct file *f, char *buffer,
			size_t buffer_size, loff_t *loff)
{
	return simple_read_from_buffer(buffer, buffer_size, loff, MY_ID, MY_ID_LEN);
}

static ssize_t my_write(struct file *f, const char *buffer,
			size_t buffer_size, loff_t *loff)
{
	char *my_id = MY_ID;
	char tmp[MY_ID_LEN];
	ssize_t ret_write;

	ret_write = simple_write_to_buffer(tmp, MY_ID_LEN, loff, buffer, buffer_size);

	if (ret_write < 0)
		return ret_write;

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
	return misc_register(&misc);
}

static void __exit hello_exit(void)
{
	misc_deregister(&misc);
}


module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("03fe7baeaee0");
MODULE_DESCRIPTION("char module task 6");
