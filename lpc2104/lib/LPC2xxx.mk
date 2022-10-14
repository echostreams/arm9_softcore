# lpc2xxx.mk : gmake include file for use with the LPC2xxx Device Library v2.0
#
#  This file assumes use of Gmake (it uses GNU Make extensions) and gcc
#   (the GNU Compiler Collection)
#
#  This may be included in gmake files to set up compiler / linker / etc.
#   settings for building projects using the library.  Or feel free to
#   roll your own if this doesn't meet your needs.
#
# Make environment variables that this file needs:
#
#     CROSS_COMPILE (defaults to arm-eabi-)
#       This is the compiler prefix.  e.g. using the default, gcc for the
#       CPU is expected to be named "arm-eabi-gcc" and so on...)
#
#     MODEL (no default value; must be explicitly set)
#       This is the CPU model that the project is compiling for.
#       Examples would be lpc2103, lpc2216, etc.
#
#     F_CPU (no default value; must be explicitly set)
#       This is the system frequency at which the CPU will
#       run (NOT the external / internal oscillator speed).
#       e.g. 60000000L for 60 MHz operation.
#
#     HSE_Val (no default value; must be explicitly set)
#       This is the speed of an external crystal / oscillator
#       from which the CPU's internal (F_CPU) frequency will be derived
#       (generally 8Mhz, 12Mhz, etc.)
#
# Make environment variables that might be useful:
#
#     LPC2XXX_NO_INTERWORK
#       Set this to 2 to prevent "-mthumb-interwork" from being added
#       to the compiler's flags.
#
# I'm happy to take suggestions on making this all work better :/


# See if we were provided a "top level directory"; if so use that as the
#  library directory

# If not provided with top level, and not explicitly set,
ifeq ("$(LPC2XXXLIB_DIR)","")
ifeq ("$(origin T)", "command line")
  #LPC2XXXLIB_DIR := $(T)
  LPC2XXXLIB_DIR := $(T)
else
  LPC2XXXLIB_DIR := $(dir $(lastword $(MAKEFILE_LIST) ))
endif # ifeq ("$(origin T)", "command line")
endif # ifeq ("$(LPC2XXXLIB_DIR)","")


# Required GMake environment variables

# Set the architecture name & compiler / linker / etc. cross compile prefix.
CROSS_COMPILE   := $(if $(CROSS_COMPILE),$(CROSS_COMPILE),arm-none-eabi-)
CPU             := $(if $(CPU),$(CPU),arm7tdmi)

# If making the library, make sure that the necessary options have been set on
#  command line.

ifeq ($(filter libLPC2xxx.a, $(MAKECMDGOALS)),libLPC2xxx.a)
  ifeq ("$(LPC2XXX_MODEL)","")
    $(error "LPC2XXX_MODEL not defined.  Please set this to the model of chip (e.g. LPC2XXX_MODEL=lpc2103)")
  endif

  ifeq ("$(F_CPU)","")
    $(error "F_CPU not defined.  Please set this to the target system clock speed (e.g. F_CPU=60000000L)")
  endif

  ifeq ("$(HSE_Val)","")
    $(error "HSE_Val not defined.  Please set this to the High Speed External oscillator speed (e.g. -DHSE_VAL=12000000L)")
  endif
endif

# For the lpc2xxx device library's use
LPC2XXXLIB_FLAGS := -D$(LPC2XXX_MODEL) -DF_CPU=$(F_CPU) -DHSE_Val=$(HSE_Val)

# CPU machine flags
override LPC2XXX_MACHINE_FLAGS   += -mlittle-endian -mlong-calls -msoft-float -mcpu=$(CPU)

# By default enable thumb interwork
ifneq ("$(LPC2XXX_NO_INTERWORK)","1")
  LPC2XXX_MACHINE_FLAGS += -mthumb-interwork
endif

# Set compiler / linker flags for the library...

LPC2XXX_OPTIMIZE  =  -O2
LPC2XXX_INCLUDE   += -I$(LPC2XXXLIB_DIR)/inc
LPC2XXX_CFLAGS    += $(LPC2XXXLIB_FLAGS) $(LPC2XXX_MACHINE_FLAGS) $(LPC2XXX_INCLUDE) $(LPC2XXX_OPTIMIZE)
LPC2XXX_CXXFLAGS  += $(LPC2XXXLIB_FLAGS) $(LPC2XXX_MACHINE_FLAGS) $(LPC2XXX_INCLUDE) $(LPC2XXX_OPTIMIZE)
LPC2XXX_LIBS      += -L. -lLPC2xxx
LPC2XXX_LDFLAGS   += $(LPC2XXX_MACHINE_FLAGS) -nostartfiles \
                    -T$(LPC2XXXLIB_DIR)/link_scripts/$(LPC2XXX_MODEL)-rom.ld \
			-specs=nano.specs 
LPC2XXX_ARFLAGS   := -rs

# Set defaults for general compiler / linker flags (can be overriden on command
#  line without affecting library build)

CFLAGS            =  $(LPC2XXX_CFLAGS)
CXXFLAGS          =  $(LPC2XXX_CXXFLAGS)
LIBS              =  $(LPC2XXX_LIBS)
LDFLAGS           =  $(LPC2XXX_LDFLAGS)
ASFLAGS           =  $(LPC2XXX_ASFLAGS)

# Program names to use for compilation / working with binaries

AS                = $(CROSS_COMPILE)as
AR                = $(CROSS_COMPILE)ar
LD                = $(CROSS_COMPILE)ld
CC                = $(CROSS_COMPILE)gcc
CXX               = $(CROSS_COMPILE)g++
C++               = $(CXX)
STRIP             = $(CROSS_COMPILE)strip
RANLIB            = $(CROSS_COMPILE)ranlib
OBJDUMP           = $(CROSS_COMPILE)objdump
OBJCOPY           = $(CROSS_COMPILE)objcopy


# Default for building a binary.  Generally should be overriden.
%.elf: libLPC2xxx.a
%.elf: %.o
	$(CC) $(LDFLAGS) $(LIBS) -o $@ $^

# Defaults for other object building...
%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -j .dataflash -O ihex $< $@

%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -j .dataflash -O srec $< $@

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -j .dataflash -O binary $< $@
