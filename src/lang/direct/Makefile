ifneq ($(filter msc,$(CONFIG_COMPILER)),)
obj_c := windows/file_windows.c
else ifneq ($(filter gcc,$(CONFIG_COMPILER)),)
obj_c :=
else
$(error unsupported compiler '$(CONFIG_COMPILER)')
endif

# obj-y += $(obj_c:.c=.o)

subdir-incdir-y += -Isrc/direct -Isrc/lang
