# CDKey SDK Client Makefile
# Copyright 2004 GameSpy Industries

TARGET=gcdkeyclient
THREADS=yes
DEBUG=no
CFLAGS=-W -I.. -I../..

SRC = \
	../../gcdkeyc.c\
	../../../md5c.c\
	../../../common/gsPlatform.c\
	../../../common/gsPlatformSocket.c\
	../../../common/gsPlatformThread.c\
	../../../common/gsPlatformUtil.c\
	../../../common/gsStringUtil.c\
	../../../common/gsDebug.c\
	../../../common/gsMemory.c\
	../../../common/linux/LinuxCommon.c\
	../../ClientTest/gcdkeyclienttest.c\

include ../../../common/linux/Makefile.common
