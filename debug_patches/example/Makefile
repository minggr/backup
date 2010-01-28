obj-m=simple.o

#KERNELDIR ?= /lib/modules/$(shell uname -r)/build
KERNELDIR=~/build
PWD=$(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -rf *.o *~ core .depend .*.cmd simple.ko *.mod.c .tmp_versions modules.order  Module.symvers

