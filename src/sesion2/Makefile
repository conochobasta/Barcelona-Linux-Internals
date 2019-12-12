obj-m+=hello-1.o
//obj-m+=hello-2.o
//obj-m+=hello-3.o
//obj-m+=hello-5.o
//obj-m+=netfilter_LKM.o
//obj-m+=kcores.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	rm -r -f *.mod.c .*.cmd *.symvers *.o
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
