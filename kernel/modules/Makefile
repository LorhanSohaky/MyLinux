obj-m += hello.o led.o
PWD=$(shell pwd)

all:
	@echo $(PWD)
	make -C ../linux-5.3.2 M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build clean
