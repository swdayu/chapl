ifneq ($(filter msc,$(CONFIG_COMPILER)),)
obj-c := wapi/file.c
endif

obj-y += $(obj-c:.c=.o)

incdir-y += -Isrc/lang
