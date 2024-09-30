ifneq ($(filter native,$(CONFIG_COMPILER)),)
pre-gen-obj-y := conf/pretypes.o
pre-gen-file-y := conf/pretypes.genfile
endif

obj-c += \
	builtin/decl.c \
	test/builtin.c \
	test/main.c

obj-y += $(obj-c:.c=.o)

incdir-y += -Isrc/lang
