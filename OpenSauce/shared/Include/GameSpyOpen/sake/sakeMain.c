///////////////////////////////////////////////////////////////////////////////
// File:	sakeMain.c
// SDK:		GameSpy Sake Persistent Storage SDK
//
// Copyright (c) IGN Entertainment, Inc.  All rights reserved.  
// This software is made available only pursuant to certain license terms offered
// by IGN or its subsidiary GameSpy Industries, Inc.  Unlicensed use or use in a 
// manner not expressly authorized by IGN or GameSpy is prohibited.

#include "sakeMain.h"
#include "sakeRequest.h"
#include "../common/gsAvailable.h"
#include "../common/gsCore.h"

#ifdef _WIN32
#include <crtdbg.h>
#endif

///////////////////////////////////////////////////////////////////////////////
// General
///////////////////////////////////////////////////////////////////////////////
gsi_char gSakeUploadUrlOverride[SAKE_MAX_URL_LENGTH];
gsi_char gSakeDownloadUrlOverride[SAKE_MAX_URL_LENGTH];

#define SAKEI_FILE_DOWNLOAD_PARAMS  GS_STR _T("?fileid=%d&gameid=%d&pid=%d")
//#define SAKEI_FILE_DOWNLOAD_URL     _T("http://") GS_STR _T(".sake.gamespy.com/SakeFileServer/download.aspx?fileid=%d&gameid=%d&pid=%d")
#define SAKEI_FILE_DOWNLOAD_URL     _T("https://") GSI_OPEN_DOMAIN_NAME_UNI _T("/SakeFileServer/download.aspx?fileid=%d&gameid=%d&pid=%d")

#define SAKEI_FILE_UPLOAD_PARAMS    GS_USTR _T("?fileid=%d&gameid=%d&pid=%d")
//#define SAKEI_FILE_UPLOAD_URL       _T("http://") GS_STR _T(".sake.gamespy.com/SakeFileServer/upload.aspx?gameid=%d&pid=%d")
#define SAKEI_FILE_UPLOAD_URL       _T("https://") GSI_OPEN_DOMAIN_NAME_UNI _T("/SakeFileServer/upload.aspx?gameid=%d&pid=%d")

///////////////////////////////////////////////////////////////////////////////

SAKEStartupResult SAKE_CALL sakeStartup(SAKE * sakePtr)
{
	SAKE sake;

	GS_ASSERT(sakePtr);

	// check for availability
	if(__GSIACResult != GSIACAvailable)
		return SAKEStartupResult_NOT_AVAILABLE;

	// check that the core is initialized
	if(gsCoreIsShutdown())
		return SAKEStartupResult_CORE_SHUTDOWN;

	// allocate the sake object
	sake = (SAKE)gsimalloc(sizeof(SAKEInternal));
	if(sake == NULL)
		return SAKEStartupResult_OUT_OF_MEMORY;

	// init the sake object
	memset(sake, 0, sizeof(SAKEInternal));

	// store the object in the user pointer
	*sakePtr = sake;

	return SAKEStartupResult_SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SAKE_CALL sakeShutdown(SAKE sake)
{
	GS_ASSERT(sake);

	//TODO: ensure that there are no pending operations
	//      that might reference this object

	// free the struct
	gsifree(sake);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Authentication


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SAKE_CALL sakeSetGame(SAKE sake, const gsi_char * gameName, int gameId, const gsi_char *secretKey)
{
	GS_ASSERT(sake);
	GS_ASSERT(gameName && (_tcslen(gameName) <= SAKEI_GAME_NAME_LENGTH));
	GS_ASSERT(gameId >= 0);
	GS_ASSERT(secretKey && (_tcslen(secretKey) <= SAKEI_SECRET_KEY_LENGTH));

#ifdef GSI_UNICODE
	// convert gamename and secretkey to ascii for executing requests
	UCS2ToAsciiString(gameName, sake->mGameName);
	UCS2ToAsciiString(secretKey, sake->mSecretKey);
#else
	strcpy(sake->mGameName, gameName);
	strcpy(sake->mSecretKey, secretKey);
#endif

	sake->mGameId = gameId;
	sake->mIsGameAuthenticated = gsi_true;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SAKE_CALL sakeSetProfile(SAKE sake, int profileId, const GSLoginCertificate * certificate,
							  const GSLoginPrivateData * privateData)
{
	GS_ASSERT(sake);
	GS_ASSERT(certificate);
	GS_ASSERT(privateData);

	sake->mProfileId = profileId;
	memcpy(&sake->mCertificate, certificate, sizeof(GSLoginCertificate));
	memcpy(&sake->mPrivateData, privateData, sizeof(GSLoginPrivateData));
	sake->mIsProfileAuthenticated = gsi_true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Requests


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SAKEStartRequestResult SAKE_CALL sakeGetStartRequestResult(SAKE sake)
{
	GS_ASSERT(sake);

	return sake->mStartRequestResult;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static SAKERequest SAKE_CALL sakeiRunRequest(SAKE sake, 
                                             void *input,
											 SAKERequestCallback callback, 
                                             void *userData,
											 SAKEIRequestType type,
											 SAKEStartRequestResult (*startRequestFunc)(SAKERequest request))
{
	SAKERequest request;

	GS_ASSERT(sake);

	request = sakeiInitRequest(sake, type, input, callback, userData);
	if(!request)
		return NULL;

	sake->mStartRequestResult = startRequestFunc(request);
	if(sake->mStartRequestResult != SAKEStartRequestResult_SUCCESS)
	{
		sakeiFreeRequest(request);
		return NULL;
	}

	return request;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static SAKEStartRequestResult SAKE_CALL sakeiCancelRequest(SAKE sake, 
                                                           SAKERequest request)
{
    GS_ASSERT(sake);
    GS_ASSERT(request);
    GS_ASSERT(request->mInfo);
    GS_ASSERT(request->mInfo->mRequestToCancel);
    
    if (!sake || 
        !request ||
        !request->mInfo ||
        !request->mInfo->mRequestToCancel)
    {
        return SAKEStartRequestResult_BAD_INPUT;
    }
    gsiCoreCancelTask(request->mInfo->mRequestToCancel);
    
    return SAKEStartRequestResult_SUCCESS;

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SAKERequest SAKE_CALL sakeReportRecord(SAKE sake, SAKEReportRecordInput *input, SAKERequestCallback callback, void *userData)
{
	return sakeiRunRequest(sake, input, callback, userData, SAKEIRequestType_REPORT_RECORD, sakeiStartReportRecordRequest);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SAKERequest SAKE_CALL sakeCreateRecord(SAKE sake, SAKECreateRecordInput *input, SAKERequestCallback callback, void *userData)
{
	return sakeiRunRequest(sake, input, callback, userData, SAKEIRequestType_CREATE_RECORD, sakeiStartCreateRecordRequest);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SAKERequest SAKE_CALL sakeUpdateRecord(SAKE sake, SAKEUpdateRecordInput *input, SAKERequestCallback callback, void *userData)
{
	return sakeiRunRequest(sake, input, callback, userData, SAKEIRequestType_UPDATE_RECORD, sakeiStartUpdateRecordRequest);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SAKERequest SAKE_CALL sakeDeleteRecord(SAKE sake, SAKEDeleteRecordInput *input, SAKERequestCallback callback, void *userData)
{
	return sakeiRunRequest(sake, input, callback, userData, SAKEIRequestType_DELETE_RECORD, sakeiStartDeleteRecordRequest);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SAKERequest SAKE_CALL sakeSearchForRecords(SAKE sake, SAKESearchForRecordsInput *input, SAKERequestCallback callback, void *userData)
{
	return sakeiRunRequest(sake, input, callback, userData, SAKEIRequestType_SEARCH_FOR_RECORDS, sakeiStartSearchForRecordsRequest);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SAKERequest SAKE_CALL sakeGetMyRecords(SAKE sake, SAKEGetMyRecordsInput *input, SAKERequestCallback callback, void *userData)
{
	return sakeiRunRequest(sake, input, callback, userData, SAKEIRequestType_GET_MY_RECORDS, sakeiStartGetMyRecordsRequest);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SAKERequest SAKE_CALL sakeGetSpecificRecords(SAKE sake, SAKEGetSpecificRecordsInput *input, SAKERequestCallback callback, void *userData)
{
	return sakeiRunRequest(sake, input, callback, userData, SAKEIRequestType_GET_SPECIFIC_RECORDS, sakeiStartGetSpecificRecordsRequest);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SAKERequest SAKE_CALL sakeGetRandomRecord(SAKE sake, SAKEGetRandomRecordInput *input, SAKERequestCallback callback, void *userData)
{
	return sakeiRunRequest(sake, input, callback, userData, SAKEIRequestType_GET_RANDOM_RECORD, sakeiStartGetRandomRecordRequest);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SAKERequest SAKE_CALL sakeRateRecord(SAKE sake, SAKERateRecordInput *input, SAKERequestCallback callback, void *userData)
{
	return sakeiRunRequest(sake, input, callback, userData, SAKEIRequestType_RATE_RECORD, sakeiStartRateRecordRequest);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SAKERequest SAKE_CALL sakeGetRecordLimit(SAKE sake, SAKEGetRecordLimitInput *input, SAKERequestCallback callback, void *userData)
{
	return sakeiRunRequest(sake, input, callback, userData, SAKEIRequestType_GET_RECORD_LIMIT, sakeiStartGetRecordLimitRequest);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SAKERequest SAKE_CALL sakeGetRecordCount(SAKE sake, SAKEGetRecordCountInput *input, SAKERequestCallback callback, void *userData)
{
	return sakeiRunRequest(sake, input, callback, userData, SAKEIRequestType_GET_RECORD_COUNT, sakeiStartGetRecordCountRequest);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// read request utility
SAKEField * SAKE_CALL sakeGetFieldByName(const char *name, SAKEField *fields, int numFields)
{
	int i;

	GS_ASSERT(name);
	GS_ASSERT(fields);
	GS_ASSERT(numFields >= 0);

	for(i = 0 ; i < numFields ; i++)
	{
		if(strcmp(fields[i].mName, name) == 0)
			return &fields[i];
	}

	return NULL;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Files
//
// Used by the Upload/Download Content API functions.
///////////////////////////////////////////////////////////////////////////////
gsi_bool SAKE_CALL sakeSetFileUploadURL(SAKE sake, gsi_char url[SAKE_MAX_URL_LENGTH])
{
    GS_ASSERT(sake);
    GS_ASSERT(url);

    if(!sake || !url)
        return gsi_false;

    _tcscpy(gSakeUploadUrlOverride, url);
    return gsi_true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
gsi_bool SAKE_CALL sakeGetFileUploadURL(SAKE sake, gsi_char url[SAKE_MAX_URL_LENGTH])
{
    int rcode;

    GS_ASSERT(sake);
    GS_ASSERT(url);
    GS_ASSERT(sake->mIsGameAuthenticated);
    GS_ASSERT(sake->mIsProfileAuthenticated);

    if(!sake || !url || !sake->mIsGameAuthenticated || !sake->mIsProfileAuthenticated)
        return gsi_false;

    if (gSakeUploadUrlOverride[0] != '\0')
        rcode = _tsnprintf(url, SAKE_MAX_URL_LENGTH, SAKEI_FILE_UPLOAD_PARAMS, gSakeUploadUrlOverride, 0, sake->mGameId, sake->mProfileId); //NAB fix
    else
	{
#ifdef GSI_UNICODE
		gsi_char uniGameName[SAKEI_GAME_NAME_LENGTH+1];
		AsciiToUCS2String(sake->mGameName, uniGameName);
		rcode = _tsnprintf(url, SAKE_MAX_URL_LENGTH, SAKEI_FILE_UPLOAD_URL, uniGameName, sake->mGameId, sake->mProfileId);
#else
        rcode = _tsnprintf(url, SAKE_MAX_URL_LENGTH, SAKEI_FILE_UPLOAD_URL, sake->mGameName, sake->mGameId, sake->mProfileId);
#endif
	}

    if(rcode < 0)
        return gsi_false;
    return gsi_true;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Set the URL format to be used by sakeGetFileDownloadUrl
gsi_bool SAKE_CALL sakeSetFileDownloadURL(SAKE sake, gsi_char url[SAKE_MAX_URL_LENGTH])
{
    GS_ASSERT(sake);
    GS_ASSERT(url);

    if(!sake || !url)
        return gsi_false;

    _tcscpy(gSakeDownloadUrlOverride, url);
    return gsi_true;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
gsi_bool SAKE_CALL sakeGetFileDownloadURL(SAKE sake, int fileId, gsi_char url[SAKE_MAX_URL_LENGTH])
{
    int rcode;

    GS_ASSERT(sake);
    GS_ASSERT(fileId != 0);
    GS_ASSERT(url);
    GS_ASSERT(sake->mIsGameAuthenticated);
    GS_ASSERT(sake->mIsProfileAuthenticated);

    if(!sake || !url || !sake->mIsGameAuthenticated || !sake->mIsProfileAuthenticated)
        return gsi_false;

    if (gSakeDownloadUrlOverride[0] != '\0')
    {
        rcode = _tsnprintf(url, SAKE_MAX_URL_LENGTH, SAKEI_FILE_DOWNLOAD_PARAMS,
            gSakeDownloadUrlOverride, fileId, sake->mGameId, sake->mProfileId);
    }
    else
	{
#ifdef GSI_UNICODE
		gsi_char uniGameName[SAKEI_GAME_NAME_LENGTH+1];
		AsciiToUCS2String(sake->mGameName, uniGameName);
		rcode = _tsnprintf(url, SAKE_MAX_URL_LENGTH, SAKEI_FILE_DOWNLOAD_URL, uniGameName, fileId, sake->mGameId, sake->mProfileId);
#else
		rcode = _tsnprintf(url, SAKE_MAX_URL_LENGTH, SAKEI_FILE_DOWNLOAD_URL, sake->mGameName, fileId, sake->mGameId, sake->mProfileId);
#endif
	}

    if(rcode < 0)
        return gsi_false;
    return gsi_true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static SAKEFileResult SAKE_CALL sakeiParseFileResult(int resultCode)
{
	if(resultCode >= SAKEFileResult_UNKNOWN_ERROR)
		return SAKEFileResult_UNKNOWN_ERROR;
	return (SAKEFileResult)resultCode;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static gsi_bool SAKE_CALL sakeiGetHeaderValueInt(const char *headers, const char *headerName, int *value)
{
	const char * header;
	int rcode;

	GS_ASSERT(headers);
	GS_ASSERT(headerName);
	GS_ASSERT(value);
#ifdef _DEBUG
	// headerName must include the trailing colon
	GS_ASSERT(headerName[strlen(headerName) - 1] == ':');
#endif

	// find this header in the list of headers
	header = strstr(headers, headerName);
	if(header)
	{
		// skip the header name
		header += strlen(headerName);

		// scan in the result
		rcode = sscanf(header, " %d", value);
		if(rcode == 1)
			return gsi_true;
	}

	return gsi_false;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
gsi_bool SAKE_CALL sakeGetFileResultFromHeaders(const char *headers, SAKEFileResult *result)
{
	int resultCode;
	gsi_bool foundResultCode;

	foundResultCode = sakeiGetHeaderValueInt(headers, SAKE_FILE_RESULT_HEADER, &resultCode);

	if(gsi_is_false(foundResultCode))
		return gsi_false;

	*result = sakeiParseFileResult(resultCode);
	return gsi_true;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
gsi_bool SAKE_CALL sakeGetFileIdFromHeaders(const char *headers, int *fileId)
{
	return sakeiGetHeaderValueInt(headers, SAKE_FILE_ID_HEADER, fileId);
}


SAKEStartRequestResult SAKE_CALL sakeDownloadContent( SAKE sake, 
                                                      SAKEDownloadContentInput *input, 
                                                      SAKEDownloadContentCompletedCallback completedCallback, 
                                                      void *userData )
{
	
	GS_ASSERT(sake);
    GS_ASSERT(input);
	GS_ASSERT(input->content);
    GS_ASSERT(input->content->mFileid);
	GS_ASSERT(completedCallback);
	
	return sakeiDownloadContent(sake, input,completedCallback,userData);
}

SAKEStartRequestResult SAKE_CALL sakeUploadContent( SAKE sake, 
                                                   SAKEUploadContentInput *input,
                                                   SAKEUploadContentCompletedCallback completedCallback, 
                                                   void *userData )
{

	GS_ASSERT(sake);
	GS_ASSERT(input->remoteFileName);
	GS_ASSERT(completedCallback);
	
	return sakeiUploadContent(sake,input,completedCallback,userData );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Cancel Request
SAKEStartRequestResult SAKE_CALL sakeCancelRequest(SAKE sake, SAKERequest request)
{
    return sakeiCancelRequest(sake, request);
    
    
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void sakeThink()
{
    ghttpThink();
    gsCoreThink(0);
}
