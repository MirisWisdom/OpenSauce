# CDKey SDK Server
# Copyright 2004 GameSpy Industries

TARGET=gcdkeyserver
THREADS=yes
DEBUG=no
CFLAGS=-W -I.. -I../..

SRC = \
	../../gcdkeys.c\
	../../../md5c.c\
	../../../common/gsAvailable.c\
	../../../common/gsPlatform.c\
	../../../common/gsPlatformSocket.c\
	../../../common/gsPlatformThread.c\
	../../../common/gsPlatformUtil.c\
	../../../common/gsStringUtil.c\
	../../../common/gsDebug.c\
	../../../common/gsMemory.c\
	../../../common/linux/LinuxCommon.c\
	../../ServerTest/gcdkeyservertest.c\

include ../../../common/linux/Makefile.common
