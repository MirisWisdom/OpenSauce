# Atlas samples Makefile
# Copyright 2011 GameSpy Industries

TARGET=atlasQuerySample
THREADS=no
DEBUG=no
CFLAGS:= $(CFLAGS) -W -Wno-write-strings -Wno-unused-function -Wno-unused-variable
SRCROOT= ../..

SRC = \
	../../../md5c.c\
	../../../darray.c\
	../../../hashtable.c\
	../../../common/linux/LinuxCommon.c\
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
	../../../GP/gp.c\
	../../../GP/gpi.c\
	../../../GP/gpiBuddy.c\
	../../../GP/gpiBuffer.c\
	../../../GP/gpiCallback.c\
	../../../GP/gpiConnect.c\
	../../../GP/gpiInfo.c\
	../../../GP/gpiKeys.c\
	../../../GP/gpiOperation.c\
	../../../GP/gpiPeer.c\
	../../../GP/gpiProfile.c\
	../../../GP/gpiSearch.c\
	../../../GP/gpiUnique.c\
	../../../GP/gpiUtility.c\
	../../../GP/gpiTransfer.c\
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
	../../sciInterface.c\
	../../sciMain.c\
	../../sciReport.c\
	../../sciSerialize.c\
	../../sciWebServices.c\
	../../atlasSampleCommon/atlas_atlas_samples_v1.c\
	../StatsCourier.cpp\
	../../atlasSampleCommon/Utility.cpp\
	../atlasQuerySample.cpp

# include directories
INCLUDES =\
	-I$(SRCROOT)\
	-I..\
	-I../atlasSampleCommon\
	-I$(SRCROOT)/..\
	-I$(SRCROOT)/../gt2\
	-I$(SRCROOT)/../common\
	-I$(SRCROOT)/../ghttp\
	-I$(SRCROOT)/../GP\
	-I$(SRCROOT)/../webservices

LDFLAGS:= $(LDFLAGS) -lAuthservice -lpthread  

include ../../../common/linux/MakeCommon.mk

