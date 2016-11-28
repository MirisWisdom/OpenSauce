# Query & Reporting 2 SDK Makefile
# Copyright 2004 GameSpy Industries

TARGET=qr2csample
THREADS=yes
DEBUG=no
CFLAGS=-W -Wno-missing-field-initializers

SRC = \
	../../../common/gsPlatform.c\
	../../../common/gsAssert.c\
	../../../common/gsAvailable.c\
	../../../common/gsPlatformSocket.c\
	../../../common/gsPlatformThread.c\
	../../../common/gsPlatformUtil.c\
	../../../common/gsStringUtil.c\
	../../../common/gsDebug.c\
	../../../common/gsMemory.c\
	../../../common/linux/LinuxCommon.c\
	../../../darray.c\
	../../../hashtable.c\
	../../qr2.c\
	../../qr2regkeys.c\
    ../../../natneg/natneg.c\
    ../../../natneg/NATify.c\
	../qr2csample.c

include ../../../common/linux/Makefile.common
