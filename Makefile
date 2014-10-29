obj-m += Hello.o
obj-m += Hello-1.o
obj-m += Hello-3.o
obj-m += Hello-4.o
obj-m += Hello-5.o
obj-m += gpscdev.o
obj-m += startstop.o
startstop-objs := start.o stop.o
obj-m += chardev.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
