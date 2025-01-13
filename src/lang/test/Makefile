obj-c += decl.c chcc.c main.c

obj-y += $(obj-c:.c=.o)

ifneq ($(filter x86,$(CONFIG_COMPILER)),)
# obj-c += test/elf_x86.c
else ifneq ($(filter x64,$(CONFIG_COMPILER)),)
# obj-c += test/elf_x64.c
endif

ifneq ($(filter msc,$(CONFIG_COMPILER)),)
ifneq ($(filter x86,$(CONFIG_COMPILER)),)
# obj-asm += test/x86.asm
else ifneq ($(filter x64,$(CONFIG_COMPILER)),)
# obj-asm += test/x64.asm
endif
# obj-y += $(obj-asm:.asm=.o)
endif
