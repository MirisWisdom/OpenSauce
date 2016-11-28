# OSX Common Makefile
# Copyright 2011 GameSpy Industries

# Project Makefile must define the following:
# TARGET - name of output executable
# SRC - list of source files
# THREADS - "no" if project should not use threads
# DEBUG - "yes" if project should use debugging flags
#
# Optionally it may define CFLAGS and LDFLAGS for additional compiler or linker
# options, respectively, before including this file.

# Copyright 2006 GameSpy Industries

CC=gcc
BASE_CFLAGS=$(DEFINES) $(INCLUDES) -I../ -I../.. -I../../../common

unicode: UNICODE_CFLAGS = -DGSI_UNICODE

# Do not fail compile on warnings
CFLAGS=$(BASE_CFLAGS) $(UNICODE_CFLAGS) -D_MACOSX -DGSI_COMMON_DEBUG -D_NO_NOPORT_H_ -g -O0 -Wall 

#optimization flags
#CFLAGS=$(BASE_CFLAGS) $(UNICODE_CFLAGS) -O6 -ffast-math -funroll-loops \
	-fomit-frame-pointer -fexpensive-optimizations

#############################################################################
# SETUP AND BUILD
#############################################################################

# library paths
ifeq ($(DEBUG),yes)
	LIBDIR = $(SRCROOT)/../libs/macosxDebug
else
	LIBDIR = $(SRCROOT)/../libs/macosxRelease
endif

OBJCPP=$(SRC:.cpp=.o)
OBJ=$(OBJCPP:.c=.o) 
#############################################################################
# SETUP AND BUILD
#############################################################################

all: $(PROJECT)
$(PROJECT): $(OBJ)
	g++ $^ -o $@ -v $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS) -L$(LIBDIR) $(LDFLAGS)

.c.o:
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@

.cpp.o:
	g++ $(CFLAGS) $(CPPLAGS) -c $< -o $@ 
 
#############################################################################
# MISC
#############################################################################

unicode: $(PROJECT)

.PHONY: clean
clean:	
	rm -f $(PROJECT) $(PROJECT) $(OBJ)

dep : depend
depend:
	g++ $(CFLAGS) $(CFLAGS)  -MM $(SRC)
