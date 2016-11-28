///////////////////////////////////////////////////////////////////////////////
// File:	sakeRequest.c
// SDK:		GameSpy Sake Persistent Storage SDK
//
// Copyright (c) IGN Entertainment, Inc.  All rights reserved.  
// This software is made available only pursuant to certain license terms offered
// by IGN or its subsidiary GameSpy Industries, Inc.  Unlicensed use or use in a 
// manner not expressly authorized by IGN or GameSpy is prohibited.

#include "sakeRequest.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
const char * GSI_SAKE_SERVICE_NAMESPACES[GSI_SAKE_SERVICE_NAMESPACE_COUNT] =
{
	"ns1=\"http://gamespy.net/sake\""
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
char sakeiSoapUrl[SAKE_MAX_URL_LENGTH] = "";


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static SAKEStartRequestResult sakeiTranslateGhttpResult( GHTTPRequest ghttpResult)
{
#ifdef GHTTP_EXTENDEDERROR
    switch (ghttpResult)
    {
    case GHTTPSuccess:
        return SAKEStartRequestResult_SUCCESS;
    case GHTTPErrorStart:
        return SAKEStartRequestResult_HTTP_ERROR;
    case GHTTPFailedToOpenFile:
        return SAKEStartRequestResult_FILE_NOT_FOUND;
    case GHTTPInvalidPost:
        return SAKEStartRequestResult_HTTP_INVALID_POST;
    case GHTTPInsufficientMemory:
        return SAKEStartRequestResult_OUT_OF_MEMORY;
    case GHTTPInvalidFileName:
        return SAKEStartRequestResult_BAD_FIELD_NAME;
    case GHTTPInvalidBufferSize:
        return SAKEStartRequestResult_HTTP_INVALID_BUFFERSIZE;
    case GHTTPInvalidURL:
        return SAKEStartRequestResult_HTTP_INVALID_URL;
    case GHTTPUnspecifiedError:
        return SAKEStartRequestResult_HTTP_ERROR;
    default:
        return SAKEStartRequestResult_UNKNOWN_ERROR;
    }
#else
    if (!IS_GHTTP_ERROR(ghttpResult))
        return SAKEStartRequestResult_SUCCESS;
    else
        return SAKEStartRequestResult_HTTP_ERROR;
#endif
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

SAKERequest SAKE_CALL sakeiInitRequest(SAKE sake, SAKEIRequestType type, void *input, SAKERequestCallback callback, void *userData)
{
	SAKERequest request;

	GS_ASSERT(sake);

	// init the request init result to success
	sake->mStartRequestResult = SAKEStartRequestResult_SUCCESS;

	// check for authentication
	if(gsi_is_false(sake->mIsGameAuthenticated) || gsi_is_false(sake->mIsProfileAuthenticated))
	{
		sake->mStartRequestResult = SAKEStartRequestResult_NOT_AUTHENTICATED;
		return NULL;
	}

	// allocate memory for the request object
	request = (SAKERequest)gsimalloc(sizeof(SAKERequestInternal));
	if(!request)
	{
		sake->mStartRequestResult = SAKEStartRequestResult_OUT_OF_MEMORY;
		return NULL;
	}

	// init the request object
	memset(request, 0, sizeof(SAKERequestInternal));
	request->mSake = sake;
	request->mType = type;
	request->mInput = input;
	request->mCallback = callback;
	request->mUserData = userData;

	return request;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SAKE_CALL sakeiFreeRequest(SAKERequest request)
{
	GS_ASSERT(request);

	// this should already be freed by the time we get here
	GS_ASSERT(request->mOutput == NULL);

	// free the request
	gsifree(request);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static SAKERequestResult SAKE_CALL sakeiCheckHttpResult(GHTTPResult httpResult)
{
	switch(httpResult)
	{
	case GHTTPSuccess:
		return SAKERequestResult_SUCCESS;
	case GHTTPOutOfMemory:
		return SAKERequestResult_OUT_OF_MEMORY;
	case GHTTPRequestCancelled:
	    return SAKERequestResult_REQUEST_CANCELLED;
	default:
		return SAKERequestResult_CONNECTION_ERROR;
	}
}

static SAKERequestResult SAKE_CALL sakeiCheckSakeResult(const char * sakeResult)
{
	if(strcmp(sakeResult, "Success") == 0)
		return SAKERequestResult_SUCCESS;
	else if(strcmp(sakeResult, "SecretKeyInvalid") == 0)
		return SAKERequestResult_SECRET_KEY_INVALID;
	else if(strcmp(sakeResult, "ServiceDisabled") == 0)
		return SAKERequestResult_SERVICE_DISABLED;
	else if(strcmp(sakeResult, "DatabaseUnavailable") == 0)
		return SAKERequestResult_DATABASE_UNAVAILABLE;
	else if(strcmp(sakeResult, "LoginTicketInvalid") == 0)
		return SAKERequestResult_LOGIN_TICKET_INVALID;
	else if(strcmp(sakeResult, "LoginTicketExpired") == 0)
		return SAKERequestResult_LOGIN_TICKET_EXPIRED;
	else if(strcmp(sakeResult, "TableNotFound") == 0)
		return SAKERequestResult_TABLE_NOT_FOUND;
	else if(strcmp(sakeResult, "RecordNotFound") == 0)
		return SAKERequestResult_RECORD_NOT_FOUND;
	else if(strcmp(sakeResult, "FieldNotFound") == 0)
		return SAKERequestResult_FIELD_NOT_FOUND;
	else if(strcmp(sakeResult, "FieldTypeInvalid") == 0)
		return SAKERequestResult_FIELD_TYPE_INVALID;
	else if(strcmp(sakeResult, "NoPermission") == 0)
		return SAKERequestResult_NO_PERMISSION;
	else if(strcmp(sakeResult, "RecordLimitReached") == 0)
		return SAKERequestResult_RECORD_LIMIT_REACHED;
	else if(strcmp(sakeResult, "AlreadyRated") == 0)
		return SAKERequestResult_ALREADY_RATED;
	else if(strcmp(sakeResult, "NotRateable") == 0)
		return SAKERequestResult_NOT_RATEABLE;
	else if(strcmp(sakeResult, "NotOwned") == 0)
		return SAKERequestResult_NOT_OWNED;
	else if(strcmp(sakeResult, "FilterInvalid") == 0)
		return SAKERequestResult_FILTER_INVALID;
	else if(strcmp(sakeResult, "SortInvalid") == 0)
		return SAKERequestResult_SORT_INVALID;
	else if(strcmp(sakeResult, "TargetFilterInvalid") == 0)
		return SAKERequestResult_TARGET_FILTER_INVALID;
	else if(strcmp(sakeResult, "CertificateInvalid") == 0)
		return SAKERequestResult_CERTIFICATE_INVALID;
	else if (strcmp(sakeResult, "AlreadyReported") == 0)
		return SAKERequestResult_ALREADY_REPORTED;
	else
		return SAKERequestResult_UNKNOWN_ERROR;
}

// convert the common error code enum (for response header errors) to Sake error  
static SAKERequestResult sakeiAuthErrorToSakeRequestResult(GSAuthErrorCode errorCode) 
{
	SAKERequestResult result = SAKERequestResult_SUCCESS;

	switch(errorCode)
	{
		case GSAuthErrorCode_InvalidGameID: 
			result = SAKERequestResult_INVALID_GAMEID;
			break;
		case GSAuthErrorCode_InvalidSessionToken: 
			result = SAKERequestResult_INVALID_SESSIONTOKEN;
			break;
		case GSAuthErrorCode_SessionTokenExpired: 
			result = SAKERequestResult_SESSIONTOKEN_EXPIRED;
			break;
		default:
			result = SAKERequestResult_UNKNOWN_ERROR;
			break;
	}

	// reset AuthError for subsequent calls
	gsiCoreSetAuthError("");
	gsiCoreSetAuthErrorCode(0);

	return result;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void SAKE_CALL sakeiSoapCallback(GHTTPResult httpResult, GSXmlStreamWriter requestData, GSXmlStreamReader responseData, void *userData)
{
	SAKERequest request = (SAKERequest)userData;
	void *output = NULL;
	SAKERequestResult result;
	char resultString[32];

	// sanity check
	GS_ASSERT(request);
	GS_ASSERT(request->mSake);
	GS_ASSERT(request->mInfo);
	if(!request || !request->mSake || !request->mInfo)
		return;

	result = sakeiCheckHttpResult(httpResult);
	if(result == SAKERequestResult_SUCCESS)
	{
		if (gsiCoreGetAuthErrorCode() == GSAuthErrorCode_None)
		{
			if (gsi_is_false(gsXmlMoveToStart(responseData)) ||
			   gsi_is_false(gsXmlMoveToNext(responseData, request->mInfo->mResponseTag)) ||
			   gsi_is_false(gsXmlReadChildAsStringNT(responseData, request->mInfo->mResultTag, resultString, sizeof(resultString))))
			{
				result = SAKERequestResult_MALFORMED_RESPONSE;
			}
			else
			{
				result = sakeiCheckSakeResult(resultString);

				// fill in the output
				if(result == SAKERequestResult_SUCCESS)
				{
					if(request->mInfo->mSakeOutputSize != 0)
					{
						request->mOutput = gsimalloc(request->mInfo->mSakeOutputSize);
						if(request->mOutput)
						{
							request->mSoapResponse = responseData;
							result = request->mInfo->mProcessSoapResponseFunc(request);
							if(result == SAKERequestResult_SUCCESS)
								output = request->mOutput;
						}
						else
						{
							result = SAKERequestResult_OUT_OF_MEMORY;
						}
					}
				}
			}
		}
		else
		{
			result = sakeiAuthErrorToSakeRequestResult(gsiCoreGetAuthErrorCode());
		}
	}

	// call the callback
	if(request->mCallback)
		request->mCallback(request->mSake, request, result, request->mInput, output, request->mUserData);

	// free data
	if(request->mInfo->mFreeDataFunc)
		request->mInfo->mFreeDataFunc(request);

	// free the output data
	gsifree(request->mOutput);
	request->mOutput = NULL;

	// free the sake request
	sakeiFreeRequest(request);

	GSI_UNUSED(requestData);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static SAKEStartRequestResult SAKE_CALL sakeiSetupRequest(SAKERequest request)
{
	SAKEStartRequestResult result;
	SAKEIRequestInfo * info;

	GS_ASSERT(request);
	GS_ASSERT(request->mSake);
	GS_ASSERT(request->mInfo);

	// store a utility pointer to the info
	info = request->mInfo;

    if ((info->mValidateInputFunc != NULL) && (request->mInput != NULL))
    {
	    // check the input
	    result = info->mValidateInputFunc(request);
	    if(result != SAKEStartRequestResult_SUCCESS)
        {		
            if(info->mFreeDataFunc)
            {
                    info->mFreeDataFunc(request);
            }
		    return result;
        }
    }

	// create the xml request stream
	request->mSoapRequest = gsXmlCreateStreamWriter(GSI_SAKE_SERVICE_NAMESPACES, GSI_SAKE_SERVICE_NAMESPACE_COUNT);
	if(request->mSoapRequest == NULL)
		return SAKEStartRequestResult_OUT_OF_MEMORY;

	// open the stream
	gsXmlWriteOpenTag(request->mSoapRequest, GSI_SAKE_SERVICE_NAMESPACE, request->mInfo->mFuncName);

	// this info is included with every request
	gsXmlWriteIntElement(request->mSoapRequest, GSI_SAKE_SERVICE_NAMESPACE, "gameid", (gsi_u32)request->mSake->mGameId);
	gsXmlWriteOpenTag(request->mSoapRequest, GSI_SAKE_SERVICE_NAMESPACE, "certificate");
	wsLoginCertWriteXML(&request->mSake->mCertificate, GSI_SAKE_SERVICE_NAMESPACE, request->mSoapRequest);
	gsXmlWriteCloseTag(request->mSoapRequest, GSI_SAKE_SERVICE_NAMESPACE, "certificate");
	gsXmlWriteHexBinaryElement(request->mSoapRequest, GSI_SAKE_SERVICE_NAMESPACE, "proof", (const gsi_u8*)(&request->mSake->mPrivateData)->mKeyHash, GS_CRYPT_MD5_HASHSIZE);

	// fill in the request data
	result = info->mFillSoapRequestFunc(request);
	if(result != SAKEStartRequestResult_SUCCESS)
	{
		gsXmlFreeWriter(request->mSoapRequest);
		request->mSoapRequest = NULL;
		if(info->mFreeDataFunc)
			info->mFreeDataFunc(request);
		return result;
	}

	// close the stream and writer
	gsXmlWriteCloseTag(request->mSoapRequest, GSI_SAKE_SERVICE_NAMESPACE, request->mInfo->mFuncName);
	gsXmlCloseWriter(request->mSoapRequest);

	return SAKEStartRequestResult_SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void SAKE_CALL sakeiExecuteRequest(SAKERequest request)
{
	GSSoapTask *soapTask;
	
	if(sakeiSoapUrl[0] == '\0')
	{
		int rcode;
		rcode = snprintf(sakeiSoapUrl, SAKE_MAX_URL_LENGTH, SAKEI_SOAP_URL_FORMAT, request->mSake->mGameName);
		GS_ASSERT(rcode >= 0);
		GSI_UNUSED(rcode);
	}
	soapTask = gsiExecuteSoap(sakeiSoapUrl, request->mInfo->mSoapAction, request->mSoapRequest, sakeiSoapCallback, request);
	
	if (soapTask != NULL)
	{
	    request->mInfo->mRequestToCancel = soapTask->mCoreTask;
	}
	else
	{
	    request->mInfo->mRequestToCancel = NULL;
	}
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SAKEStartRequestResult SAKE_CALL sakeiStartRequest(SAKERequest request, SAKEIRequestInfo * info)
{
	SAKEStartRequestResult result;

	request->mInfo = info;

	result = sakeiSetupRequest(request);
	if(result != SAKEStartRequestResult_SUCCESS)
		return result;

	sakeiExecuteRequest(request);

	return SAKEStartRequestResult_SUCCESS;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void sakeiDownloadContentProgressCallback(GHTTPRequest   request,
                                          GHTTPState     state,
                                          const char     *buffer,
                                          GHTTPByteCount bufferLen,
                                          GHTTPByteCount bytesTransfered,
                                          GHTTPByteCount totalSize,
                                          void           *param)
{
    // Check progress and call any developer callbacks

    SAKEInternalDownloadData *downloadData;
    downloadData = (SAKEInternalDownloadData *)param;

    GS_ASSERT(downloadData);

    if (state == GHTTPReceivingFile)
    {	
        // Check if we have a pointer to developer's progress callback 
        if (downloadData->mProgressCallback)
        {
            downloadData->mProgressCallback( downloadData->mSake, (gsi_u32)bytesTransfered, (gsi_u32)totalSize, downloadData->mUserData);

        }
    }

    GSI_UNUSED(request);
    GSI_UNUSED(bufferLen);
    GSI_UNUSED(buffer);
    GSI_UNUSED(buffer);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static GHTTPBool sakeiDownloadContentCompletedCallback(GHTTPRequest   request, 
                                                GHTTPResult    httpResult, 
                                                char           *buffer, 
                                                GHTTPByteCount bufferLength, 
												char           *headers,
                                                void           *param)
{
    GHTTPBool      freeBuffer = GHTTPTrue; // When downloading memory and on error, this callback tells the http to release the buffer
	SAKEFileResult fileResult;
	SAKERequestResult result = sakeiCheckHttpResult(httpResult);
	SAKEInternalDownloadData *downloadData;
	GSAuthErrorCode errorCode;
	char authError[AUTHERROR_LENGTH];

	downloadData = (SAKEInternalDownloadData *)param;

	// check response headers for error
	errorCode = gsiSoapGetAuthErrorFromHeaders(headers, authError);
	if (errorCode != GSAuthErrorCode_None)
	{
		gsiCoreSetAuthError(authError);
		gsiCoreSetAuthErrorCode(errorCode);
	}

	if(result != SAKERequestResult_SUCCESS)
	{
		downloadData->mCompletedCallback(downloadData->mSake, result, SAKEFileResult_SERVER_ERROR, buffer, (gsi_i32) bufferLength, downloadData->mUserData);
		gsifree(downloadData);
		return freeBuffer;
	}
	else if(gsiCoreGetAuthErrorCode() != GSAuthErrorCode_None)
	{
		result = sakeiAuthErrorToSakeRequestResult(gsiCoreGetAuthErrorCode());
		downloadData->mCompletedCallback(downloadData->mSake, result, SAKEFileResult_SERVER_ERROR, buffer, (gsi_i32) bufferLength, downloadData->mUserData);
		gsifree(downloadData);
		return freeBuffer;
	}

	if(!sakeGetFileResultFromHeaders(ghttpGetHeaders(request), &fileResult))
	{
		downloadData->mCompletedCallback(downloadData->mSake, SAKERequestResult_MALFORMED_RESPONSE, SAKEFileResult_SERVER_ERROR, buffer, (gsi_i32) bufferLength, downloadData->mUserData);
		gsifree(downloadData);
		return freeBuffer;
	}

	if(fileResult != SAKEFileResult_SUCCESS)
	{
		downloadData->mCompletedCallback(downloadData->mSake, SAKERequestResult_CONTENTSERVER_FAILURE, fileResult, buffer, (gsi_i32) bufferLength,downloadData->mUserData);
		gsifree(downloadData);
		return freeBuffer;
	}

	downloadData->mCompletedCallback(downloadData->mSake, result, fileResult, buffer, (gsi_i32) bufferLength, downloadData->mUserData);
	gsifree(downloadData);

	return GHTTPFalse;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SAKEStartRequestResult SAKE_CALL sakeiDownloadContent( SAKE sake, 
                                                      SAKEDownloadContentInput *input, 
                                                      SAKEDownloadContentCompletedCallback completedCallback, 
                                                      void *userData )
{
    SAKEInternalDownloadData *downloadData;
	GHTTPRequest request;

	// for extra request headers for apigee
	char headerString[HEADERS_LENGTH];
	char sessionToken[SESSIONTOKEN_LENGTH];
	char gameIdString[GAMEID_LENGTH];
	char profileIdString[PROFILEID_LENGTH];

#ifdef GSI_UNICODE
	gsi_char uniHeaderString[HEADERS_LENGTH]; // for when GSI_UNICODE is defined
#endif

	gsi_char *downloadUrl = NULL;

	downloadUrl = (gsi_char *)gsimalloc(sizeof(gsi_char) * SAKE_MAX_URL_LENGTH);
	if(downloadUrl==NULL || 
       gsi_is_false(sakeGetFileDownloadURL(sake, (int)input->content->mFileid, downloadUrl)))
	{
        gsifree(downloadUrl);
		return SAKEStartRequestResult_HTTP_INVALID_URL;
	}

	downloadData = (SAKEInternalDownloadData *) malloc(sizeof(SAKEInternalDownloadData));
	if(downloadData==NULL)
	{
        gsifree(downloadUrl);
		return SAKEStartRequestResult_OUT_OF_MEMORY;
	}
    downloadData->mProgressCallback  = input->progressCallback ;
	downloadData->mCompletedCallback = completedCallback;
	downloadData->mSake = sake;
	downloadData->mUserData = userData;

	// make sure to include session token (and other stuff) in headers if we have one set 
	// (needed for apigee verification)
	strcpy(headerString, "");
	if (gsiCoreGetSessionToken(sessionToken))
	{
		strcat(headerString, SESSIONTOKEN_HEADER " ");
		strcat(headerString, sessionToken);
	}
	if (gsiCoreGetGameId(gameIdString))
	{
		strcat(headerString, "\r\n" GAMEID_HEADER " ");
		strcat(headerString, gameIdString);
	}
	if (gsiCoreGetProfileId(profileIdString))
	{
		strcat(headerString, "\r\n" PROFILEID_HEADER " ");
		strcat(headerString, profileIdString);
	}
	strcat(headerString, "\r\n"); // it seems http requires trailing carriage return when no body is present

	if(input->content->mType==SAKEContentStorageType_MEMORY)
	{   

#ifdef GSI_UNICODE
		AsciiToUCS2String(headerString, uniHeaderString);
		request = ghttpGetEx(downloadUrl,   // The download URL
			uniHeaderString,          // Any extra header info    
			input->content->mStorage.mMemory->mBuffer,  // Buffer to download to    
			input->content->mStorage.mMemory->mLength,  // Buffer size in bytes after the download
			NULL,                                  // pointer to Http Post, if any
			GHTTPFalse,                            // No throttle
			(GHTTPBool)input->transferBlocking,               // This is a blocking call
			sakeiDownloadContentProgressCallback,  // pointer to the Progress Callback function
			sakeiDownloadContentCompletedCallback, // pointer to the Complete Callback function 
			downloadData); // Any parameters to be passed to the callbacks.
#else
		request = ghttpGetEx(downloadUrl,   // The download URL
                             headerString,          // Any extra header info    
                             input->content->mStorage.mMemory->mBuffer,  // Buffer to download to    
                             input->content->mStorage.mMemory->mLength,  // Buffer size in bytes after the download
                             NULL,                                  // pointer to Http Post, if any
                             GHTTPFalse,                            // No throttle
                             (GHTTPBool)input->transferBlocking,               // This is a blocking call
                             sakeiDownloadContentProgressCallback,  // pointer to the Progress Callback function
                             sakeiDownloadContentCompletedCallback, // pointer to the Complete Callback function 
                             downloadData); // Any parameters to be passed to the callbacks.
#endif
	}
	else 
	{
		if(input->content->mStorage.mFile==NULL)
        {
            gsifree(downloadUrl);
            gsifree(downloadData);
			return SAKEStartRequestResult_INVALID_DATA;
        }
#ifdef GSI_UNICODE
		AsciiToUCS2String(headerString, uniHeaderString);
		request = ghttpSaveEx(downloadUrl,   // The download URL
			input->content->mStorage.mFile,  // The path to the downloaded filename
			uniHeaderString,          // Any extra header info    
			NULL,          // pointer to Http Post, if any
			GHTTPFalse,    // No throttle
			(GHTTPBool)input->transferBlocking,     // This is a blocking call
			sakeiDownloadContentProgressCallback,          // pointer to the Progress Callback function
			sakeiDownloadContentCompletedCallback, // pointer to the Complete Callback function 
			downloadData); // Any parameters to be passed to the callbacks.
#else
		request = ghttpSaveEx(downloadUrl,   // The download URL
                              input->content->mStorage.mFile,  // The path to the downloaded filename
                              headerString,          // Any extra header info    
                              NULL,          // pointer to Http Post, if any
                              GHTTPFalse,    // No throttle
                              (GHTTPBool)input->transferBlocking,     // This is a blocking call
                              sakeiDownloadContentProgressCallback,          // pointer to the Progress Callback function
                              sakeiDownloadContentCompletedCallback, // pointer to the Complete Callback function 
                              downloadData); // Any parameters to be passed to the callbacks.
#endif
	}

    gsifree(downloadUrl);

    return sakeiTranslateGhttpResult(request);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void sakeiUploadContentProgressCallback(GHTTPRequest   request,
                                      GHTTPState     state,
                                      const char     *buffer,
                                      GHTTPByteCount bufferLen,
                                      GHTTPByteCount bytesTransfered,
                                      GHTTPByteCount totalSize,
                                      void           *param)
{
    // Check progress and call any developer callbacks

    SAKEInternalUploadData *uploadData;
    uploadData = (SAKEInternalUploadData *)param;

    GS_ASSERT(uploadData);

    if (state == GHTTPReceivingFile)
    {	
        // Check if we have a pointer to developer's progress callback 
        if (uploadData->mProgressCallback)
        {
            uploadData->mProgressCallback( uploadData->mSake, (gsi_u32)bytesTransfered, (gsi_u32)totalSize, uploadData->mUserData);

        }
    }

    GSI_UNUSED(request);
    GSI_UNUSED(bufferLen);
    GSI_UNUSED(buffer);
    GSI_UNUSED(buffer);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static GHTTPBool sakeiUploadContentCompleteCallback(GHTTPRequest request, 
                                           GHTTPResult httpResult, 
                                           char *buffer, 
                                           GHTTPByteCount bufferLength,
										   char *headers,
                                           void *param)
{

    GHTTPBool      freeBuffer = GHTTPFalse ; // Let the user cleanup his buffers
	SAKEFileResult fileResult;
	SAKERequestResult result = sakeiCheckHttpResult(httpResult);
	gsi_i32 fileId;
	SAKEInternalUploadData *uploadData;
	GSAuthErrorCode errorCode;
	char authError[AUTHERROR_LENGTH];

	uploadData = (SAKEInternalUploadData *)param;

	// check response headers for error
	errorCode = gsiSoapGetAuthErrorFromHeaders(headers, authError);
	if (errorCode != GSAuthErrorCode_None)
	{
		gsiCoreSetAuthError(authError);
		gsiCoreSetAuthErrorCode(errorCode);
	}

	if(result != SAKERequestResult_SUCCESS)
	{
		uploadData->mCompletedCallback(uploadData->mSake, result, 0, SAKEFileResult_SERVER_ERROR, uploadData->mUserData);
		gsifree(uploadData);
		return freeBuffer;
	}
	else if(gsiCoreGetAuthErrorCode() != GSAuthErrorCode_None)
	{
		result = sakeiAuthErrorToSakeRequestResult(gsiCoreGetAuthErrorCode());
		uploadData->mCompletedCallback(uploadData->mSake, result, 0, SAKEFileResult_SERVER_ERROR, uploadData->mUserData);
		gsifree(uploadData);
		return freeBuffer;
	}

	if(!sakeGetFileResultFromHeaders(ghttpGetHeaders(request), &fileResult))
	{
		uploadData->mCompletedCallback(uploadData->mSake, SAKERequestResult_MALFORMED_RESPONSE, 0, SAKEFileResult_UNKNOWN_ERROR, uploadData->mUserData);
		gsifree(uploadData);
		return freeBuffer;
	}

	if(fileResult != SAKEFileResult_SUCCESS)
	{
		uploadData->mCompletedCallback(uploadData->mSake,SAKERequestResult_CONTENTSERVER_FAILURE, 0, fileResult, uploadData->mUserData);
		gsifree(uploadData);
		return freeBuffer;
	}

	if(!sakeGetFileIdFromHeaders(ghttpGetHeaders(request), &fileId))
	{
		uploadData->mCompletedCallback(uploadData->mSake, SAKERequestResult_MALFORMED_RESPONSE, 0, SAKEFileResult_SERVER_ERROR, uploadData->mUserData);
		gsifree(uploadData);
		return freeBuffer;
	}
	uploadData->mCompletedCallback(uploadData->mSake, result, fileId, fileResult, uploadData->mUserData);
	gsifree(uploadData);

	GSI_UNUSED(buffer);
	GSI_UNUSED(bufferLength);

	return freeBuffer;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SAKEStartRequestResult SAKE_CALL sakeiUploadContent( SAKE sake, 
                                                    SAKEUploadContentInput *input,
                                                    SAKEUploadContentCompletedCallback completedCallback, 
                                                    void *userData )
{

	SAKEInternalUploadData *uploadData = NULL;
	GHTTPRequest aRequest;
	GHTTPPost aPost;
    gsi_bool  result = gsi_true;

	gsi_char *actualFileName;
	gsi_char *uploadUrl = NULL;

    if (!sake||!input->remoteFileName || !completedCallback)
    {
        GS_SAKE_DEBUG_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, 
            "NULL or Invalid Instance: /n/t/tsake = 0x%x, \
            /n/t/tcompletedCallback = 0x%x, /n/t/tremoteFileName = 0x%x \n", 
            sake,completedCallback, input->remoteFileName );

        if (sake) 
        {
            sake->mStartRequestResult = SAKEStartRequestResult_BAD_INPUT;
        }
        return sake->mStartRequestResult;
    }
    else
    {
        sake->mStartRequestResult = SAKEStartRequestResult_SUCCESS;

	    uploadData = (SAKEInternalUploadData *) gsimalloc(sizeof(SAKEInternalUploadData));
	    if(uploadData == NULL)
	    {
            GS_SAKE_DEBUG_LOG(GSIDebugType_Memory, GSIDebugLevel_HotError, 
                "Could not allocate memory for uploadData \n", ""); // Extra argument required for compiling in pure C
            sake->mStartRequestResult = SAKEStartRequestResult_OUT_OF_MEMORY;
            return sake->mStartRequestResult;
	    }
        else
        {
            memset(uploadData, 0 , sizeof(SAKEInternalUploadData));

            uploadData->mProgressCallback  = input->progressCallback;
            uploadData->mCompletedCallback = completedCallback;
            uploadData->mUserData = userData;
            uploadData->mSake = sake;

            aPost = ghttpNewPost();

            if(aPost == NULL)
            {
                GS_SAKE_DEBUG_LOG(GSIDebugType_Memory, GSIDebugLevel_HotError, 
                    "Could not get a new Post, might be out of memory\n", ""); // Extra argument required for compiling in pure C
                gsifree(uploadData);
                result = gsi_false;
                sake->mStartRequestResult = SAKEStartRequestResult_OUT_OF_MEMORY;
            }
            else
            {
                if(input->content->mType == SAKEContentStorageType_DISK)
                {
                    // TODO check what happens in unexpected scenarios
                    actualFileName = _tcsrchr(input->content->mStorage.mFile, '/');
                    if (actualFileName != NULL)
                    {
                        actualFileName++;
                    }
                    else
                    {
                        actualFileName = input->remoteFileName ;
                    }

                    if(!ghttpPostAddFileFromDisk(aPost, 
                                                 actualFileName, 
                                                 input->content->mStorage.mFile, 
                                                 input->remoteFileName, 
                                                 NULL))
                    {
                        GS_SAKE_DEBUG_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, 
                            "Http Post Failed. Cannot upload %s\n", input->content->mStorage.mFile);
                        sake->mStartRequestResult = SAKEStartRequestResult_HTTP_ERROR;
                        gsifree(uploadData);
                        return sake->mStartRequestResult;
                    }
                }
                else
                {
                    // File in memory
                    if(input->content->mStorage.mMemory == NULL || input->content->mStorage.mMemory->mLength==0)
                    {
                        GS_SAKE_DEBUG_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, 
                            "Null memory passed in\n", ""); // Extra argument required for compiling in pure C
                        sake->mStartRequestResult = SAKEStartRequestResult_INVALID_DATA;
                        gsifree(uploadData);
                        return sake->mStartRequestResult;
                    }
                    else
                    {
                        if(!ghttpPostAddFileFromMemory(aPost, 
                                                    input->remoteFileName, 
                                                    input->content->mStorage.mMemory->mBuffer, 
                                                    input->content->mStorage.mMemory->mLength, 
                                                    input->remoteFileName, 
                                                    NULL))
                        {
                            GS_SAKE_DEBUG_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, 
                                "Http Post Failed\n", ""); // Extra argument required for compiling in pure C
                            sake->mStartRequestResult = SAKEStartRequestResult_HTTP_ERROR;
                            gsifree(uploadData);
                            return sake->mStartRequestResult;
                        }
                    }
                } //if SAKEContentStorageType_DISK
            
                if (result == gsi_true)
                {
					// Do the post

					char headerString[HEADERS_LENGTH];
					char sessionToken[SESSIONTOKEN_LENGTH];
					char gameIdString[GAMEID_LENGTH];
					char profileIdString[PROFILEID_LENGTH];
					gsi_char uniHeaderString[HEADERS_LENGTH]; // for when GSI_UNICODE is defined	

                    uploadUrl = (gsi_char *)gsimalloc(sizeof(gsi_char) * SAKE_MAX_URL_LENGTH);
                    if(uploadUrl == NULL || !sakeGetFileUploadURL(sake, uploadUrl))
                    {
                        // TODO test this path
                        GS_SAKE_DEBUG_LOG(GSIDebugType_Memory, GSIDebugLevel_HotError, 
                            "Could not allocate memory for uploadUrl \n", ""); // Extra argument required for compiling in pure C
                        sake->mStartRequestResult = SAKEStartRequestResult_OUT_OF_MEMORY;
                        gsifree(uploadData);
                        gsifree(uploadUrl);
                        return sake->mStartRequestResult;
                    }

					// make sure to include session token (and other stuff) in headers if we have one set 
					// (needed for apigee verification)
					strcpy(headerString, "Sake-File-Type:0");
					if (gsiCoreGetSessionToken(sessionToken))
					{
						strcat(headerString, "\r\n" SESSIONTOKEN_HEADER " ");
						strcat(headerString, sessionToken);
					}
					if (gsiCoreGetGameId(gameIdString))
					{
						strcat(headerString, "\r\n" GAMEID_HEADER " ");
						strcat(headerString, gameIdString);
					}
					if (gsiCoreGetProfileId(profileIdString))
					{
						strcat(headerString, "\r\n" PROFILEID_HEADER " ");
						strcat(headerString, profileIdString);
					}
					strcat(headerString, "\r\n"); // it seems http requires trailing carriage return before dime attachment

#ifdef GSI_UNICODE
				
					AsciiToUCS2String(headerString, uniHeaderString);
					aRequest = ghttpPostEx(uploadUrl, 
							uniHeaderString, 
							aPost, 
							GHTTPFalse, 
							(GHTTPBool)input->transferBlocking, 
							sakeiUploadContentProgressCallback, 
							sakeiUploadContentCompleteCallback, 
							uploadData);
#else
					aRequest = ghttpPostEx(uploadUrl, 
                                            headerString, 
                                            aPost, 
                                            GHTTPFalse, 
                                            (GHTTPBool)input->transferBlocking, 
                                            sakeiUploadContentProgressCallback, 
                                            sakeiUploadContentCompleteCallback, 
                                            uploadData);
#endif
                    GSI_UNUSED(uniHeaderString);
                    gsifree(uploadUrl);
                    if (aRequest < 0)
                    {
                        return sakeiTranslateGhttpResult(aRequest);
                    }
                }
            }
        }
    }
    return SAKEStartRequestResult_SUCCESS;
}
