# Sake SDK Makefile - Mac OSX
# Copyright 2006 GameSpy Industries

PROJECT=saketest

DEFINES=-D_MACOSX -DGSI_NO_THREADS

SRCROOT= ../..

SRC = \
	../../../md5c.c\
	../../../darray.c\
	../../../hashtable.c\
	../../../common/gsAssert.c\
	../../../common/gsAvailable.c\
	../../../common/gsCore.c\
	../../../common/gsDebug.c\
	../../../common/gsCrypt.c\
	../../../common/gsLargeInt.c\
	../../../common/gsStringUtil.c\
	../../../common/gsPlatform.c\
	../../../common/gsPlatformSocket.c\
	../../../common/gsPlatformThread.c\
	../../../common/gsPlatformUtil.c\
	../../../common/gsMemory.c\
	../../../common/gsXML.c\
	../../../common/gsSHA1.c\
	../../../common/gsRC4.c\
	../../../common/gsSSL.c\
	../../../common/gsSoap.c\
	../../../common/gsUdpEngine.c\
	../../../common/macosx/MacOSXCommon.c\
	../../../gp/gp.c\
	../../../gp/gpi.c\
	../../../gp/gpiBuddy.c\
	../../../gp/gpiBuffer.c\
	../../../gp/gpiCallback.c\
	../../../gp/gpiConnect.c\
	../../../gp/gpiInfo.c\
	../../../gp/gpiKeys.c\
	../../../gp/gpiOperation.c\
	../../../gp/gpiPeer.c\
	../../../gp/gpiProfile.c\
	../../../gp/gpiSearch.c\
	../../../gp/gpiUnique.c\
	../../../gp/gpiUtility.c\
	../../../gp/gpiTransfer.c\
	../../../ghttp/ghttpBuffer.c\
	../../../ghttp/ghttpCallbacks.c\
	../../../ghttp/ghttpConnection.c\
	../../../ghttp/ghttpEncryption.c\
	../../../ghttp/ghttpMain.c\
	../../../ghttp/ghttpProcess.c\
	../../../ghttp/ghttpCommon.c\
	../../../ghttp/ghttpPost.c\
	../../../gt2/gt2Auth.c\
	../../../gt2/gt2Buffer.c\
	../../../gt2/gt2Callback.c\
	../../../gt2/gt2Connection.c\
	../../../gt2/gt2Filter.c\
	../../../gt2/gt2Main.c\
	../../../gt2/gt2Message.c\
	../../../gt2/gt2Socket.c\
	../../../gt2/gt2Encode.c\
	../../../gt2/gt2Utility.c\
	../../sakeMain.c\
	../../sakeRequest.c\
	../../sakeRequestMisc.c\
	../../sakeRequestModify.c\
	../../sakeRequestRead.c\
	../saketest.c

# include directories
INCLUDES = -I$(SRCROOT) -I$(SRCROOT)/.. -I$(SRCROOT)/../common -I$(SRCROOT)/../ghttp

LDFLAGS:= $(LDFLAGS) -lAuthservice

include ../../../common/macosx/MakeCommon.mk
