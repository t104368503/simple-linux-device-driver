TARGET_MODULE:=ldd_hello_world
Device_MAJOR_ID:=60
Device_MINOR:=0

obj-m := $(TARGET_MODULE).o

ifeq ($(KERNELDIR),)
	KERNELDIR=/lib/modules/$(shell uname -r)/build
endif

       
all:
# run kernel build system to make module
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
# run kernel build system to cleanup in current directory    
	make -C $(KERNELDIR) M=$(PWD) clean


add:
	sudo mknod /dev/$(TARGET_MODULE) c 60 0 


load:
	sudo insmod ./$(TARGET_MODULE).ko 

unload:
	sudo rmmod ./$(TARGET_MODULE).ko 

