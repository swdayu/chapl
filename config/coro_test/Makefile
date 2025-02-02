obj-asm := ../../src/lang/chcc/x64/x64.asm

obj-c := \
	../../src/lang/builtin/decl.c \
	../../src/lang/builtin/coro.c \
	main.c

obj-y += $(obj-asm:.asm=.o) $(obj-c:.c=.o)

incdir-y += -Isrc/lang
