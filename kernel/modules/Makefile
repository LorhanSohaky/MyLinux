obj-m += hello.o
PWD=$(shell pwd)

all:
	@echo $(PWD)
	make -C ../linux-5.3.2 M=$(PWD) modules

clean:
	make -C ../linux-5.3.2 clean
