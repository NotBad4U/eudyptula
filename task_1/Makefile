KERNEL_SRC_DIR := /lib/modules/$(shell uname -r)/build

obj-m += hello.o

all:
	make -C $(KERNEL_SRC_DIR) M=$(PWD) modules

clean:
	make -C $(KERNEL_SRC_DIR) M=$(PWD) clean