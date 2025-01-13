ifneq ($(filter native,$(CONFIG_COMPILER)),)
pre-gen-obj-y := conf/pretypes.o
pre-gen-file-y := conf/pretypes.genfile
endif

obj-y += direct/ builtin/ test/

subdir-incdir-y += -Isrc/lang
