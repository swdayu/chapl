ifneq ($(filter native,$(CONFIG_COMPILER)),)
pre-gen-obj-y := conf/pretypes.o
pre-gen-file-y := conf/pretypes.genfile
endif

obj_c += builtin/decl.c test/decl.c
obj_c += chcc/cifa.c test/chcc.c
obj_c += test/main.c

obj-y += $(obj_c:.c=.o)

incdir-y += -Isrc/lang
