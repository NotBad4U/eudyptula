#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/semaphore.h>
#include <linux/string.h>


#define MY_ID "03fe7baeaee0"
#define MY_ID_LEN 14

static ssize_t data_len;
static char data[PAGE_SIZE];

// This directory entry will point to /sys/kernel/eudyptula
static struct kobject *eudyptula_kobj;

DEFINE_SEMAPHORE(foo);

static ssize_t id_show(struct kobject *kobj, struct kobj_attribute *attr,
		      char *buf)
{
	strncpy(buf, MY_ID, MY_ID_LEN);
	return MY_ID_LEN;
}

static ssize_t id_store(struct kobject *kobj, struct kobj_attribute *attr,
		       const char *buf, size_t count)
{
	char *my_id = MY_ID;

	if ((count != MY_ID_LEN) || (strncmp(my_id, buf, MY_ID_LEN))) {
		pr_info("That doesn't match the ID: %s", MY_ID);
		return -EINVAL;
	}

	pr_info("That match the ID: %s", MY_ID);
	return MY_ID_LEN;
}

static ssize_t jiffies_show(struct kobject *kobj, struct kobj_attribute *attr,
		      char *buf)
{
	return sprintf(buf, "%lu\n", jiffies);
}

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
	down(&foo);
	strncpy(buf, data, data_len);
	up(&foo);

	return data_len;
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{
	if (count > PAGE_SIZE)
		count = PAGE_SIZE;

	down(&foo);

	data_len = count;
	strncpy(data, buf, count);

	up(&foo);

	return count;
}

static struct kobj_attribute id_attribute = __ATTR_RW(id);

static struct kobj_attribute jiffies_attribute = __ATTR_RO(jiffies);

static struct kobj_attribute foo_attribute = __ATTR_RW(foo);


static struct attribute *attrs[] = {
	&id_attribute.attr,
	&jiffies_attribute.attr,
	&foo_attribute.attr,
	NULL,
};

static struct attribute_group attr_group = {
	.attrs = attrs,
};

static int __init hello_init(void)
{
	int retval;

	eudyptula_kobj = kobject_create_and_add("eudyptula", kernel_kobj);

	if (!eudyptula_kobj)
		return -ENOMEM;

	retval = sysfs_create_group(eudyptula_kobj, &attr_group);
	if (retval)
		kobject_put(eudyptula_kobj);

	return retval;
}

static void __exit hello_exit(void)
{
	kobject_put(eudyptula_kobj);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("03fe7baeaee0");
MODULE_DESCRIPTION("task 9");
