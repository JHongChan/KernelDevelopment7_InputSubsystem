obj-m += event_module.o
all:
	make -C /home/sice/kj_gjx_embedd_base/lab_8/linux-3.5 M=`pwd` modules
	cp event_module.ko /opt/rootfs/lib/modules/3.5.0cyh/event_module.ko
clean:
	make -C /home/sice/kj_gjx_embedd_base/lab_8/linux-3.5 M=`pwd` modules clean
