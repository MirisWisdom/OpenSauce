///////////////////////////////////////////////////////////////////////////////
// File:	sakeMain.h
// SDK:		GameSpy Sake Persistent Storage SDK
//
// Copyright (c) IGN Entertainment, Inc.  All rights reserved.  
// This software is made available only pursuant to certain license terms offered
// by IGN or its subsidiary GameSpy Industries, Inc.  Unlicensed use or use in a 
// manner not expressly authorized by IGN or GameSpy is prohibited.

#ifndef __SAKEMAIN_H__
#define __SAKEMAIN_H__

#include "sake.h"


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if defined(__cplusplus)
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define SAKEI_GAME_NAME_LENGTH    15
#define SAKEI_SECRET_KEY_LENGTH    8
#define SAKEI_LOGIN_TICKET_LENGTH 24

#define IS_GHTTP_ERROR(x) (x<0)

//////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define GS_SAKE_ERROR(s,c)              GS_ERROR(GSResultSDK_Sake,s,c)
#define GS_SAKE_SUCCESS(s,c)            GS_RESULT(0,GSResultSDK_Sake,s,c)
#define GS_SAKE_DP(t,l,m, ...)          gsDebugFormat(GSIDebugCat_Sake, t, l, m, __VA_ARGS__) 
    // m is concatenated to format string
#define GS_SAKE_DEBUG_LOG(t, l, m, ... ) GS_SAKE_DP(t, l, "%s(%d): In %s: "m"\n" , __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef struct SAKEInternal
{
	gsi_bool mIsGameAuthenticated;
	char     mGameName[SAKEI_GAME_NAME_LENGTH + 1];
	int      mGameId;
	char     mSecretKey[SAKEI_SECRET_KEY_LENGTH + 1];

	gsi_bool mIsProfileAuthenticated;
	int      mProfileId;
	GSLoginCertificate mCertificate;
	GSLoginPrivateData mPrivateData;
	SAKEStartRequestResult mStartRequestResult;
} SAKEInternal;

typedef struct SAKEInternalUploadData
{
	SAKE					mSake;
    SAKEUploadContentProgressCallback  mProgressCallback;
	SAKEUploadContentCompletedCallback mCompletedCallback;
	void					*mUserData;
} SAKEInternalUploadData;


typedef struct SAKEInternalDownloadData
{
	SAKE						mSake;
    SAKEDownloadContentProgressCallback   mProgressCallback ;
	SAKEDownloadContentCompletedCallback  mCompletedCallback;
	void						*mUserData;
}SAKEInternalDownloadData;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if defined(__cplusplus)
} // extern "C"
#endif


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#endif // __SAKEMAIN_H__
