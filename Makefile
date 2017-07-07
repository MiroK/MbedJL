# Based on embedding example from Julia /examples folder
# This Makefile template requires the following variables to be set
# in the environment or on the command-line:
#   JULIA: path to julia[.exe] executable
#   BIN:   binary build directory

JULIA=/usr/local/bin/julia
BIN=.

ifndef JULIA
  $(error "Please pass JULIA=[path of target julia binary], or set as environment variable!")
endif
ifndef BIN
  $(error "Please pass BIN=[path of build directory], or set as environment variable!")
endif

#=============================================================================
# this source directory where embedding.c is located
SRCDIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

# get the executable suffix, if any
EXE := $(suffix $(abspath $(JULIA))).x

# get compiler and linker flags. (see: `contrib/julia-config.jl`)
JULIA_CONFIG := $(JULIA) -e 'include(joinpath(JULIA_HOME, Base.DATAROOTDIR, "julia", "julia-config.jl"))' --
CPPFLAGS_ADD :=
CFLAGS_ADD = $(shell $(JULIA_CONFIG) --cflags) -I/usr/include/python2.7
LDFLAGS_ADD = -lm $(shell $(JULIA_CONFIG) --ldflags --ldlibs) -lpython2.7

#=============================================================================

release: $(BIN)/embedding$(EXE)

$(BIN)/embedding$(EXE): $(SRCDIR)/embedding.c
	$(CC) $^ -o $@ $(CPPFLAGS_ADD) $(CPPFLAGS) $(CFLAGS_ADD) $(CFLAGS) $(LDFLAGS_ADD) $(LDFLAGS)

clean:
	-rm -f $(BIN)/embedding$(EXE)

.PHONY: release clean 
