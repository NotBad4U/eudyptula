#include <linux/usb.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hid.h>

static const struct usb_device_id usb_kbd_id_table[] = {
	{
		USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID,
		USB_INTERFACE_SUBCLASS_BOOT,
		USB_INTERFACE_PROTOCOL_KEYBOARD)
	},
	{}
};

MODULE_DEVICE_TABLE(usb, usb_kbd_id_table);

static int __init hello_init(void)
{
	pr_info("Hello USB World!\n");
	return 0;
}

static void __exit hello_exit(void)
{
	pr_info("Remove hello.ko module\n");
}


module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("03fe7baeaee0");
MODULE_DESCRIPTION("usb driver task 5");
