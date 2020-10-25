# GameSpy Transport 2 SDK Makefile
# Copyright 2004 GameSpy Industries

TARGET=gt2testc
THREADS=yes
DEBUG=no
CFLAGS=-Wno-missing-field-initializers

SRC = \
	../../../darray.c\
	../../../hashtable.c\
	../../../common/gsPlatform.c\
	../../../common/gsPlatformSocket.c\
	../../../common/gsPlatformThread.c\
	../../../common/gsPlatformUtil.c\
	../../../common/gsStringUtil.c\
	../../../common/gsDebug.c\
	../../../common/gsMemory.c\
	../../../common/linux/LinuxCommon.c\
	../../gt2Auth.c\
	../../gt2Buffer.c\
	../../gt2Callback.c\
	../../gt2Connection.c\
	../../gt2Filter.c\
	../../gt2Main.c\
	../../gt2Message.c\
	../../gt2Socket.c\
	../../gt2Encode.c\
	../../gt2Utility.c\
	../$(TARGET).c

include ../../../common/linux/Makefile.common
