obj-m += hello.o led.o blink.o simple_led.o

PWD=$(shell pwd)

all:
	@echo $(PWD)
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build clean
