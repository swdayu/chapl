ifneq ($(filter native,$(CONFIG_COMPILER)),)
pre-gen-obj-y := conf/pretypes.o
pre-gen-file-y := conf/pretypes.genfile
endif

obj_c += builtin/decl.c test/decl.c
obj_c += chcc/cifa.c chcc/yufa.c test/chcc.c
obj_c += test/main.c

ifneq ($(filter x86,$(CONFIG_COMPILER)),)
#obj_c += test/elf_x86.c
else ifneq ($(filter x64,$(CONFIG_COMPILER)),)
#obj_c += test/elf_x64.c
endif

obj-y += direct/ $(obj_c:.c=.o)

ifneq ($(filter msc,$(CONFIG_COMPILER)),)
ifneq ($(filter x86,$(CONFIG_COMPILER)),)
# obj_asm += test/x86.asm
else ifneq ($(filter x64,$(CONFIG_COMPILER)),)
#obj_asm += test/x64.asm
endif
#obj-y += $(obj_asm:.asm=.o)
endif

incdir-y += -Isrc/lang
