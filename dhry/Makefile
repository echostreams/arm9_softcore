NAME		= dhry
TARGET		= dhry
LD_RAMROM	= ROM
PROGRAM_MK  = true
DEBUG		= 1
# Optimization setting
# (-Os for small code size, -O2 for speed)
OFLAGS  	= -Os
EFLAGS		=
CSRCS		= dhry_1.c dhry_2.c reloc.c framework.c stack.c
#CSRCS		= main.c reloc.c framework.c stack.c
ASRCS		= startup.S

#----------------------------------------------------------------------
# TOOL DEFINITIONS
#----------------------------------------------------------------------
TOOLTARGET      = arm-none-eabi
AR              = $(TOOLTARGET)-ar
AS              = $(TOOLTARGET)-gcc
AWK             = gawk
CC              = $(TOOLTARGET)-gcc
LD              = $(TOOLTARGET)-gcc
OBJCOPY         = $(TOOLTARGET)-objcopy
RANLIB          = $(TOOLTARGET)-ranlib
RM              = rm -f
SED             = sed
SHELL           = bash --posix
SIZE            = $(TOOLTARGET)-size
STRIP           = $(TOOLTARGET)-strip

CODE		    = ARM
TARGET_MODE	    = arm

CPU_VARIANT	    = LPC2104
#CPU		    = arm926ej-s
CPU		        = arm7tdmi
OPTS		    = -mcpu=$(CPU) #-mapcs-frame -mapcs-stack-check -msoft-float -mfloat-abi=softfp -fno-common #-fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -MMD -MP

#----------------------------------------------------------------------
# COMPILER AND ASSEMBLER OPTIONS
#----------------------------------------------------------------------
W_OPTS    = -Wall -Wcast-align -Wcast-qual -Wimplicit \
            -Wnested-externs -Wpointer-arith -Wswitch \
            -Wreturn-type \
                        -Wformat -Wformat-security\
                        -Werror -Wextra -pedantic

LD_SCRIPT	= link_16k_128k_rom.ld
LD_FLAGS	= -Wl,--gc-sections -nostartfiles #-nostdlib -lnosys
LD_OPTS   	= $(OPTS) $(EFLAGS) -specs=nano.specs -T $(LD_SCRIPT) -o $(NAME).elf \
			-Wl,-Map=$(NAME).map,--cref -specs=nosys.specs -u _printf_float -u _scan_float

CA_OPTS		= $(OPTS) -D$(CPU_VARIANT) -D MSC_CLOCK #-flto -ffunction-sections -fdata-sections -fno-builtin
CC_OPTS		= $(CA_OPTS) $(OFLAGS) $(DBFLAGS) #$(W_OPTS)
CC_OPTS_A	= $(CA_OPTS)

#----------------------------------------------------------------------
# LIST ALL OBJECT FILES
#----------------------------------------------------------------------
OBJS ?= $(CSRCS:.c=.o) $(ASRCS:.S=.o)

all: $(TARGET) codesize

%.o: %.c
	$(CC) -c $(CC_OPTS) -o $@ $<
%.o: %.S
	$(AS) -c $(CC_OPTS_A) -o $@ $<

clean:
	$(RM) $(TARGET) $(OBJS) $(OBJS:.o=.lst) $(NAME).elf hello.elf hello.bin

$(TARGET): $(OBJS) $(LIBS)
	$(LD) $(OBJS) $(LIBS) $(INC) $(LD_OPTS) $(LD_FLAGS)
	$(OBJCOPY) -O binary $(NAME).elf $(TARGET)

hello:
	$(CC) -c $(CC_OPTS) hello.c
	$(LD) hello.o startup.o reloc.o framework.o stack.o $(LIBS) $(INC) $(LD_OPTS) -o hello.elf
	$(OBJCOPY) -O binary hello.elf hello.bin

codesize: $(TARGET)
	@echo ""
	@echo "=== Result ==============================================="
	@echo ""
	@echo " TEXT: Code size"
	@echo " DATA: Initialized data"
	@echo " BSS:  Uninitialized data"
	@echo " ROM:  Size in non-volatile program memory (TEXT + DATA)"
	@echo " RAM:  Size in volatile data memory (DATA + BSS)"
ifdef LIBS
	@echo ""
	@echo "=== Component sizes ======================================"
	@echo ""
	@echo "   TEXT    DATA     BSS     ROM     RAM FILENAME"
	@echo "   ====    ====     ===     ===     === ========"
# ifdef LIBS
	@for f in $(LIBS) ; do \
	  $(SIZE) "$$f" \
	  | $(AWK) "NR > 1 { t+=\$$1; d+=\$$2; b+=\$$3; } \
		    END { ro=t+d; ra=d+b; printf \"%7d %7d %7d %7d %7d %s\n\", \
						 t,d,b,ro,ra,\"$$f\" }" ; \
	done
endif
	@echo ""
	@echo ""
ifeq (true, $(PROGRAM_MK))
	@echo "=== Total size (including external libraries) ============"
	@echo ""
	@echo "   Code compiled with optimization switch: " $(OFLAGS)
	@echo "   (other settings may produce different sizes)"
	@echo "   (external libraries may be compiled with different settings)"
	@echo ""
	@echo "   TEXT    DATA     BSS     ROM     RAM FILENAME"
	@echo "   ====    ====     ===     ===     === ========"
	@$(SIZE) $(NAME).elf \
	| $(AWK) 'NR > 1 { printf "%7d %7d %7d %7d %7d %s\n", \
			   $$1,$$2,$$3,$$1+$$2,$$2+$$3,"$(NAME).elf" }'
	@$(SIZE) $(NAME).elf \
        | $(AWK) 'NR > 1 { printf "%7x %7x %7x %7x %7x %s\n", \
                           $$1,$$2,$$3,$$1+$$2,$$2+$$3,"$(NAME).elf" }'

	@echo ""
ifeq (RAM, $(LD_RAMROM))
	@echo "                          |===========|"
	@echo "                               ^^^"
	@echo "   Everything placed in RAM since LD_RAMROM = RAM"
	@echo ""
endif

else
	@echo "=== Total size ==========================================="
	@echo ""
	@echo "   Code compiled with optimization switch: " $(OFLAGS)
	@echo "   (other settings may produce different sizes)"
	@echo ""
	@echo "   TEXT    DATA     BSS     ROM     RAM FILENAME"
	@echo "   ====    ====     ===     ===     === ========"
	@$(SIZE) $(TARGET) \
	| $(AWK) 'NR > 1 { t+=$$1; d+=$$2; b+=$$3; } \
		  END { ro=t+d; ra=d+b; printf "%7d %7d %7d %7d %7d %s\n", \
					t,d,b,ro,ra,"$(TARGET)" }'
endif


