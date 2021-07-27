obj-m += hello.o
CURRENT_PATH := $(shell pwd)
# LINUX_KERNEL := $(shell uname -r)
# LINUX_KERNEL_PATH := /usr/src/linux-headers-$(LINUX_KERNEL)
LINUX_KERNEL_PATH := /home/mrh929/study/kernel/linux-4.14.239
all:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) modules
clean:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) clean
