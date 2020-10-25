# Common Makefile include for Linux
# Copyright 2009 GameSpy Industries

# Project Makefile must define the following:
# TARGET - name of output executable
# SRC - list of source files
# THREADS - "no" if project should not use threads
# DEBUG - "yes" if project should use debugging flags
#
# Optionally it may define CFLAGS and LDFLAGS for additional compiler or linker
# options, respectively, before including this file.

DEBUG_TARGET=$(TARGET)_debug

CC=gcc
BASE_CFLAGS=-D_LINUX -Wall -Werror -pipe

ifeq ($(THREADS),no)
	BASE_CFLAGS:=$(BASE_CFLAGS) -DGSI_NO_THREADS
else
	LDFLAGS:= -lpthread $(LDFLAGS)
endif

# library paths
ifeq ($(DEBUG),yes)
	LIBDIR = $(SRCROOT)/../libs/linuxDebug
	BASE_CFLAGS:=$(BASE_CFLAGS) -DGSI_COMMON_DEBUG -D_DEBUG
	CFLAGS:=$(BASE_CFLAGS) $(CFLAGS) $(INCLUDES) -g
else
	LIBDIR = $(SRCROOT)/../libs/linuxRelease
	CFLAGS:=$(BASE_CFLAGS) $(CFLAGS) $(INCLUDES) -O2
endif

OBJCPP=$(SRC:.cpp=.o)
OBJ=$(OBJCPP:.c=.o) 
#############################################################################
# SETUP AND BUILD
#############################################################################

all: $(TARGET)
$(TARGET): $(OBJ)
	g++ $^ -o $@ -v $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS) -L$(LIBDIR) $(LDFLAGS)

.c.o:
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@

.cpp.o:
	g++ $(CFLAGS) $(CPPLAGS) -c $< -o $@ 
 
#############################################################################
# MISC
#############################################################################

.PHONY: clean
clean:	
	rm -f $(TARGET) $(DEBUG_TARGET) $(OBJ)

dep : depend
depend:
	g++ $(CFLAGS) $(CFLAGS)  -MM $(SRC)
