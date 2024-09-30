cur_makefile := $(lastword $(MAKEFILE_LIST))
$(cur_makefile):: ;

.DEFAULT_GOAL := all
PHONY := all
all:

# dont use make's built-in rules and variables
# (this increases performance and avoids hard-to-debug behaviour)
MAKEFLAGS += -rR

# add makefile search directory relative to the root of source
MAKEFLAGS += --include-dir=$(CURDIR)

# dont print "entering directory ..."
MAKEFLAGS += --no-print-directory

# avoid funny character set dependencies
unexport LC_ALL
LC_COLLATE=C
LC_NUMERIC=C
export LC_COLLATE LC_NUMERIC

# avoid interference with shell env settings
unexport GREP_OPTIONS

# echo level 0 quiet 1 raw 2 raw-why 3 verbose or -s silent
BUILD_VERBOSE := 0
quiet := quiet_
Q     := @
ifeq ($(origin V),command line)
BUILD_VERBOSE = $(V)
endif
ifneq ($(BUILD_VERBOSE),0)
quiet :=
Q     :=
endif
ifneq ($(filter 4.%,$(MAKE_VERSION)),) # make-4
ifneq ($(filter %s ,$(firstword x$(MAKEFLAGS))),)
quiet := silent_
BUILD_VERBOSE := 0
endif
else # make-3.8x
ifneq ($(filter s% -s%,$(MAKEFLAGS)),)
quiet := silent_
BUILD_VERBOSE := 0
endif
endif

export Q
export quiet
export BUILD_VERBOSE
export srctree := $(CURDIR)
export BUILD_OUTPUT := $(CURDIR)/out

include scripts/Makefile.include

TARGET_INFO := $(T)
ifneq ($(CODE_REV_INFO),)
TARGET_INFO := ($(CODE_REV_INFO)) $(TARGET_INFO)
endif

ifeq ($(filter help clean allclean,$(MAKECMDGOALS)),)
ifneq ($(T),)

ifeq ($(filter-out %/ %$(backslash),$(T)),)
$(error target can NOT contain slash T=$(T))
endif

ifeq ($(wildcard config/$(T)/Makefile.target),)
$(error target NOT exist config/$(T))
endif

make_start_time := $(get_time)
make_start_date_time := $(get_date_time)

$(info MAKE START: $(make_start_date_time))
$(info MAKE TARGET: $(TARGET_INFO)$(if $(SOFTWARE_VERSION), v$(SOFTWARE_VERSION)))

TARGET_CCCFGS_FILE := config/$(T)/Makefile.cccfgs
ifeq ($(wildcard $(TARGET_CCCFGS_FILE)),)
$(error Makefile.cccfgs not provided)
else
include $(TARGET_CCCFGS_FILE)
$(TARGET_CCCFGS_FILE):: ;
endif

ifeq ($(cccfg-y),)
$(error compiler config not available)
endif

PHONY += $(cccfg-y)
$(cccfg-y):
	$(call echo-info,CCCFG:$@)
	$(Q)$(MAKE) -f $(srctree)/scripts/Makefile.start CCCFG=$@ $(filter show lst,$(MAKECMDGOALS)) all
	$(call echo-info,)

# execute each cccfg in sequence
PHONY += not_parallel
not_parallel: $(cccfg-y)
.NOTPARALLEL: not_parallel

all: not_parallel
	$(call show_make_end_time,$(make_start_time))
endif
endif

PHONY += show
show:
	$(call echo-info,Environment:)
	$(call echo-info,  HOST=$(MAKE_HOST))
	$(call echo-info,  SHELL=$(SHELL))
	$(call echo-info,  MAKE=$(MAKE))
	$(call echo-info,  MAKE_VERSION=$(MAKE_VERSION))
	$(call echo-info,  FEATURES=$(.FEATURES))
	$(call echo-info,  CURDIR=$(CURDIR))
	$(call echo-info,  OUTPUT=$(BUILD_OUTPUT)/$(T))
	$(call echo-info,  MAKEFLAGS=$(MAKEFLAGS))
	$(call echo-info,  MAKECMDGOALS=$(MAKECMDGOALS))
	$(call echo-info,  BUILD_VERBOSE=$(BUILD_VERBOSE) $(quiet) $(Q))
	$(call echo-info,  TARGET=$(T))

PHONY += help
help:
	$(call echo-info,Usage: make T=target [V=n] [options] [goals])
	$(call echo-info,  T=target      Select a target with config to build)
	$(call echo-info,  V=0           Default echo level - quiet mode)
	$(call echo-info,  V=1           Raw echo level - display raw long command)
	$(call echo-info,  V=2           Raw-Why echo level - also display rebuild reason)
	$(call echo-info,  V=3           Verbose echo level - also enable tool verbose)
	$(call echo-info,Opitons:)
	$(call echo-info,  -s            Make option to enable silent echo mode)
	$(call echo-info,  -j[n]         Make option to enable parallel build)
	$(call echo-info,Goals:)
	$(call echo-info,  show          Show information about the environment)
	$(call echo-info,  help          Display this informaiton)
	$(call echo-info,  all           Make all things for the target)
	$(call echo-info,  lst           Make lst file for the final image)
	$(call echo-info,  clean         Clean the specified target files)
	$(call echo-info,  allclean      Clean the entire output directory)

PHONY += clean
ifneq ($(T),)
clean:
	$(call CMDRMDIR,$(BUILD_OUTPUT)/$(T))
else
clean:
	@:
endif

PHONY += allclean
allclean:
	$(call CMDRMDIR,$(BUILD_OUTPUT))

define make_missing_goal
PHONY += $(1)
$(1):
	@:
endef

$(foreach g,$(filter-out $(PHONY),$(MAKECMDGOALS)),$(eval $(call make_missing_goal,$(g))))

.PHONY: $(PHONY)
