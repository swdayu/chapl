ifneq ($(filter msc,$(CONFIG_COMPILER)),)
ifneq ($(filter fasm,$(CONFIG_COMPILER)),)
	ifneq ($(filter x64,$(CONFIG_COMPILER)),)
	obj-y += prh_msmx64.o
	else
	obj-y += prh_msmx86.o
	endif
else
	ifneq ($(filter x64,$(CONFIG_COMPILER)),)
	obj-y += prh_mscx64.o
	else
	obj-y += prh_mscx86.o
	endif
endif
else ifneq ($(filter gnu,$(CONFIG_COMPILER)),)
ifneq ($(filter fasm,$(CONFIG_COMPILER)),)
	ifneq ($(filter x64,$(CONFIG_COMPILER)),)
	obj-y += prh_gsmx64.o
	else
	obj-y += prh_gsmx86.o
	endif
else
	ifneq ($(filter x64,$(CONFIG_COMPILER)),)
	obj-y += prh_gnux64.o
	else
	obj-y += prh_gnux86.o
	endif
endif
else
	$(error "unsupported assembler")
endif

ccflags-y += -Iprh
