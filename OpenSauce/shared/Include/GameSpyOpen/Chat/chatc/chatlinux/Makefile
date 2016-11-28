# Chat SDK Makefile
# Copyright 2004 GameSpy Industries

TARGET=chatsdk
THREADS=yes
DEBUG=no
CFLAGS=-W

SRC = \
	../../../darray.c\
	../../../hashtable.c\
	../../../md5c.c\
	../../../common/gsPlatform.c\
	../../../common/gsPlatformSocket.c\
	../../../common/gsPlatformThread.c\
	../../../common/gsPlatformUtil.c\
	../../../common/gsStringUtil.c\
	../../../common/gsDebug.c\
	../../../common/gsMemory.c\
	../../../common/linux/LinuxCommon.c\
	../../chatCrypt.c\
	../../chatCallbacks.c\
	../../chatChannel.c\
	../../chatHandlers.c\
	../../chatMain.c\
	../../chatSocket.c\
	../chatc.c

include ../../../common/linux/Makefile.common
