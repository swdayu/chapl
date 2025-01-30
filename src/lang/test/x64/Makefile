ifneq ($(filter msc,$(CONFIG_COMPILER)),)
obj-asm := x64.asm
obj-y += $(obj-asm:.asm=.o)
else
obj-s := x64.s
obj-y += $(obj-s:.s=.o)
endif
