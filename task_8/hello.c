#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>
#include <linux/semaphore.h>
#include <linux/errno.h>


#define MY_ID "03fe7baeaee0"
#define MY_ID_LEN 13

#define PERM_rw_r__r__	0644
#define PERM_r__r__r__	0444
#define PERM_rw_rw_rw_	0666

DEFINE_SEMAPHORE(foo);

// This directory entry will point to /sys/kernel/debug/eudyptula
static struct dentry *eudyptula_dir;

static int data_len;
static char data[PAGE_SIZE];

static ssize_t foo_read(struct file *f, char *buffer,
			size_t buffer_size, loff_t *loff)
{
	ssize_t res = 0;

	down(&foo);
	res = simple_read_from_buffer(buffer, buffer_size, loff, data, data_len);
	up(&foo);

	return res;
}

static ssize_t foo_write(struct file *f, const char *buffer,
			size_t buffer_size, loff_t *loff)
{
	ssize_t res = 0;

	if (buffer_size > PAGE_SIZE)
		buffer_size = PAGE_SIZE;

	down(&foo);
	res = simple_write_to_buffer(data, PAGE_SIZE, loff, buffer, buffer_size);

	if (res > 0)
		data_len = res;

	up(&foo);

	return res;
}

static const struct file_operations foo_ops = {
	.owner = THIS_MODULE,
	.read = foo_read,
	.write = foo_write,
};

static ssize_t id_read(struct file *f, char *buffer,
			size_t buffer_size, loff_t *loff)
{
	return simple_read_from_buffer(buffer, buffer_size, loff, MY_ID, MY_ID_LEN);
}

static ssize_t id_write(struct file *f, const char *buffer,
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

static const struct file_operations id_ops = {
	.owner = THIS_MODULE,
	.read = id_read,
	.write = id_write,
};

static int __init hello_init(void)
{
	// If parent is null, then directory is created in root of debugfs.
	eudyptula_dir = debugfs_create_dir("eudyptula", 0);

	if (!eudyptula_dir)
		return -ENOENT;


	if (!debugfs_create_file("id", PERM_rw_rw_rw_, eudyptula_dir, NULL, &id_ops) ||
		!debugfs_create_u64("jiffies", PERM_r__r__r__, eudyptula_dir, (u64 *)&jiffies) ||
		!debugfs_create_file("foo", PERM_rw_r__r__, eudyptula_dir, NULL, &foo_ops)) {

		debugfs_remove_recursive(eudyptula_dir);
		return -ENOENT;
	}

	pr_debug("Hello World!\n");
	return 0;
}

static void __exit hello_exit(void)
{
	debugfs_remove_recursive(eudyptula_dir);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("03fe7baeaee0");
MODULE_DESCRIPTION("task 8");
