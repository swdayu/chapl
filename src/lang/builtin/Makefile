ifneq ($(filter native,$(CONFIG_COMPILER)),)
pre-gen-obj-y := ../conf/pretypes.o
pre-gen-file-y := ../conf/pretypes.genfile
endif

obj-c += decl.c

obj-y += $(obj-c:.c=.o)

incdir-y += -Isrc/lang
