///////////////////////////////////////////////////////////////////////////////
// File:	gsAvailable.h
// SDK:		GameSpy Common
//
// Copyright (c) IGN Entertainment, Inc.  All rights reserved.  
// This software is made available only pursuant to certain license terms offered
// by IGN or its subsidiary GameSpy Industries, Inc.  Unlicensed use or use in a 
// manner not expressly authorized by IGN or GameSpy is prohibited.

#ifndef _GSAVAILABLE_H_
#define _GSAVAILABLE_H_

#include "gsStringUtil.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef GSI_UNICODE
#define GSIStartAvailableCheck   GSIStartAvailableCheckA
#else
#define GSIStartAvailableCheck   GSIStartAvailableCheckW
#endif

// the available check contacts a backend server at "<gamename>.available.gamespy.com"
// an app can resolve the hostname itself and store the IP here before starting the check
extern char GSIACHostname[64];

// these are possible return types for GSIAvailableCheckThink
typedef enum
{
	GSIACWaiting,                 // still waiting for a response from the backend
	GSIACAvailable,               // the game's backend services are available
	GSIACUnavailable,             // the game's backend services are unavailable
	GSIACTemporarilyUnavailable   // the game's backend services are temporarily unavailable
} GSIACResult;

// start an available check for a particular game
// return 0 if no error starting up, non-zero if there's an error
void GSIStartAvailableCheck(const gsi_char * gamename);

// let the available check think
// continue to call this while it returns GSIACWaiting
// if it returns GSIACAvailable, use the GameSpy SDKs as normal
// if it returns GSIACUnavailable or GSIACTemporarilyUnavailable, do NOT
// continue to use the GameSpy SDKs.  the backend services are not available
// for the game.  in this case, you can show the user a
// message based on the particular result.
GSIACResult GSIAvailableCheckThink(void);

// this should only be used if the availability check needs to be aborted
// for example, if the player leaves the game's multiplayer area before the check completes
void GSICancelAvailableCheck(void);

// Get the current availability
GSIACResult GSIGetAvailable(void);

// internal use only
extern GSIACResult __GSIACResult;
extern char __GSIACGamename[64];

#ifdef __cplusplus
}
#endif

#endif
