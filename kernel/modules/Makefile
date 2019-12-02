obj-m += my_device.o my_device_pi.o

PWD=$(shell pwd)

all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C ../linux M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build clean
