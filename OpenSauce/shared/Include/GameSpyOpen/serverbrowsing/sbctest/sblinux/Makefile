# CDKey SDK Server Makefile
# Copyright 2004 GameSpy Industries

TARGET=sbctest
THREADS=yes
DEBUG=no
CFLAGS=-W

SRC = \
	../sbctest.c\
	../../../common/gsMemory.c\
	../../../common/linux/LinuxCommon.c\
	../../../darray.c\
	../../../hashtable.c\
	../../../common/gsAvailable.c\
	../../../common/gsDebug.c\
	../../../common/gsStringUtil.c\
	../../../common/gsPlatform.c\
	../../../common/gsPlatformSocket.c\
	../../../common/gsPlatformThread.c\
	../../../common/gsPlatformUtil.c\
	../../sb_crypt.c\
	../../sb_queryengine.c\
	../../sb_server.c\
	../../sb_serverbrowsing.c\
	../../sb_serverlist.c\
	../../../natneg/natneg.c\
    ../../../natneg/NATify.c\
	../../../qr2/qr2regkeys.c

include ../../../common/linux/Makefile.common
