ifneq ($(filter msc,$(CONFIG_COMPILER)),)
ifneq ($(filter fasm,$(CONFIG_COMPILER)),)
	ifneq ($(filter x64,$(CONFIG_COMPILER)),)
	obj-y += assembly/gms_x64.o
	else
	obj-y += assembly/gms_x86.o
	endif
else
	ifneq ($(filter x64,$(CONFIG_COMPILER)),)
	obj-y += assembly/msc_x64.o
	else
	obj-y += assembly/msc_x86.o
	endif
endif
else ifneq ($(filter gnu,$(CONFIG_COMPILER)),)
ifneq ($(filter fasm,$(CONFIG_COMPILER)),)
	ifneq ($(filter x64,$(CONFIG_COMPILER)),)
	obj-y += assembly/gnu_x64.o
	else
	obj-y += assembly/gnu_x86.o
	endif
else
	ifneq ($(filter x64,$(CONFIG_COMPILER)),)
	obj-y += assembly/gcc_x64.o
	else
	obj-y += assembly/gcc_x86.o
	endif
endif
else
	$(error "unsupported assembler")
endif

ccflags-y += -Iprh
