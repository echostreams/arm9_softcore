# Makefile : gmake file for building the LPC2xxx Device Library v2.0
#            device library.


# Directories to make in
subdirs := src

.PHONY: all libLPC2xxx.a

all: show_targets

O := $(if $(O),$(O),$(PWD))

show_targets:
	echo Available Targets:
	echo   libLPC2xxx.a -- build the library (needs MODEL, F_CPU, HSE_Val to be set)

libLPC2xxx.a:
	$(MAKE) -C src O=$(O) $@

