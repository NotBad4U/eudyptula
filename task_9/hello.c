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

#define PERM_rw_r__r__	0644
#define PERM_r__r__r__	0444
#define PERM_rw_rw_rw_	0666

static int data_len;
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
	return sprintf(buf, "%lu", jiffies);
}

static ssize_t jiffies_store(struct kobject *kobj, struct kobj_attribute *attr,
		       const char *buf, size_t count)
{
	// Should never happend
	return -EINVAL;
}

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
	return 0;
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{
	return 0;
}

static struct kobj_attribute id_attribute =
	__ATTR(id, 0664, id_show, id_store);

static struct kobj_attribute jiffies_attribute =
	__ATTR(jiffies, 0664, jiffies_show, jiffies_store);

static struct kobj_attribute foo_attribute =
	__ATTR(foo, 0664, foo_show, foo_store);


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
