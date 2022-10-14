# Makefile : gmake file for building the LPC2xxx Device Library v2.0
#            device library proper.
#
# Author: Tymm Twillman <tymm@gmail.com>
# Date:   29. December 2011

# Prevent warnings about overriding commands...
LPC2XXX_RECURSE=1


# Check to see if we were passed a "top directory"
ifneq ("$(origin T)", "command line")
  T := $(dir $(CURDIR))
endif

# Include required compiler / environment settings
include $(T)/LPC2xxx.mk

# Set the search path...
vpath %.c $(T)/src
vpath %.s $(T)/src
vpath %.h $(T)/inc

# If called from out of the tree, run make from the directory
#  the build was called from, so output files end up there.
ifeq ("$(origin O)", "command line")
  BUILD_OUTPUT := $(O)
endif

ifneq ("$(origin T)", "command line")
ifneq ($(BUILD_OUTPUT),)

PHONY += $(MAKECMDGOALS) sub-make

$(filter-out _all sub-make $(CURDIR)/Makefile, $(MAKECMDGOALS)) _all: sub-make
	@:

sub-make:
	$(MAKE) -C $(BUILD_OUTPUT) T=$(dir $(CURDIR)) -f $(CURDIR)/Makefile \
	           $(filter-out _all sub-make,$(MAKECMDGOALS))

skip-makefile := 1
endif
endif

ifeq ($(skip-makefile),)

# Standard archive creation flags for GNU ar
ARFLAGS := $(if $(ARFLAGS),$(ARFLAGS),-urls)


# Dependencies / object files for the library
libLPC2xxx_SRC := LPC2xxx_rtc.c LPC2xxx_pll.c system_LPC2xxx.c \
                  LPC2xxx_lib_assert.c
libLPC2xxx_OBJ := $(libLPC2xxx_SRC:.c=.o) LPC2xxx_crt0.o


.PHONY: all

all: libLPC2xxx.a

libLPC2xxx.a: $(libLPC2xxx_OBJ)
	$(AR) $(ARFLAGS) $@ $^

.PHONY: clean

clean:
	rm -f $(libLPC2xxx_OBJ) libLPC2xxx.a


endif # ifeq ($(skip-makefile),)

