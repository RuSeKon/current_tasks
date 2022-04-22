obj-m += hello_comnd_ln_arg.o
obj-m = startstop.o
startstop-objs = start.o stop.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
