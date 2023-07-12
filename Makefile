obj-m += fingerprint.o
PWD = $(shell pwd)

default: modules

modules:
	pkexec make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

install: modules
	pkexec make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules_install

clean:
	pkexec make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test: test.c
	gcc test.c -o test

test_and_run: test
	./test
