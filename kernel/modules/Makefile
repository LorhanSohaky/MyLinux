obj-m += my_device_pi.o

PWD=$(shell pwd)

all:
	@echo $(PWD)
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C /home/bcc/740951/linux M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build clean
