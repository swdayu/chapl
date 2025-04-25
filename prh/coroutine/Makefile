ifneq ($(filter msc,$(CONFIG_COMPILER)),)
ifneq ($(filter x64,$(CONFIG_COMPILER)),)
obj-y += coro_msc_x64.o
else
obj-y += coro_msc_x86.o
endif
else
ifneq ($(filter x64,$(CONFIG_COMPILER)),)
obj-y += coro_gnu_x64.o
else
obj-y += coro_gnu_x86.o
endif
endif

ccflags-y += -Iprh
