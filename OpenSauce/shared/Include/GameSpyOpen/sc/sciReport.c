///////////////////////////////////////////////////////////////////////////////
// File:	sciReport.c
// SDK:		GameSpy ATLAS Competition SDK
//
// Copyright (c) IGN Entertainment, Inc.  All rights reserved.  
// This software is made available only pursuant to certain license terms offered
// by IGN or its subsidiary GameSpy Industries, Inc.  Unlicensed use or use in a 
// manner not expressly authorized by IGN or GameSpy is prohibited.

#include "sciReport.h"
#include "sciSerialize.h"
#include "../md5.h"


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult SC_CALL sciCreateReport(gsi_u8 theSessionGuid[SC_SESSION_GUID_SIZE], 
								 gsi_u32 theHeaderVersion,
								 gsi_u32 thePlayerCount,
								 gsi_u32 theTeamCount,
								 SCIReport ** theReportOut)
{
	SCIReport * theNewReport;
	SCIReportHeader * theReportHeader;
	gsi_u8 * theReportData;

	// roster is [CCID (16) + TeamIndex (4)] * numplayers
	const gsi_u32 theRosterSize = SC_GUID_BINARY_SIZE * thePlayerCount + 
		                          SC_REPORT_TEAMINDEX_LENGTH * thePlayerCount;

	GS_ASSERT(theReportOut != NULL);

	// allocate the report
	theNewReport = (SCIReport*)gsimalloc(sizeof(SCIReport));
	if (theNewReport == NULL)
		return SCResult_OUT_OF_MEMORY;
	memset(theNewReport, 0, sizeof(SCIReport));

	// allocate the report buffer (holds submission data)
	theReportData = (gsi_u8*)gsimalloc(SC_REPORT_BUFFER_BYTES);
	if (theReportData == NULL)
	{
		gsifree(theNewReport);
		return SCResult_OUT_OF_MEMORY;
	}
	memset(theReportData, 0, SC_REPORT_BUFFER_BYTES);
	theNewReport->mBuffer.mIsStatic = gsi_false;
	theNewReport->mBuffer.mCapacity = SC_REPORT_BUFFER_BYTES;

	// Fill in report header
	theReportHeader = (SCIReportHeader*)theReportData;
	memset(theReportHeader, 0, sizeof(SCIReportHeader));

	theReportHeader->mProtocolVersion  = htonl(SC_REPORT_PROTOCOL);
	theReportHeader->mDeveloperVersion = htonl(theHeaderVersion);

	theReportHeader->mRosterSectionLength = theRosterSize;
	theReportHeader->mAuthSectionLength = SC_REPORT_AUTHDATA_LENGTH * thePlayerCount;
	theReportHeader->mResultsSectionLength = SC_REPORT_ENTITYRESULT_LENGTH * (thePlayerCount + theTeamCount);
	theReportHeader->mPlayerCount      = (gsi_u16)thePlayerCount;
	theReportHeader->mTeamCount        = (gsi_u16)theTeamCount;
	
	// Finished, return new report
	theNewReport->mReportState = SCIReportState_ROSTER;
	theNewReport->mBuffer.mData = (char *)theReportData;
	theNewReport->mCurEntityStartPos = -1;
	theNewReport->mNumTeamsReported = 0;

	*theReportOut = theNewReport;
	GSI_UNUSED(theSessionGuid);
	return SCResult_NO_ERROR;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
SCResult sciDestroyReport(SCIReport *theReport)
{
	theReport->mReportState = SCIReportState_NONE;
	theReport->mCurEntityKeyCount = 0;
	theReport->mCurEntityStartPos = 0;
	theReport->mNumTeamsReported = 0;
	theReport->mNumResultsReported = 0;
	
	gsifree(theReport->mBuffer.mData);
	theReport->mBuffer.mData = NULL;
	theReport->mBuffer.mCapacity = 0;
	theReport->mBuffer.mLen = 0;
	theReport->mBuffer.mPos = 0;
	gsifree(theReport);
	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult SC_CALL sciReportSetPlayerConnectionId(SCIReport * theReport, gsi_u32 thePlayerIndex, const gsi_u8 theConnectionId[SC_CONNECTION_GUID_SIZE])
{
	gsi_u32 rosterDataOffset = 0;

	// Roster section is just after the header
	rosterDataOffset = sizeof(SCIReportHeader);
	rosterDataOffset += SC_REPORT_ROSTERDATA_LENGTH * thePlayerIndex;

	// copy the connection id
	GS_ASSERT((rosterDataOffset + SC_GUID_BINARY_SIZE) < theReport->mBuffer.mCapacity);

	sciSerializeGUID((gsi_u8 *)&theReport->mBuffer.mData[rosterDataOffset], theConnectionId);

	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult SC_CALL sciReportSetPlayerTeamIndex(SCIReport * theReport, gsi_u32 thePlayerIndex, gsi_u32 theTeamIndex)
{
	gsi_u32 rosterDataOffset = 0;

	// Roster section is just after the header
	rosterDataOffset = sizeof(SCIReportHeader);
	rosterDataOffset += SC_REPORT_ROSTERDATA_LENGTH * thePlayerIndex;

	// copy the team index, which must appear just after the connection id
	rosterDataOffset += SC_GUID_BINARY_SIZE;
	GS_ASSERT((rosterDataOffset + sizeof(gsi_i32)) < theReport->mBuffer.mCapacity);
	sciSerializeInt32((gsi_u8 *)&theReport->mBuffer.mData[rosterDataOffset], (gsi_i32)theTeamIndex);
	
	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult SC_CALL sciReportSetPlayerGameResult(SCIReport * theReport, gsi_u32 thePlayerIndex, SCGameResult theGameResult)
{
	SCIReportHeader * aHeader = (SCIReportHeader*)theReport->mBuffer.mData;
	gsi_u32 resultsDataOffset = 0;

	// Results section is just after the auth section
	resultsDataOffset = sizeof(SCIReportHeader) + aHeader->mRosterSectionLength + aHeader->mAuthSectionLength;
	resultsDataOffset += sizeof(gsi_u32) * thePlayerIndex; // 4 byte game result per player

	// copy the game result in network byte order
	GS_ASSERT((resultsDataOffset + sizeof(gsi_u32)) < theReport->mBuffer.mCapacity);
	sciSerializeInt32((gsi_u8 *)&theReport->mBuffer.mData[resultsDataOffset], theGameResult);
	
	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult SC_CALL sciReportSetPlayerAuthInfo(SCIReport * theReport, gsi_u32 thePlayerIndex, const GSLoginCertificate * certificate, const gsi_u8 theAuthHash[16])
{
	SCIReportHeader * aHeader = (SCIReportHeader*)theReport->mBuffer.mData;
	gsi_u32 authDataOffset = 0;

	// Auth section is just after the roster
	authDataOffset = sizeof(SCIReportHeader) + aHeader->mRosterSectionLength;
	authDataOffset += SC_REPORT_AUTHDATA_LENGTH * thePlayerIndex;

	// copy auth data into the buffer
	// &theReport->mBuffer.mData[authDataOffset]
	GSI_UNUSED(theAuthHash);
	GSI_UNUSED(certificate);
	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult SC_CALL sciReportSetTeamGameResult(SCIReport * theReport, gsi_u32 theTeamIndex, SCGameResult theGameResult)
{
	SCIReportHeader * aHeader = (SCIReportHeader*)theReport->mBuffer.mData;
	gsi_u32 resultsDataOffset = 0;

	// Results section is just after the auth section
	//    team results are just after player results
	resultsDataOffset = sizeof(SCIReportHeader) + aHeader->mRosterSectionLength + aHeader->mAuthSectionLength;
	resultsDataOffset += sizeof(gsi_u32) * aHeader->mPlayerCount; // 4 byte game result per player
	resultsDataOffset += sizeof(gsi_u32) * theTeamIndex; // 4 byte game result per player

	// copy the game result in network byte order
	GS_ASSERT((resultsDataOffset + sizeof(gsi_i32)) < theReport->mBuffer.mCapacity);
	sciSerializeInt32((gsi_u8 *)&theReport->mBuffer.mData[resultsDataOffset], theGameResult);
	
	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult SC_CALL sciReportSetAsMatchless(SCIReport * theReport)
{
	SCIReportHeader * aHeader;

	aHeader = (SCIReportHeader *)theReport->mBuffer.mData;

	aHeader->mFlags |= SC_REPORT_FLAG_MATCHLESS_SESSION;

	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult SC_CALL sciReportBeginNewTeam(SCIReport * theReport)
{
	SCIReportHeader * aHeader = (SCIReportHeader*)theReport->mBuffer.mData;

	//sciReportEndEntity(theReport);

	theReport->mCurEntityKeyCount = 0;
	theReport->mCurEntityStartPos = (gsi_i32)theReport->mBuffer.mPos;
	theReport->mBuffer.mPos += sizeof(gsi_u16);
	sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mCurEntityStartPos], (gsi_i16)theReport->mCurEntityKeyCount);
	
	aHeader->mTeamSectionLength += sizeof(gsi_u16);

	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult SC_CALL sciReportBeginNewPlayer(SCIReport * theReport)
{
	SCIReportHeader * aHeader = (SCIReportHeader*)theReport->mBuffer.mData;
	GS_ASSERT(theReport != NULL);

	//if (theReport->mReportState == SCIReportState_ROSTER)
	{
		// this is the first player

	}
	//GS_ASSERT(theReport->mReportState == SCIReportState_ROSTER);

	theReport->mCurEntityKeyCount = 0;
	theReport->mCurEntityStartPos = (gsi_i32)theReport->mBuffer.mPos;
	theReport->mBuffer.mPos += sizeof(gsi_u16);
	sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mCurEntityStartPos], (gsi_i16)theReport->mCurEntityKeyCount);

	aHeader->mPlayerSectionLength += sizeof(gsi_u16);
	
	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult SC_CALL sciReportBeginGlobalData(SCIReport * theReport)
{
	SCIReportHeader * aHeader = (SCIReportHeader*)theReport->mBuffer.mData;

	GS_ASSERT(theReport != NULL);
	GS_ASSERT(theReport->mReportState == SCIReportState_ROSTER);

	theReport->mReportState = SCIReportState_GLOBALDATA;

	// set buffer write position to start of global data
	theReport->mBuffer.mPos = sizeof(SCIReportHeader) + aHeader->mRosterSectionLength +
		aHeader->mAuthSectionLength + aHeader->mResultsSectionLength;
	
	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult SC_CALL sciReportBeginPlayerData(SCIReport * theReport)
{
	GS_ASSERT(theReport != NULL);

	// Must not have passed into player data state yet.
	GS_ASSERT(theReport->mReportState == SCIReportState_ROSTER ||
		theReport->mReportState == SCIReportState_GLOBALDATA);
	
	// case where there is no global data
	if (theReport->mReportState == SCIReportState_ROSTER)
	{
	}

	theReport->mReportState    = SCIReportState_PLAYERDATA;

	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult SC_CALL sciReportBeginTeamData(SCIReport * theReport)
{
	GS_ASSERT(theReport != NULL);

	// Must not have passed into player data state yet.
	GS_ASSERT(
		theReport->mReportState == SCIReportState_ROSTER || 
		theReport->mReportState == SCIReportState_GLOBALDATA ||
		theReport->mReportState == SCIReportState_PLAYERDATA
		);
	
	theReport->mReportState    = SCIReportState_TEAMDATA;

	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult SC_CALL sciReportEndEntity(SCIReport * theReport)
{
	GSI_UNUSED(theReport);
	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult SC_CALL sciReportEnd(SCIReport * theReport, gsi_bool isAuth, SCGameStatus theStatus)
{
	SCIReportHeader * aHeader = (SCIReportHeader*)theReport->mBuffer.mData;

	sciReportEndEntity(theReport);

	if (gsi_is_true(isAuth))
		aHeader->mFlags |= SC_REPORT_FLAG_AUTHORITATIVE;

	// Send header in network byte order
	aHeader->mGameStatus  = htonl(theStatus);

	aHeader->mFlags       = htonl(aHeader->mFlags);
	aHeader->mPlayerCount = htons(aHeader->mPlayerCount);
	aHeader->mTeamCount   = htons(aHeader->mTeamCount);
	aHeader->mReserved = 0;

	aHeader->mGameKeyCount = htons(aHeader->mGameKeyCount);
	aHeader->mPlayerKeyCount = htons(aHeader->mPlayerKeyCount);
	aHeader->mTeamKeyCount   = htons(aHeader->mTeamKeyCount);

	aHeader->mRosterSectionLength  = htonl(aHeader->mRosterSectionLength);
	aHeader->mAuthSectionLength    = htonl(aHeader->mAuthSectionLength);
	aHeader->mResultsSectionLength = htonl(aHeader->mResultsSectionLength);

	aHeader->mGameSectionLength   = htonl(aHeader->mGameSectionLength);
	aHeader->mPlayerSectionLength = htonl(aHeader->mPlayerSectionLength);
	aHeader->mTeamSectionLength   = htonl(aHeader->mTeamSectionLength);

	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult SC_CALL sciReportAddIntValue(SCIReport * theReport,
									  gsi_u16     theKeyId,
									  gsi_i32     theValue)
{
	SCIReportHeader * aHeader = (SCIReportHeader*)theReport->mBuffer.mData;
	int writtenLen = 0;
	gsi_i16 theKeyType = (gsi_i16)SCDataType_INT32;
	
	// calculate length of data to be written
	writtenLen += sizeof(gsi_u16) + sizeof(gsi_u16); // 2 byte key ID, 2 byte key type;
	writtenLen += sizeof(gsi_u32); // 4 bytes of data

	// Check room in buffer
	GS_ASSERT((theReport->mBuffer.mPos + writtenLen) < theReport->mBuffer.mCapacity);

	// Update count and length markers
	if (theReport->mReportState == SCIReportState_GLOBALDATA)
	{
		aHeader->mGameKeyCount++;
		aHeader->mGameSectionLength += writtenLen;
	}
	else if (theReport->mReportState == SCIReportState_PLAYERDATA)
	{
		GS_ASSERT(theReport->mCurEntityStartPos != -1);
		aHeader->mPlayerKeyCount++;
		aHeader->mPlayerSectionLength += writtenLen;
		
		theReport->mCurEntityKeyCount++;
		sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mCurEntityStartPos], (gsi_i16)theReport->mCurEntityKeyCount);
	}
	else if (theReport->mReportState == SCIReportState_TEAMDATA)
	{
		aHeader->mTeamKeyCount++;
		aHeader->mTeamSectionLength += writtenLen;

		theReport->mCurEntityKeyCount++;
		sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mCurEntityStartPos], (gsi_i16)theReport->mCurEntityKeyCount);
	}

	// Write the data
	sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mBuffer.mPos], (gsi_i16)theKeyId);
	theReport->mBuffer.mPos += sizeof(gsi_u16);
	sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mBuffer.mPos], theKeyType);
	theReport->mBuffer.mPos += sizeof(gsi_u16);
	sciSerializeInt32((gsi_u8 *)&theReport->mBuffer.mData[theReport->mBuffer.mPos], theValue);
	theReport->mBuffer.mPos += sizeof(gsi_u32);

	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult SC_CALL sciReportAddInt64Value(SCIReport * theReport, 
										gsi_u16 theKeyId, 
										gsi_i64 theValue)
{
	SCIReportHeader * aHeader = (SCIReportHeader*)theReport->mBuffer.mData;
	int writtenLen = 0;
	gsi_i16 theKeyType = SCDataType_INT64;

	// calculate length of data to be written
	writtenLen += sizeof(gsi_u16) + sizeof(gsi_u16); // 2 byte key ID, 2 byte key type;
	writtenLen += sizeof(gsi_i64); // 4 bytes of data

	// Check room in buffer
	GS_ASSERT((theReport->mBuffer.mPos + writtenLen) < theReport->mBuffer.mCapacity);

	// Update count and length markers
	if (theReport->mReportState == SCIReportState_GLOBALDATA)
	{
		aHeader->mGameKeyCount++;
		aHeader->mGameSectionLength += writtenLen;
	}
	else if (theReport->mReportState == SCIReportState_PLAYERDATA)
	{
		GS_ASSERT(theReport->mCurEntityStartPos != -1);
		aHeader->mPlayerKeyCount++;
		aHeader->mPlayerSectionLength += writtenLen;

		theReport->mCurEntityKeyCount++;
		sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mCurEntityStartPos], (gsi_i16)theReport->mCurEntityKeyCount);
	}
	else if (theReport->mReportState == SCIReportState_TEAMDATA)
	{
		aHeader->mTeamKeyCount++;
		aHeader->mTeamSectionLength += writtenLen;

		theReport->mCurEntityKeyCount++;
		sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mCurEntityStartPos], (gsi_i16)theReport->mCurEntityKeyCount);
	}

	// Write the data
	sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mBuffer.mPos], (gsi_i16)theKeyId);
	theReport->mBuffer.mPos += sizeof(gsi_u16);
	sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mBuffer.mPos], theKeyType);
	theReport->mBuffer.mPos += sizeof(gsi_u16);
	sciSerializeInt64((gsi_u8 *)&theReport->mBuffer.mData[theReport->mBuffer.mPos], theValue);
	theReport->mBuffer.mPos += sizeof(gsi_i64);

	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult SC_CALL sciReportAddShortValue(SCIReport * theReport,
									    gsi_u16     theKeyId,
									    gsi_i16     theValue)
{
	SCIReportHeader * aHeader = (SCIReportHeader*)theReport->mBuffer.mData;
	int writtenLen = 0;
	gsi_i16 theKeyType = SCDataType_INT16;
	
	// calculate length of data to be written
	writtenLen += sizeof(gsi_u16) + sizeof(gsi_u16); // 2 byte key ID, 2 byte key type;
	writtenLen += sizeof(gsi_u16); // 2 bytes of data

	// Check room in buffer
	GS_ASSERT((theReport->mBuffer.mPos + writtenLen) < theReport->mBuffer.mCapacity);

	// Update count and length markers
	if (theReport->mReportState == SCIReportState_GLOBALDATA)
	{
		aHeader->mGameKeyCount++;
		aHeader->mGameSectionLength += writtenLen;
	}
	else if (theReport->mReportState == SCIReportState_PLAYERDATA)
	{
		GS_ASSERT(theReport->mCurEntityStartPos != -1);
		aHeader->mPlayerKeyCount++;
		aHeader->mPlayerSectionLength += writtenLen;
		
		theReport->mCurEntityKeyCount++;
		sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mCurEntityStartPos], (gsi_i16)theReport->mCurEntityKeyCount);
	}
	else if (theReport->mReportState == SCIReportState_TEAMDATA)
	{
		aHeader->mTeamKeyCount++;
		aHeader->mTeamSectionLength += writtenLen;

		theReport->mCurEntityKeyCount++;
		sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mCurEntityStartPos], (gsi_i16)theReport->mCurEntityKeyCount);
	}

	// Write the data
	sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mBuffer.mPos], (gsi_i16)theKeyId);
	theReport->mBuffer.mPos += sizeof(gsi_u16);
	sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mBuffer.mPos], theKeyType);
	theReport->mBuffer.mPos += sizeof(gsi_u16);
	sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mBuffer.mPos], theValue);
	theReport->mBuffer.mPos += sizeof(gsi_u16);

	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult SC_CALL sciReportAddByteValue(SCIReport * theReport,
									   gsi_u16     theKeyId,
									   gsi_i8      theValue)
{
	SCIReportHeader * aHeader = (SCIReportHeader*)theReport->mBuffer.mData;
	int writtenLen = 0;
	gsi_i16 theKeyType = SCDataType_BYTE;
	
	// calculate length of data to be written
	writtenLen += sizeof(gsi_u16) + sizeof(gsi_u16); // 2 byte key ID, 2 byte key type;
	writtenLen += sizeof(gsi_u8); // 1 bytes of data

	// Check room in buffer
	GS_ASSERT((theReport->mBuffer.mPos + writtenLen) < theReport->mBuffer.mCapacity);

	// Update count and length markers
	if (theReport->mReportState == SCIReportState_GLOBALDATA)
	{
		aHeader->mGameKeyCount++;
		aHeader->mGameSectionLength += writtenLen;
	}
	else if (theReport->mReportState == SCIReportState_PLAYERDATA)
	{
		GS_ASSERT(theReport->mCurEntityStartPos != -1);
		aHeader->mPlayerKeyCount++;
		aHeader->mPlayerSectionLength += writtenLen;
		
		theReport->mCurEntityKeyCount++;
		sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mCurEntityStartPos], (gsi_i16)theReport->mCurEntityKeyCount);
	}
	else if (theReport->mReportState == SCIReportState_TEAMDATA)
	{
		aHeader->mTeamKeyCount++;
		aHeader->mTeamSectionLength += writtenLen;

		theReport->mCurEntityKeyCount++;
		sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mCurEntityStartPos], (gsi_i16)theReport->mCurEntityKeyCount);
	}

	// Write the data
	sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mBuffer.mPos], (gsi_i16)theKeyId);
	theReport->mBuffer.mPos += sizeof(gsi_u16);
	sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mBuffer.mPos], theKeyType);
	theReport->mBuffer.mPos += sizeof(gsi_u16);
	sciSerializeInt8((gsi_u8 *)&theReport->mBuffer.mData[theReport->mBuffer.mPos], theValue);
	theReport->mBuffer.mPos += sizeof(gsi_u8);

	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult SC_CALL sciReportAddFloatValue(SCIReport * theReport,
									    gsi_u16     theKeyId,
									    float       theValue)
{
	SCIReportHeader * aHeader = (SCIReportHeader*)theReport->mBuffer.mData;
	int writtenLen = 0;
	gsi_i16 theKeyType = SCDataType_FLOAT;

	// calculate length of data to be written
	writtenLen += sizeof(gsi_u16) + sizeof(gsi_u16); // 2 byte key ID, 2 byte key type;
	writtenLen += sizeof(float);					 // stored in stream as a 4 byte char array

	// Check room in buffer
	GS_ASSERT((theReport->mBuffer.mPos + writtenLen) < theReport->mBuffer.mCapacity);

	// Update count and length markers
	if (theReport->mReportState == SCIReportState_GLOBALDATA)
	{
		aHeader->mGameKeyCount++;
		aHeader->mGameSectionLength += writtenLen;
	}
	else if (theReport->mReportState == SCIReportState_PLAYERDATA)
	{
		GS_ASSERT(theReport->mCurEntityStartPos != -1);
		aHeader->mPlayerKeyCount++;
		aHeader->mPlayerSectionLength += writtenLen;
		
		theReport->mCurEntityKeyCount++;
		sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mCurEntityStartPos], (gsi_i16)theReport->mCurEntityKeyCount);
	}
	else if (theReport->mReportState == SCIReportState_TEAMDATA)
	{
		aHeader->mTeamKeyCount++;
		aHeader->mTeamSectionLength += writtenLen;

		theReport->mCurEntityKeyCount++;
		sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mCurEntityStartPos], (gsi_i16)theReport->mCurEntityKeyCount);
	}
	// Needs real error handling code or an assertion on a non-constant expression
	//else 
	//	GS_ASSERT(0); // invalid state for writing key/value pairs!

	// Write the data
	sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mBuffer.mPos], (gsi_i16)theKeyId);
	theReport->mBuffer.mPos += sizeof(gsi_u16);
	sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mBuffer.mPos], theKeyType);
	theReport->mBuffer.mPos += sizeof(gsi_u16);
	sciSerializeFloat((gsi_u8 *)&theReport->mBuffer.mData[theReport->mBuffer.mPos], theValue);
	theReport->mBuffer.mPos += sizeof(float);

	return SCResult_NO_ERROR;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Some defines to support the 7-bit encoded int writer below.
#define MASK_7_BITS    0x7F
#define SET_MORE_BYTES 0x80
#define NEEDS_2_BYTES  0x00000080
#define NEEDS_3_BYTES  0x00000400
#define NEEDS_4_BYTES  0x00200000
#define NEEDS_5_BYTES  0x10000000

static void SevenBitEncoderWriteByte(char *dest, int value, int moreBytes)
{
	*dest = (char)value;
	if(moreBytes)
		*dest |= SET_MORE_BYTES;
}

static int SevenBitEncoderWriteInt(char *dest, int value)
{
	int moreBytes;

	// First byte.
	moreBytes = (value >= NEEDS_2_BYTES);
	if(dest != NULL) SevenBitEncoderWriteByte(dest, value & MASK_7_BITS, moreBytes);
	if(moreBytes)
	{
		// Second byte.
		moreBytes = (value >= NEEDS_3_BYTES);
		if(dest != NULL) SevenBitEncoderWriteByte(dest + 1, (value >> 7) & MASK_7_BITS, moreBytes);
		if(moreBytes)
		{
			// Third byte.
			moreBytes = (value >= NEEDS_4_BYTES);
			if(dest != NULL) SevenBitEncoderWriteByte(dest + 2, (value >> 14) & MASK_7_BITS, moreBytes);
			if(moreBytes)
			{
				// Fourth byte.
				moreBytes = (value >= NEEDS_5_BYTES);
				if(dest != NULL) SevenBitEncoderWriteByte(dest + 3, (value >> 21) & MASK_7_BITS, moreBytes);
				if(moreBytes)
				{
					// Fifth byte.
					if(dest != NULL) SevenBitEncoderWriteByte(dest + 4, (value >> 28) & MASK_7_BITS, 0);
					return 5;
				}
				else
					return 4;
			}
			else
				return 3;
		}
		else
			return 2;
	}
	else
		return 1;
}

SCResult SC_CALL sciReportAddStringValue(SCIReport *      theReport,
										 gsi_u16          theKeyId,
										 const gsi_char * theValue)
{
	SCIReportHeader * aHeader = (SCIReportHeader*)theReport->mBuffer.mData;
	int writtenLen = 0, numLenBytes = 0, asciiLen = 0;
	gsi_i16 theKeyType = SCDataType_STRING;	

	// for unicode we need to store the ascii-converted string first in the buffer so as
	// not to dynamically allocate space for it. Then once we have the length of it, we can
	// write over the buffer

#if defined(GSI_UNICODE)
	// First check room in buffer
	GS_ASSERT(((theReport->mBuffer.mPos + (_tcslen(theValue)) + (sizeof(gsi_u16)*2))
		< theReport->mBuffer.mCapacity));

	// as long as we have enough room - add to the buffer and store length of the Ascii string
	// need to remove 1 for the appended null char \0 which we want to remove
	asciiLen = UCS2ToAsciiString(theValue, &theReport->mBuffer.mData[theReport->mBuffer.mPos])-1;

	// now determine how many bytes are necessary to represent len
	numLenBytes = SevenBitEncoderWriteInt(NULL, asciiLen);

#else

	numLenBytes = SevenBitEncoderWriteInt(NULL, (int)strlen(theValue)); //finds out number of bytes necessary to represent len
	asciiLen = (int)strlen(theValue);
#endif

	// calculate length of data to be written
	writtenLen += sizeof(gsi_u16) + sizeof(gsi_u16);	// 2 byte key ID, 2 byte key type;
	writtenLen += asciiLen;								// 0 for empty string
	writtenLen += numLenBytes;							// the number of bytes necessary to represent len

	// Check room in buffer
	GS_ASSERT((theReport->mBuffer.mPos + writtenLen) < theReport->mBuffer.mCapacity);

	// Update count and length markers
	if (theReport->mReportState == SCIReportState_GLOBALDATA)
	{
		aHeader->mGameKeyCount++;
		aHeader->mGameSectionLength += writtenLen;
	}
	else if (theReport->mReportState == SCIReportState_PLAYERDATA)
	{
		aHeader->mPlayerKeyCount++;
		aHeader->mPlayerSectionLength += writtenLen;

		theReport->mCurEntityKeyCount++;
		sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mCurEntityStartPos], (gsi_i16)theReport->mCurEntityKeyCount);
	}
	else if (theReport->mReportState == SCIReportState_TEAMDATA)
	{
		aHeader->mTeamKeyCount++;
		aHeader->mTeamSectionLength += writtenLen;

		theReport->mCurEntityKeyCount++;
		sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mCurEntityStartPos], (gsi_i16)theReport->mCurEntityKeyCount);
	}
	// Needs real error handling code or an assertion on a non-constant expression
	//else 
	//	GS_ASSERT(0); // invalid state for writing key/value pairs!

	// Write the data
	sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mBuffer.mPos], (gsi_i16)theKeyId);
	theReport->mBuffer.mPos += sizeof(gsi_u16);
	sciSerializeInt16((gsi_u8 *)&theReport->mBuffer.mData[theReport->mBuffer.mPos], theKeyType);
	theReport->mBuffer.mPos += sizeof(gsi_u16);

	// now prior to writing the string we need to prepend the length of the string to follow
	// the .NET format of (string length)(String)
	SevenBitEncoderWriteInt(&theReport->mBuffer.mData[theReport->mBuffer.mPos], asciiLen);
	theReport->mBuffer.mPos += numLenBytes;

#if defined(GSI_UNICODE)
	// Now strip to Ascii and write the length - subtract 1 to get rid of appended null character
	theReport->mBuffer.mPos += UCS2ToAsciiString(theValue, &theReport->mBuffer.mData[theReport->mBuffer.mPos])-1;
#else
	strncpy(&theReport->mBuffer.mData[theReport->mBuffer.mPos], theValue, strlen(theValue));
	theReport->mBuffer.mPos += strlen(theValue);
#endif

	return SCResult_NO_ERROR;
}
