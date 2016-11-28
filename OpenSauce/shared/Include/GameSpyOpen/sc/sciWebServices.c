///////////////////////////////////////////////////////////////////////////////
// File:	sciWebServices.c
// SDK:		GameSpy ATLAS Competition SDK
//
// Copyright (c) IGN Entertainment, Inc.  All rights reserved.  
// This software is made available only pursuant to certain license terms offered
// by IGN or its subsidiary GameSpy Industries, Inc.  Unlicensed use or use in a 
// manner not expressly authorized by IGN or GameSpy is prohibited.

#include "../common/gsCore.h"

#include "sci.h"
#include "sciInterface.h"
#include "sciWebServices.h"
#include "sciReport.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*#define SC_CHECKBANLIST_SOAPACTION  "SOAPAction: \"http://gamespy.net/competition/CheckProfileOnBanList\""
#define SC_CREATEMATCHLESSSESSION_SOAPACTION "SOAPAction: \"http://gamespy.net/competition/CreateMatchlessSession\""
#define SC_CREATESESSION_SOAPACTION  "SOAPAction: \"http://gamespy.net/competition/CreateSession\""
#define SC_SUBMITREPORT_SOAPACTION   "SOAPAction: \"http://gamespy.net/competition/SubmitReport\""
#define SC_SETINTENTION_SOAPACTION   "SOAPAction: \"http://gamespy.net/competition/SetReportIntention\""

#define SC_SERVICE_NAMESPACE_COUNT     1
const char * SC_SERVICE_NAMESPACES[SC_SERVICE_NAMESPACE_COUNT] =
{
	"gsc=\"http://gamespy.net/competition/\""
};*/

#define SC_CHECKBANLIST_SOAPACTION				"SOAPAction: \"http://gamespy.net/atlas/services/dataservice/CheckProfileOnBanList\""
#define SC_CREATEMATCHLESSSESSION_SOAPACTION	"SOAPAction: \"http://gamespy.net/atlas/services/submissionservice/CreateMatchlessSession\""
#define SC_CREATESESSION_SOAPACTION				"SOAPAction: \"http://gamespy.net/atlas/services/submissionservice/CreateSession\""
#define SC_SUBMITREPORT_SOAPACTION				"SOAPAction: \"http://gamespy.net/atlas/services/submissionservice/SubmitReport\""
#define SC_SETINTENTION_SOAPACTION				"SOAPAction: \"http://gamespy.net/atlas/services/submissionservice/SetReportIntention\""
#define SC_SERVICE_NAMESPACE_COUNT				1

const char * SC_SERVICE_NAMESPACES[SC_SERVICE_NAMESPACE_COUNT] =
{
	"gsc=\"http://gamespy.net/atlas/services/submissionservice/\""
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*#define SC_DATA_RUN_PLAYERSTATS_QUERY_SOAPACTION			"SOAPAction: \"http://gamespy.net/atlasdataservices/2010/01/StatisticsService/GetPlayerStats\""
#define SC_DATA_RUN_GAMESTATS_QUERY_SOAPACTION			"SOAPAction: \"http://gamespy.net/atlasdataservices/2010/01/StatisticsService/GetGameStats\""
#define SC_DATA_RUN_TEAMSTATS_QUERY_SOAPACTION			"SOAPAction: \"http://gamespy.net/atlasdataservices/2010/01/StatisticsService/GetTeamStats\""
#define SC_DATA_SERVICE_NAMESPACES_COUNT			1
#define SC_DATA_SERVICE_NAMESPACE					"gscd"

const char * SC_DATA_SERVICE_NAMESPACES[SC_DATA_SERVICE_NAMESPACES_COUNT] =
{
SC_DATA_SERVICE_NAMESPACE "=\"http://gamespy.net/atlasdataservices/2010/01\"",
};*/

#define SC_DATA_RUN_PLAYERSTATS_QUERY_SOAPACTION			"SOAPAction: \"http://gamespy.net/atlas/services/dataservice/2010/11/StatisticsService/GetPlayerStats\""
#define SC_DATA_RUN_GAMESTATS_QUERY_SOAPACTION			"SOAPAction: \"http://gamespy.net/atlas/services/dataservice/2010/11/StatisticsService/GetGameStats\""
#define SC_DATA_RUN_TEAMSTATS_QUERY_SOAPACTION			"SOAPAction: \"http://gamespy.net/atlas/services/dataservice/2010/11/StatisticsService/GetTeamStats\""
#define SC_DATA_SERVICE_NAMESPACES_COUNT			1
#define SC_DATA_SERVICE_NAMESPACE					"gscd"

const char * SC_DATA_SERVICE_NAMESPACES[SC_DATA_SERVICE_NAMESPACES_COUNT] =
{
	SC_DATA_SERVICE_NAMESPACE "=\"http://gamespy.net/atlas/services/dataservice/2010/11\"",
};

const char * SC_REQUEST     = "request";
const char * SC_CERTIFICATE = "authcertificate";
const char * SC_CERT_PROOF  = "authproof";
const char * SC_GAME_ID		= "gameid";
const char * SC_VERSION		= "version";

#define SC_MEMORY_ERROR	"Out of memory"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult sciWsInit(SCWebServices* webServices,
                   SCInterfacePtr theInterface)
{
	
	GS_ASSERT(webServices != NULL);
	GS_ASSERT(theInterface != NULL);
	GS_ASSERT(!webServices->mInit);

	// Check gsCore
	if (gsCoreIsShutdown())
	{
		return SCResult_CORE_NOT_INITIALIZED;
	}

	// Initialize SCWebServices struct
	memset(webServices, 0, sizeof(SCWebServices));
	webServices->mInterface                  = theInterface;

	// Now initialized
	webServices->mInit = gsi_true;

	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void sciWsDestroy(SCWebServices* webServices)
{
	GS_ASSERT(webServices != NULL);
	GS_ASSERT(webServices->mInit);

	// No longer initialized
	memset(webServices, 0, sizeof(SCWebServices));
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void sciWsThink(SCWebServices* webServices)
{
	GS_ASSERT(webServices != NULL);
	GS_ASSERT(webServices->mInit);

	gsCoreThink(0);

	GSI_UNUSED(webServices);
}

// convert the common error code enum (for response header errors) to SC error  
SCResult sciAuthErrorToSCResult(GSAuthErrorCode errorCode) 
{
	SCResult result = SCResult_NO_ERROR;

	switch(errorCode)
	{
	case GSAuthErrorCode_InvalidGameID: 
		result = SCResult_INVALID_GAMEID;
		break;
	case GSAuthErrorCode_InvalidSessionToken: 
		result = SCResult_INVALID_SESSIONTOKEN;
		break;
	case GSAuthErrorCode_SessionTokenExpired: 
		result = SCResult_SESSIONTOKEN_EXPIRED;
		break;
	default:
		result = SCResult_UNKNOWN_ERROR;
		break;
	}

	// reset AuthError for subsequent calls
	gsiCoreSetAuthError("");
	gsiCoreSetAuthErrorCode(0);

	return result;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult sciWsCheckBanList  (gsi_u32         hostProfileId,
							 SCPlatform      hostPlatform,
                             SCWebServices * webServices,
							 gsi_u32         gameId,
							 const GSLoginCertificate * certificate,
							 const GSLoginPrivateData * privateData,
							 SCCheckBanListCallback callback,
							 gsi_time        timeoutMs,
							 void *          userData)
{
	GSXmlStreamWriter request = NULL;

	// Check parameters
	GS_ASSERT(webServices != NULL);
	GS_ASSERT(webServices->mInit);

	// Check for pending request
	if (webServices->mCheckBanListCallback)
		return SCResult_CALLBACK_PENDING;

	// Create the XML message writer
	request = gsXmlCreateStreamWriter(SC_SERVICE_NAMESPACES, SC_SERVICE_NAMESPACE_COUNT);
	if (request == NULL)
		return SCResult_OUT_OF_MEMORY;

	// Fill in the request data
	if (gsi_is_false(gsXmlWriteOpenTag(request, "gsc", "CheckProfileOnBanList")) ||
		gsi_is_false(gsXmlWriteOpenTag(request, "gsc", "certificate")) ||
		gsi_is_false(wsLoginCertWriteXML(certificate, "gsc", request)) ||
		gsi_is_false(gsXmlWriteCloseTag(request, "gsc", "certificate")) ||
		gsi_is_false(gsXmlWriteHexBinaryElement(request, "gsc", "Proof", (const gsi_u8*)privateData->mKeyHash, GS_CRYPT_MD5_HASHSIZE)) ||
		gsi_is_false(gsXmlWriteIntElement(request, "gsc", "GameId", (gsi_u32)gameId)) ||
		gsi_is_false(gsXmlWriteIntElement(request, "gsc", "HostProfileId", (gsi_u32)hostProfileId)) ||
		gsi_is_false(gsXmlWriteIntElement(request, "gsc", "HostPlatformId", (gsi_u32)hostPlatform)) ||
		gsi_is_false(gsXmlWriteCloseTag(request, "gsc", "CheckProfileOnBanList")) ||
		gsi_is_false(gsXmlCloseWriter(request))
		)
	{
		gsXmlFreeWriter(request);
		return SCResult_HTTP_ERROR;
	}

	// Set callback
	webServices->mCheckBanListCallback = callback;
	webServices->mCheckBanListUserData = userData;
	webServices->mCheckBanListPending  = gsi_true;

	// Execute soap call
	gsiExecuteSoap(scGameConfigDataServiceURL, SC_CHECKBANLIST_SOAPACTION, request, sciWsCheckBanListCallback, webServices);

	GSI_UNUSED(timeoutMs);
	return SCResult_NO_ERROR;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void sciWsCheckBanListCallback(GHTTPResult httpResult,
								GSXmlStreamWriter  requestData,
								GSXmlStreamReader  responseData,
								void*        userData)
{
	int profileId = 0;
	gsi_bool profileBanned = gsi_false;
	int platformId = -1;
	SCResult aTranslatedResult     = SCResult_HTTP_ERROR;
	SCWebServices* aWebServices    = (SCWebServices*)userData;
	
	GS_ASSERT(aWebServices != NULL);
	GS_ASSERT(aWebServices->mCheckBanListPending);

	// Check for shutdown
	if (!aWebServices->mInit)
		return;

	if (httpResult == GHTTPSuccess)
	{
		if (gsiCoreGetAuthErrorCode() == GSAuthErrorCode_None)
		{
			int result = SCServiceResult_NO_ERROR;

			// Make sure a basic User Config Response is there.
			if (gsi_is_false(gsXmlMoveToStart(responseData)) 
				|| gsi_is_false(gsXmlMoveToNext(responseData, "CheckProfileOnBanListResponse")) 
				|| gsi_is_false(gsXmlMoveToNext(responseData, "CheckProfileOnBanListResult")) 
				|| gsi_is_false(gsXmlReadChildAsInt(responseData, "result", &result)) 
				)
			{
				aTranslatedResult = SCResult_RESPONSE_INVALID;
			}
			else
			{
				// Read the User Config data.
				if (result == SCServiceResult_NO_ERROR)
				{
					if(gsi_is_false(gsXmlMoveToNext(responseData, "UserConfig"))
					|| gsi_is_false(gsXmlReadAttributeAsInt(responseData, "ProfileID", &profileId))
					|| gsi_is_false(gsXmlReadAttributeAsInt(responseData, "PlatformID", &platformId))
					|| gsi_is_false(gsXmlReadAttributeAsBool(responseData, "IsBanned", &profileBanned))
					)
					{
						aTranslatedResult = SCResult_RESPONSE_INVALID;
					}

					aTranslatedResult = SCResult_NO_ERROR;
				}
				else
				{
					aTranslatedResult = SCResult_UNKNOWN_RESPONSE;
				}
			}
		}
		else
		{
			aTranslatedResult = sciAuthErrorToSCResult(gsiCoreGetAuthErrorCode());
		}
	}
	else
	{
		aTranslatedResult = SCResult_HTTP_ERROR;
	}

	// Client callback
	aWebServices->mCheckBanListPending = gsi_false;
	if (aWebServices->mCheckBanListCallback != NULL)
	{
		aWebServices->mCheckBanListCallback(aWebServices->mInterface, httpResult, aTranslatedResult, aWebServices->mCheckBanListUserData, profileId, platformId, profileBanned);
		aWebServices->mCheckBanListUserData = NULL;
		aWebServices->mCheckBanListCallback = NULL;
	}
	GSI_UNUSED(requestData);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult sciWsCreateSession (SCWebServices * webServices,
							 gsi_u32         gameId,
  						     gsi_u16         platformId,
							 const GSLoginCertificate * certificate,
							 const GSLoginPrivateData * privateData,
							 SCCreateSessionCallback callback,
							 gsi_time        timeoutMs,
							 void *          userData)
{
	GSXmlStreamWriter request = NULL;

	// Check parameters
	GS_ASSERT(webServices != NULL);
	GS_ASSERT(webServices->mInit);

	// Check for pending request
	if (webServices->mCreateSessionPending)
		return SCResult_CALLBACK_PENDING;

	// Create the XML message writer
	request = gsXmlCreateStreamWriter(SC_SERVICE_NAMESPACES, SC_SERVICE_NAMESPACE_COUNT);
	if (request == NULL)
		return SCResult_OUT_OF_MEMORY;

	// Fill in the request data
	if (gsi_is_false(gsXmlWriteOpenTag(request, "gsc", "CreateSession")) ||
		gsi_is_false(gsXmlWriteOpenTag(request, "gsc", "certificate")) ||
		gsi_is_false(wsLoginCertWriteXML(certificate, "gsc", request)) ||
		gsi_is_false(gsXmlWriteCloseTag(request, "gsc", "certificate")) ||
		gsi_is_false(gsXmlWriteHexBinaryElement(request, "gsc", "proof", (const gsi_u8*)privateData->mKeyHash, GS_CRYPT_MD5_HASHSIZE)) ||
		gsi_is_false(gsXmlWriteIntElement(request, "gsc", "gameid", (gsi_u32)gameId)) ||
		gsi_is_false(gsXmlWriteIntElement(request, "gsc", "platformid", (gsi_u16)platformId)) ||
		gsi_is_false(gsXmlWriteCloseTag(request, "gsc", "CreateSession")) ||
		gsi_is_false(gsXmlCloseWriter(request))
		)
	{
		gsXmlFreeWriter(request);
		return SCResult_HTTP_ERROR;
	}

	// Set callback
	webServices->mCreateSessionCallback = callback;
	webServices->mCreateSessionUserData = userData;
	webServices->mCreateSessionPending  = gsi_true;

	// Execute soap call
	gsiExecuteSoap(scServiceURL, SC_CREATESESSION_SOAPACTION, 
		request, sciWsCreateSessionCallback, webServices);
	GSI_UNUSED(timeoutMs);
	return SCResult_NO_ERROR;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult sciWsCreateMatchlessSession (SCWebServices * webServices,
							          gsi_u32         gameId,
  								      gsi_u16         platformId,
							          const GSLoginCertificate * certificate,
							          const GSLoginPrivateData * privateData,
							          SCCreateSessionCallback callback,
							          gsi_time        timeoutMs,
							          void *          userData)
{
	GSXmlStreamWriter request = NULL;

	// Check parameters
	GS_ASSERT(webServices != NULL);
	GS_ASSERT(webServices->mInit);

	// Check for pending request
	if (webServices->mCreateSessionPending)
		return SCResult_CALLBACK_PENDING;

	// Create the XML message writer
	request = gsXmlCreateStreamWriter(SC_SERVICE_NAMESPACES, SC_SERVICE_NAMESPACE_COUNT);
	if (request == NULL)
		return SCResult_OUT_OF_MEMORY;

	// Fill in the request data
	if (gsi_is_false(gsXmlWriteOpenTag(request, "gsc", "CreateMatchlessSession")) ||
		gsi_is_false(gsXmlWriteOpenTag(request, "gsc", "certificate")) ||
		gsi_is_false(wsLoginCertWriteXML(certificate, "gsc", request)) ||
		gsi_is_false(gsXmlWriteCloseTag(request, "gsc", "certificate")) ||
		gsi_is_false(gsXmlWriteHexBinaryElement(request, "gsc", "proof", (const gsi_u8*)privateData->mKeyHash, GS_CRYPT_MD5_HASHSIZE)) ||
		gsi_is_false(gsXmlWriteIntElement(request, "gsc", "gameid", (gsi_u32)gameId)) ||
		gsi_is_false(gsXmlWriteIntElement(request, "gsc", "platformid", (gsi_u16)platformId)) ||
		gsi_is_false(gsXmlWriteCloseTag(request, "gsc", "CreateMatchlessSession")) ||
		gsi_is_false(gsXmlCloseWriter(request))
		)
	{
		gsXmlFreeWriter(request);
		return SCResult_HTTP_ERROR;
	}

	// Set callback
	webServices->mCreateSessionCallback = callback;
	webServices->mCreateSessionUserData = userData;
	webServices->mCreateSessionPending  = gsi_true;

	// Execute soap call
	gsiExecuteSoap(scServiceURL, SC_CREATEMATCHLESSSESSION_SOAPACTION, 
		request, sciWsCreateSessionCallback, webServices);
	GSI_UNUSED(timeoutMs);
	return SCResult_NO_ERROR;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void sciWsCreateSessionCallback(GHTTPResult httpResult,
								GSXmlStreamWriter  requestData,
								GSXmlStreamReader  responseData,
								void*        userData)
{
	SCResult aTranslatedResult     = SCResult_HTTP_ERROR;
	SCWebServices* aWebServices    = (SCWebServices*)userData;
	
	char csid[255];
	char ccid[255];
	int csidLen = 255;
	int ccidLen = 255;

	GS_ASSERT(aWebServices != NULL);
	GS_ASSERT(aWebServices->mCreateSessionPending);

	// Check for shutdown
	if (!aWebServices->mInit)
		return;

	if (httpResult == GHTTPSuccess)
	{
		if (gsiCoreGetAuthErrorCode() == GSAuthErrorCode_None)
		{
			int createResult = 0;

			// Parse through in a way that will work for either type of CreateSession response.
			if (gsi_is_false(gsXmlMoveToStart(responseData)))
			{
				aTranslatedResult = SCResult_RESPONSE_INVALID;
			}
			else if(gsi_is_false(gsXmlMoveToNext(responseData, "CreateSessionResponse")))
			{
				if(gsi_is_false(gsXmlMoveToNext(responseData, "CreateMatchlessSessionResponse")))
				{
					aTranslatedResult = SCResult_RESPONSE_INVALID;
				}
			}
			
			if(gsi_is_false(gsXmlMoveToNext(responseData, "CreateSessionResult")))
			{
				if(gsi_is_false(gsXmlMoveToNext(responseData, "CreateMatchlessSessionResult")))
				{
					aTranslatedResult = SCResult_RESPONSE_INVALID;
				}
			}

			if(gsi_is_false(gsXmlReadChildAsInt(responseData, "result", &createResult)))
			{
				aTranslatedResult = SCResult_RESPONSE_INVALID;
			}
			else if(aTranslatedResult != SCResult_RESPONSE_INVALID)
			{
				// Parse server reported result
				if (createResult == SCServiceResult_NO_ERROR)
				{
					// Read session and connection ID
					if(gsi_is_false(gsXmlReadChildAsStringNT(responseData, "csid", csid, csidLen)) ||
					   gsi_is_false(gsXmlReadChildAsStringNT(responseData, "ccid", ccid, ccidLen))
					   )
					{
						aTranslatedResult = SCResult_RESPONSE_INVALID;
					}
					else
					{
						sciInterfaceSetSessionId((SCInterface*)aWebServices->mInterface, csid);
						sciInterfaceSetConnectionId((SCInterface*)aWebServices->mInterface, ccid);
						aTranslatedResult = SCResult_NO_ERROR;
					}
				}
				else
				{
					aTranslatedResult = SCResult_RESPONSE_INVALID;
				}
			}
		}
		else
		{
			aTranslatedResult = sciAuthErrorToSCResult(gsiCoreGetAuthErrorCode());
		}
	}
	else
	{
		aTranslatedResult = SCResult_HTTP_ERROR;
	}

	// Client callback
	aWebServices->mCreateSessionPending = gsi_false;
	if (aWebServices->mCreateSessionCallback != NULL)
	{
		aWebServices->mCreateSessionCallback(aWebServices->mInterface, httpResult, aTranslatedResult, aWebServices->mCreateSessionUserData);
		aWebServices->mCreateSessionUserData = NULL;
		aWebServices->mCreateSessionCallback = NULL;
	}
	GSI_UNUSED(requestData);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult sciWsSetReportIntention(SCWebServices* webServices,
								 gsi_u32        gameId,
								 const char *   theSessionId,
								 const char *   theConnectionId,
								 gsi_bool       isAuthoritative,
								 const GSLoginCertificate * certificate,
								 const GSLoginPrivateData * privateData,
                                 SCSetReportIntentionCallback callback,
                                 gsi_time       timeoutMs,
								 void *         userData)
{
	GSXmlStreamWriter request = NULL;

	// Check parameters
	GS_ASSERT(webServices != NULL);
	GS_ASSERT(webServices->mInit);

	// Check for pending request
	if (webServices->mSetReportIntentionPending)
		return SCResult_CALLBACK_PENDING;

	// Create the XML message writer
	request = gsXmlCreateStreamWriter(SC_SERVICE_NAMESPACES, SC_SERVICE_NAMESPACE_COUNT);
	if (request == NULL)
		return SCResult_OUT_OF_MEMORY;

	// Fill in the request data
	if (gsi_is_false(gsXmlWriteOpenTag(request, "gsc", "SetReportIntention")) ||
		gsi_is_false(gsXmlWriteOpenTag(request, "gsc", "certificate")) ||
		gsi_is_false(wsLoginCertWriteXML(certificate, "gsc", request)) ||
		gsi_is_false(gsXmlWriteCloseTag(request, "gsc", "certificate")) ||
		gsi_is_false(gsXmlWriteHexBinaryElement(request, "gsc", "proof", (const gsi_u8*)privateData->mKeyHash, GS_CRYPT_MD5_HASHSIZE)) ||
		gsi_is_false(gsXmlWriteStringElement(request, "gsc", "csid", theSessionId)) ||
		gsi_is_false(gsXmlWriteStringElement(request, "gsc", "ccid", theConnectionId)) ||
		gsi_is_false(gsXmlWriteIntElement(request, "gsc", "gameid", (gsi_u32)gameId)) ||
		gsi_is_false(gsXmlWriteIntElement(request, "gsc", "authoritative", (gsi_u32)(gsi_is_true(isAuthoritative) ? 1:0))) ||
		gsi_is_false(gsXmlWriteCloseTag(request, "gsc", "SetReportIntention")) ||
		gsi_is_false(gsXmlCloseWriter(request))
		)
	{
		gsXmlFreeWriter(request);
		return SCResult_HTTP_ERROR;
	}

	// Set callback
	webServices->mSetReportIntentionCallback = callback;
	webServices->mSetReportIntentionUserData = userData;
	webServices->mSetReportIntentionPending = gsi_true;

	// Execute soap call
	gsiExecuteSoap(scServiceURL, SC_SETINTENTION_SOAPACTION, 
		request, sciWsSetReportIntentionCallback, webServices);
	
	GSI_UNUSED(timeoutMs);
	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void sciWsSetReportIntentionCallback(GHTTPResult httpResult,
                                     GSXmlStreamWriter requestData,
                                     GSXmlStreamReader responseData,
                                     void*       userData)
{
	SCResult aTranslatedResult     = SCResult_HTTP_ERROR;
	SCWebServices* aWebServices    = (SCWebServices*)userData;

	char ccid[255];
	int ccidLen = 255;


	GS_ASSERT(aWebServices != NULL);
	GS_ASSERT(aWebServices->mSetReportIntentionPending);

	// Check for shutdown
	if (!aWebServices->mInit)
		return;

	if (httpResult == GHTTPSuccess)
	{
		if (gsiCoreGetAuthErrorCode() == GSAuthErrorCode_None)
		{
			int intentionResult = 0;

			if (gsi_is_false(gsXmlMoveToStart(responseData)) ||
				gsi_is_false(gsXmlMoveToNext(responseData, "SetReportIntentionResponse")) ||
				gsi_is_false(gsXmlMoveToNext(responseData, "SetReportIntentionResult")) ||
				gsi_is_false(gsXmlReadChildAsInt(responseData, "result", &intentionResult)) ||
  				gsi_is_false(gsXmlReadChildAsStringNT(responseData, "ccid", ccid, ccidLen))
				)
			{
				aTranslatedResult = SCResult_RESPONSE_INVALID;
			}
			else
			{
				if (intentionResult == SCServiceResult_NO_ERROR)
				{
					aTranslatedResult = SCResult_NO_ERROR;
					sciInterfaceSetConnectionId((SCInterface*)aWebServices->mInterface, ccid);
				}
				else
					aTranslatedResult = SCResult_UNKNOWN_RESPONSE;
			}
		}
		else
		{
			aTranslatedResult = sciAuthErrorToSCResult(gsiCoreGetAuthErrorCode());
		}
	}
	else
	{
		aTranslatedResult = SCResult_HTTP_ERROR;
	}

	// Client callback
	aWebServices->mSetReportIntentionPending = gsi_false;
	if (aWebServices->mSetReportIntentionCallback != NULL)
	{
		aWebServices->mSetReportIntentionCallback(aWebServices->mInterface,
			                                      httpResult,
		                                          aTranslatedResult,
												  aWebServices->mSetReportIntentionUserData);
		aWebServices->mSetReportIntentionUserData = NULL;
		aWebServices->mSetReportIntentionCallback = NULL;
	}
	GSI_UNUSED(requestData);
}


///////////////////////////////////////////////////////////////////////////////
// declared here to allow function to get around Unicode calls
extern GHTTPBool ghiPostAddFileFromMemory(GHTTPPost post,const char * name,const char * buffer,
	int bufferLen,const char * reportFilename,const char * contentType);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Private GSSoapCustomFunc used by sciWsSubmitReport
static void sciWsSubmitReportCustom(GHTTPPost thePost, void* userData)
{
	SCWebServices* aWebServices = (SCWebServices*)userData;

	//Use internal method to get around unicode calls
	ghiPostAddFileFromMemory(thePost, "report", (char *)aWebServices->mSubmitReportData,
		(gsi_i32)aWebServices->mSubmitReportLength, "report", "application/bin");
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult sciWsSubmitReport(SCWebServices* webServices,
						   gsi_u32        gameId,
						   const char *   theSessionId,
						   const char *   theConnectionId,
						   const SCIReport *    theReport,
						   gsi_bool       isAuthoritative,
						   const GSLoginCertificate * certificate,
						   const GSLoginPrivateData * privateData,
                           SCSubmitReportCallback callback,
                           gsi_time       timeoutMs,
						   void *         userData)
{
	GSXmlStreamWriter request = NULL;

	// Check parameters
	GS_ASSERT(webServices != NULL);

	// Check for pending request
	if (webServices->mSubmitReportPending)
	{
		return SCResult_CALLBACK_PENDING;
	}

	// Check for complete report
	if (theReport->mBuffer.mPos < sizeof(SCIReportHeader))
		return SCResult_REPORT_INVALID;

	// Create the XML message writer
	request = gsXmlCreateStreamWriter(SC_SERVICE_NAMESPACES, SC_SERVICE_NAMESPACE_COUNT);
	if (request == NULL)
		return SCResult_OUT_OF_MEMORY;

	// Fill in the request data
	if (gsi_is_false(gsXmlWriteOpenTag(request, "gsc", "SubmitReport")) ||
		gsi_is_false(gsXmlWriteOpenTag(request, "gsc", "certificate")) ||
		gsi_is_false(wsLoginCertWriteXML(certificate, "gsc", request)) ||
		gsi_is_false(gsXmlWriteCloseTag(request, "gsc", "certificate")) ||
		gsi_is_false(gsXmlWriteHexBinaryElement(request, "gsc", "proof", (const gsi_u8*)privateData->mKeyHash, GS_CRYPT_MD5_HASHSIZE)) ||
		gsi_is_false(gsXmlWriteStringElement(request, "gsc", "csid", theSessionId)) ||
		gsi_is_false(gsXmlWriteStringElement(request, "gsc", "ccid", theConnectionId)) ||
		gsi_is_false(gsXmlWriteIntElement(request, "gsc", "gameid", (gsi_u32)gameId)) ||
		gsi_is_false(gsXmlWriteIntElement(request, "gsc", "authoritative", (gsi_u32)(gsi_is_true(isAuthoritative) ? 1:0))) ||
		gsi_is_false(gsXmlWriteCloseTag(request, "gsc", "SubmitReport")) ||
		gsi_is_false(gsXmlCloseWriter(request))
		)
	{
		gsXmlFreeWriter(request);
		return SCResult_OUT_OF_MEMORY;
	}
	
	// Get submission size
	webServices->mSubmitReportData  = (gsi_u8*)theReport->mBuffer.mData;
	webServices->mSubmitReportLength = theReport->mBuffer.mPos;

	// Set callback
	webServices->mSubmitReportDataCallback = callback;
	webServices->mSubmitReportUserData     = userData;
	webServices->mSubmitReportPending      = gsi_true;

	// Execute soap call
	gsiExecuteSoapCustom(scServiceURL, SC_SUBMITREPORT_SOAPACTION, 
		request, sciWsSubmitReportCallback,sciWsSubmitReportCustom, webServices);
	
	GSI_UNUSED(timeoutMs);
	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void sciWsSubmitReportCallback(GHTTPResult       httpResult,
                               GSXmlStreamWriter requestData,
                               GSXmlStreamReader responseData,
                               void*             userData)
{
	SCResult aTranslatedResult     = SCResult_HTTP_ERROR;
	SCWebServices* aWebServices    = (SCWebServices*)userData;

	GS_ASSERT(aWebServices != NULL);

	// Check for shutdown
	if (!aWebServices->mInit)
		return;

	GS_ASSERT(aWebServices->mSubmitReportPending);

	if (httpResult == GHTTPSuccess)
	{
		if (gsiCoreGetAuthErrorCode() == GSAuthErrorCode_None)
		{
			int submitResult = 0;

			if (gsi_is_false(gsXmlMoveToStart(responseData)) ||
				gsi_is_false(gsXmlMoveToNext(responseData, "SubmitReportResponse")) ||
				gsi_is_false(gsXmlMoveToNext(responseData, "SubmitReportResult")) ||
				gsi_is_false(gsXmlReadChildAsInt(responseData, "result", &submitResult))
				)
			{
				aTranslatedResult = SCResult_RESPONSE_INVALID;
			}
			else
			{
				switch (submitResult)
				{
				case SCServiceResult_NO_ERROR:       
					aTranslatedResult = SCResult_NO_ERROR;          
					break;
				case SCServiceResult_REPORT_INVALID: 
					aTranslatedResult = SCResult_REPORT_INVALID;    
					break;
				default:
					aTranslatedResult = SCResult_UNKNOWN_RESPONSE; 
					break;
				};
			}
		}
		else
		{
			aTranslatedResult = sciAuthErrorToSCResult(gsiCoreGetAuthErrorCode());
		}
	}
	else
	{
		aTranslatedResult = SCResult_HTTP_ERROR;
	}

	// Client callback
	aWebServices->mSubmitReportPending = gsi_false;
	if (aWebServices->mSubmitReportDataCallback != NULL)
	{
		aWebServices->mSubmitReportDataCallback(aWebServices->mInterface,
			                                httpResult,
		                                    aTranslatedResult,
											aWebServices->mSubmitReportUserData);
		aWebServices->mSubmitReportUserData = NULL;
		aWebServices->mSubmitReportDataCallback = NULL;
	}
	GSI_UNUSED(requestData);
}

SCResult sciWsParseHeader(GSXmlStreamReader responseData, gsi_char **message, int *messageLen)
{
	const char *tempStr;
//#ifdef GSI_UNICODE
//	gsi_u16 tempStrLen;
//#else
	int tempStrLen;
//#endif

	SCResult result = SCResult_RESPONSE_INVALID;
	int resultNumber = -1;
	if (gsi_is_false(gsXmlMoveToStart(responseData)) ||
		gsi_is_false(gsXmlMoveToNext(responseData, "status")) ||
		gsi_is_false(gsXmlReadChildAsInt(responseData, "number", &resultNumber)) ||
		gsi_is_false(gsXmlReadChildAsString(responseData, "message", &tempStr, &tempStrLen)))
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: status, number, message");
		return SCResult_RESPONSE_INVALID;
	}

	*messageLen = tempStrLen;
	// Allocate a string based on the user's preferences for unicode/ascii
	*message = gsimalloc(tempStrLen + 1);
	if (*message == NULL)
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, SC_MEMORY_ERROR);
		return SCResult_OUT_OF_MEMORY;
	}
#ifndef GSI_UNICODE
	gsiSafeStrcpyA(*message, tempStr, tempStrLen + 1);	
#endif

	// convert backend error numbers to actual SDK codes
	switch(resultNumber)
	{
	case 0:
		{
			result = SCResult_NO_ERROR; 
			break;
		}			
	case 1: 
		{
			result = SCResult_UNKNOWN_RESPONSE; 
			break;
		}			
	case 2: 
		{
			result = SCResult_INVALID_PARAMETERS; 
			break;
		}
	case 90:
		{
			result = SCResult_UNKNOWN_ERROR;
			break;
		}
	case 100:
		{
			result = SCResult_QUERY_DISABLED; 
			break;
		}
	case 101:
		{
			result = SCResult_QUERY_TYPE_MISMATCH;
			break;
		}
	case 102:
	case 103:
		{
			result = SCResult_QUERY_INVALID;
			break;
		}
	case 104:
		{

			result = SCResult_QUERY_PARAMS_MISSING;
			break;
		}
	case 105:
		{
			result = SCResult_QUERY_PARAMS_TOO_MANY;			
			break;
		}
	case 106:
		{
			result = SCResult_QUERY_PARAM_TYPE_INVALID;
			break;
		}
	default: 
		{
			result = SCResult_UNKNOWN_RESPONSE; 
			break;
		}
	}
	return result;
}


SCResult sciWsParseStat(GSXmlStreamReader  responseXml,
						SCStat *stat)
{
	const char *name, *type, *val;
//#ifdef GSI_UNICODE
//	gsi_u16 nameLen, typeLen, valLen;
//#else
	int nameLen, typeLen, valLen;
//#endif	

	if (gsi_is_false(gsXmlReadAttributeAsString(responseXml, "n", &name, &nameLen)) ||
		gsi_is_false(gsXmlReadAttributeAsString(responseXml, "t", &type, &typeLen)) ||
		gsi_is_false(gsXmlReadAttributeAsString(responseXml, "v", &val, &valLen)))
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: n, t, v");
		return SCResult_RESPONSE_INVALID;
	}

	// Allocate a string based on the user's preferences for unicode/ascii
	stat->mName = gsimalloc(nameLen + 1);
	stat->mValue = gsimalloc(valLen + 1);	
	if (stat->mName == NULL || stat->mValue == NULL)
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, SC_MEMORY_ERROR);
		return SCResult_OUT_OF_MEMORY;
	}
#ifndef GSI_UNICODE
	gsiSafeStrcpyA(stat->mName, name, nameLen+ 1);
	gsiSafeStrcpyA(stat->mValue, val, valLen + 1);
#endif

	// We convert the type string to an enum since it takes up less space.
	// It also makes checking the type easier for users
	if (strncmp(type, "Int32", typeLen) == 0)
	{	
		stat->mStatType = SCDataType_INT32;
		return SCResult_NO_ERROR;
	}
	if (strncmp(type, "String", typeLen) == 0)
	{
		stat->mStatType = SCDataType_STRING;
		return SCResult_NO_ERROR;
	}
	if (strncmp(type, "Int64", typeLen) == 0)
	{	
		stat->mStatType = SCDataType_INT64;
		return SCResult_NO_ERROR;
	}
	if (strncmp(type, "Int16", typeLen) == 0)
	{
		stat->mStatType = SCDataType_INT16;
		return SCResult_NO_ERROR;
	}
	if (strncmp(type, "Float", typeLen) == 0)
	{
		stat->mStatType = SCDataType_FLOAT;
		return SCResult_NO_ERROR;
	}
	if (strncmp(type, "Byte", typeLen) == 0)
	{	
		stat->mStatType = SCDataType_BYTE;
		return SCResult_NO_ERROR;
	}

	return SCResult_NO_ERROR;
}


SCResult sciWsParsePlayer(GSXmlStreamReader  responseXml,
						  SCPlayer *player)
{
	gsi_u32 i;
	SCResult result;
	GS_ASSERT(player);

	if (gsi_is_false(gsXmlReadChildAsInt(responseXml, "pid", (int *)&player->mProfileId)) ||
		gsi_is_false(gsXmlMoveToChild(responseXml, "stats")))
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: pid");
		return SCResult_RESPONSE_INVALID;
	}

	player->mStatsCount = gsXmlCountChildren(responseXml, "s");
	if (player->mStatsCount == 0)
	{
		player->mStats = NULL;
		return SCResult_NO_ERROR;
	}

	player->mStats = (SCStat *)gsimalloc(sizeof(SCStat) * player->mStatsCount);
	if (player->mStats == NULL)
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, SC_MEMORY_ERROR);
		return SCResult_OUT_OF_MEMORY;
	}
	memset(player->mStats, 0, sizeof(SCStat) * player->mStatsCount);
	for (i = 0; i < player->mStatsCount; i++)
	{
		if (i == 0)
		{
			if (gsi_is_false(gsXmlMoveToChild(responseXml, "s")))
			{
				GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: s");
				return SCResult_RESPONSE_INVALID;
			}
		}
		else
		{
			if (gsi_is_false(gsXmlMoveToSibling(responseXml, "s")))
			{
				GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: s");
				return SCResult_RESPONSE_INVALID;
			}
		}
		result = sciWsParseStat(responseXml, &player->mStats[i]);
		if (result != SCResult_NO_ERROR)
		{
			GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse player->mStats[%d]", i);
			return result;
		}
	}
	if (gsi_is_false(gsXmlMoveToParent(responseXml)) ||
		gsi_is_false(gsXmlMoveToParent(responseXml)))
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML");
		return SCResult_RESPONSE_INVALID;
	}
	return SCResult_NO_ERROR;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult sciWsParseTeam(GSXmlStreamReader  responseXml,
						SCTeam *team)
{
	gsi_u32 i;
	SCResult result;
	GS_ASSERT(team);

	if (gsi_is_false(gsXmlReadChildAsInt(responseXml, "tid", (int *)&team->mTeamId)) ||
		gsi_is_false(gsXmlMoveToChild(responseXml, "stats")))
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: pid");
		return SCResult_RESPONSE_INVALID;
	}

	team->mStatsCount = gsXmlCountChildren(responseXml, "s");
	if (team->mStatsCount == 0)
	{
		team->mStats = NULL;
		return SCResult_NO_ERROR;
	}

	team->mStats = (SCStat *)gsimalloc(sizeof(SCStat) * team->mStatsCount);
	if (team->mStats == NULL)
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, SC_MEMORY_ERROR);
		return SCResult_OUT_OF_MEMORY;
	}
	memset(team->mStats, 0, sizeof(SCStat) * team->mStatsCount);
	for (i = 0; i < team->mStatsCount; i++)
	{
		if (i == 0)
		{
			if (gsi_is_false(gsXmlMoveToChild(responseXml, "s")))
			{
				GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: s");
				return SCResult_RESPONSE_INVALID;
			}
		}
		else
		{
			if (gsi_is_false(gsXmlMoveToSibling(responseXml, "s")))
			{
				GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: s");
				return SCResult_RESPONSE_INVALID;
			}
		}
		result = sciWsParseStat(responseXml, &team->mStats[i]);
		if (result != SCResult_NO_ERROR)
		{
			GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse team->mStats[%d]", i);
			return result;
		}
	}
	if (gsi_is_false(gsXmlMoveToParent(responseXml)) ||
		gsi_is_false(gsXmlMoveToParent(responseXml)))
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML");
		return SCResult_RESPONSE_INVALID;
	}
	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult sciWsParsePlayerStatsCategory(GSXmlStreamReader  responseXml,
									   SCPlayerStatsCategory *category)
{
	gsi_u32 i;
	SCResult result;
	const char *tempStr;
//#ifdef GSI_UNICODE
//	gsi_u16 tempStrLen;
//#else
	int tempStrLen;
//#endif	
	GS_ASSERT(category);

	if (gsi_is_false(gsXmlReadChildAsString(responseXml, "name", &tempStr, &tempStrLen)) ||
		gsi_is_false(gsXmlMoveToChild(responseXml, "players")))
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: name");
		return SCResult_RESPONSE_INVALID;
	}

	// Allocate a string based on the user's preferences for unicode/ascii
	category->mName = gsimalloc(tempStrLen + 1);	
	if (category->mName == NULL)
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, SC_MEMORY_ERROR);
		return SCResult_OUT_OF_MEMORY;
	}
#ifndef GSI_UNICODE
	gsiSafeStrcpyA(category->mName, tempStr, tempStrLen + 1);	
#endif

	category->mPlayersCount = gsXmlCountChildren(responseXml, "p");
	if (category->mPlayersCount == 0)
	{
		category->mPlayers = NULL;
		return SCResult_NO_ERROR;
	}

	category->mPlayers = (SCPlayer *)gsimalloc(sizeof(SCPlayer) * category->mPlayersCount);
	if (category->mPlayers == NULL)
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, SC_MEMORY_ERROR);
		return SCResult_OUT_OF_MEMORY;
	}
	memset(category->mPlayers, 0, sizeof(SCPlayer) * category->mPlayersCount);

	for (i = 0; i < category->mPlayersCount; i++)
	{
		if (i == 0)
		{
			if (gsi_is_false(gsXmlMoveToChild(responseXml, "p")))
			{
				GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: p");
				return SCResult_RESPONSE_INVALID;
			}
		}
		else
		{
			if (gsi_is_false(gsXmlMoveToSibling(responseXml, "p")))
			{
				GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: p");
				return SCResult_RESPONSE_INVALID;
			}
		}
		result = sciWsParsePlayer(responseXml, &category->mPlayers[i]);
		if (result != SCResult_NO_ERROR)
		{
			GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse category->mPlayers[%d]", i);
			return result;
		}
	}
	if (gsi_is_false(gsXmlMoveToParent(responseXml)))
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML");
		return SCResult_RESPONSE_INVALID;
	}
	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult sciWsParsePlayerStatsQueryResponse(GSXmlStreamReader  responseXml,
											SCPlayerStatsQueryResponse **response)
{
	// We are parsing a single SCPlayerStatsQueryResponse, setting *response to the 
	// parsedResponse if everything parses correctly
	gsi_u32 i;
	SCResult result;
	SCPlayerStatsQueryResponse *parsedResponse = NULL;
	if (gsi_is_false(gsXmlMoveToSibling(responseXml, "categories")))
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: categories");
		return SCResult_RESPONSE_INVALID;
	}

	parsedResponse = (SCPlayerStatsQueryResponse *)gsimalloc(sizeof(SCPlayerStatsQueryResponse));
	if (parsedResponse == NULL)
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, SC_MEMORY_ERROR);
		return SCResult_OUT_OF_MEMORY;
	}
	parsedResponse->mCategoriesCount = gsXmlCountChildren(responseXml, "c");
	if (parsedResponse->mCategoriesCount == 0)
	{
		parsedResponse->mCategories = NULL;
		return SCResult_NO_ERROR;
	}

	parsedResponse->mCategories = (SCPlayerStatsCategory *)gsimalloc(sizeof(SCPlayerStatsCategory) * parsedResponse->mCategoriesCount);
	if (parsedResponse->mCategories == NULL)
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, SC_MEMORY_ERROR);
		return SCResult_OUT_OF_MEMORY;
	}
	memset(parsedResponse->mCategories, 0, sizeof(SCPlayerStatsCategory) * parsedResponse->mCategoriesCount);

	for (i = 0; i < parsedResponse->mCategoriesCount; i++)
	{
		if (i == 0)
		{	
			// We only move to child on the first iteration
			if (gsi_is_false(gsXmlMoveToChild(responseXml, "c")))
			{
				GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: c");
				return SCResult_RESPONSE_INVALID;
			}			
		}
		else 
		{
			// the cursor now is able to move to sibling nodes
			if (gsi_is_false(gsXmlMoveToSibling(responseXml, "c")))
			{
				GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: c");
				return SCResult_RESPONSE_INVALID;
			}
		}
		result = sciWsParsePlayerStatsCategory(responseXml, &parsedResponse->mCategories[i]);
		if (result != SCResult_NO_ERROR)
		{
			GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse parsedResponse->mCategories[%d]", i);
			return result;
		}
	}

	*response = parsedResponse;
	return SCResult_NO_ERROR;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult sciWsParseGameStatsCategory(GSXmlStreamReader  responseXml,
									 SCGameStatsCategory *category)
{
	gsi_u32 i;
	SCResult result;
	const char *tempStr;
//#ifdef GSI_UNICODE
//	gsi_u16 tempStrLen;
//#else
	int tempStrLen;
//#endif	
	GS_ASSERT(category);

	if (gsi_is_false(gsXmlReadChildAsString(responseXml, "name", &tempStr, &tempStrLen)) ||
		gsi_is_false(gsXmlMoveToChild(responseXml, "stats")))
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: name");
		return SCResult_RESPONSE_INVALID;
	}

	// Allocate a string based on the user's preferences for unicode/ascii
	category->mName = gsimalloc(tempStrLen + 1);	
	if (category->mName == NULL)
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, SC_MEMORY_ERROR);
		return SCResult_OUT_OF_MEMORY;
	}
#ifndef GSI_UNICODE
	gsiSafeStrcpyA(category->mName, tempStr, tempStrLen + 1);	
#endif

	category->mStatsCount = gsXmlCountChildren(responseXml, "s");
	if (category->mStatsCount == 0)
	{
		category->mStats = NULL;
		return SCResult_NO_ERROR;
	}

	category->mStats = (SCStat *)gsimalloc(sizeof(SCStat) * category->mStatsCount);
	if (category->mStats == NULL)
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, SC_MEMORY_ERROR);
		return SCResult_OUT_OF_MEMORY;
	}
	memset(category->mStats, 0, sizeof(SCPlayer) * category->mStatsCount);

	for (i = 0; i < category->mStatsCount; i++)
	{
		if (i == 0)
		{
			if (gsi_is_false(gsXmlMoveToChild(responseXml, "s")))
			{
				GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: p");
				return SCResult_RESPONSE_INVALID;
			}
		}
		else
		{
			if (gsi_is_false(gsXmlMoveToSibling(responseXml, "s")))
			{
				GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: p");
				return SCResult_RESPONSE_INVALID;
			}
		}
		result = sciWsParseStat(responseXml, &category->mStats[i]);
		if (result != SCResult_NO_ERROR)
		{
			GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse category->mStats[%d]", i);
			return result;
		}
	}
	if (gsi_is_false(gsXmlMoveToParent(responseXml)))
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML");
		return SCResult_RESPONSE_INVALID;
	}
	return SCResult_NO_ERROR;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult sciWsParseGameStatsQueryResponse(GSXmlStreamReader  responseXml,
										  SCGameStatsQueryResponse **response)
{
	// We are parsing a single SCPlayerStatsQueryResponse, setting *response to the 
	// parsedResponse if everything parses correctly
	gsi_u32 i;
	SCResult result;
	SCGameStatsQueryResponse *parsedResponse = NULL;
	if (gsi_is_false(gsXmlMoveToSibling(responseXml, "categories")))
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: categories");
		return SCResult_RESPONSE_INVALID;
	}

	parsedResponse = (SCGameStatsQueryResponse *)gsimalloc(sizeof(SCGameStatsQueryResponse));
	if (parsedResponse == NULL)
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, SC_MEMORY_ERROR);
		return SCResult_OUT_OF_MEMORY;
	}
	parsedResponse->mCategoriesCount = gsXmlCountChildren(responseXml, "c");
	if (parsedResponse->mCategoriesCount == 0)
	{
		parsedResponse->mCategories = NULL;
		return SCResult_NO_ERROR;
	}

	parsedResponse->mCategories = (SCGameStatsCategory *)gsimalloc(sizeof(SCGameStatsCategory) * parsedResponse->mCategoriesCount);
	if (parsedResponse->mCategories == NULL)
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, SC_MEMORY_ERROR);
		return SCResult_OUT_OF_MEMORY;
	}
	memset(parsedResponse->mCategories, 0, sizeof(SCGameStatsCategory) * parsedResponse->mCategoriesCount);

	for (i = 0; i < parsedResponse->mCategoriesCount; i++)
	{
		if (i == 0)
		{	
			// We only move to child on the first iteration
			if (gsi_is_false(gsXmlMoveToChild(responseXml, "c")))
			{
				GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: c");
				return SCResult_RESPONSE_INVALID;
			}			
		}
		else 
		{
			// the cursor now is able to move to sibling nodes
			if (gsi_is_false(gsXmlMoveToSibling(responseXml, "c")))
			{
				GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: c");
				return SCResult_RESPONSE_INVALID;
			}
		}
		result = sciWsParseGameStatsCategory(responseXml, &parsedResponse->mCategories[i]);
		if (result != SCResult_NO_ERROR)
		{
			GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse parsedResponse->mCategories[%d]", i);
			return result;
		}
	}

	*response = parsedResponse;
	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult sciWsParseTeamStatsCategory(GSXmlStreamReader  responseXml,
									 SCTeamStatsCategory *category)
{
	gsi_u32 i;
	SCResult result;
	const char *tempStr;
//#ifdef GSI_UNICODE
//	gsi_u16 tempStrLen;
//#else
	int tempStrLen;
//#endif	
	GS_ASSERT(category);

	if (gsi_is_false(gsXmlReadChildAsString(responseXml, "name", &tempStr, &tempStrLen)) ||
		gsi_is_false(gsXmlMoveToChild(responseXml, "teams")))
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: name");
		return SCResult_RESPONSE_INVALID;
	}

	// Allocate a string based on the user's preferences for unicode/ascii
	category->mName = gsimalloc(tempStrLen + 1);	
	if (category->mName == NULL)
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, SC_MEMORY_ERROR);
		return SCResult_OUT_OF_MEMORY;
	}
#ifndef GSI_UNICODE
	gsiSafeStrcpyA(category->mName, tempStr, tempStrLen + 1);	
#endif

	category->mTeamsCount = gsXmlCountChildren(responseXml, "t");
	if (category->mTeamsCount == 0)
	{
		category->mTeams = NULL;
		return SCResult_NO_ERROR;
	}

	category->mTeams = (SCTeam *)gsimalloc(sizeof(SCTeam) * category->mTeamsCount);
	if (category->mTeams == NULL)
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, SC_MEMORY_ERROR);
		return SCResult_OUT_OF_MEMORY;
	}
	memset(category->mTeams, 0, sizeof(SCTeam) * category->mTeamsCount);

	for (i = 0; i < category->mTeamsCount; i++)
	{
		if (i == 0)
		{
			if (gsi_is_false(gsXmlMoveToChild(responseXml, "t")))
			{
				GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: p");
				return SCResult_RESPONSE_INVALID;
			}
		}
		else
		{
			if (gsi_is_false(gsXmlMoveToSibling(responseXml, "t")))
			{
				GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: p");
				return SCResult_RESPONSE_INVALID;
			}
		}
		result = sciWsParseTeam(responseXml, &category->mTeams[i]);
		if (result != SCResult_NO_ERROR)
		{
			GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse category->mTeams[%d]", i);
			return result;
		}
	}
	if (gsi_is_false(gsXmlMoveToParent(responseXml)))
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML");
		return SCResult_RESPONSE_INVALID;
	}
	return SCResult_NO_ERROR;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult sciWsParseTeamStatsQueryResponse(GSXmlStreamReader  responseXml,
										  SCTeamStatsQueryResponse **response)
{
	// We are parsing a single SCPlayerStatsQueryResponse, setting *response to the 
	// parsedResponse if everything parses correctly
	gsi_u32 i;
	SCResult result;
	SCTeamStatsQueryResponse *parsedResponse = NULL;
	if (gsi_is_false(gsXmlMoveToSibling(responseXml, "categories")))
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: categories");
		return SCResult_RESPONSE_INVALID;
	}

	parsedResponse = (SCTeamStatsQueryResponse *)gsimalloc(sizeof(SCTeamStatsQueryResponse));
	if (parsedResponse == NULL)
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, SC_MEMORY_ERROR);
		return SCResult_OUT_OF_MEMORY;
	}
	parsedResponse->mCategoriesCount = gsXmlCountChildren(responseXml, "c");
	if (parsedResponse->mCategoriesCount == 0)
	{
		parsedResponse->mCategories = NULL;
		return SCResult_NO_ERROR;
	}

	parsedResponse->mCategories = (SCTeamStatsCategory *)gsimalloc(sizeof(SCTeamStatsCategory) * parsedResponse->mCategoriesCount);
	if (parsedResponse->mCategories == NULL)
	{
		GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, SC_MEMORY_ERROR);
		return SCResult_OUT_OF_MEMORY;
	}
	memset(parsedResponse->mCategories, 0, sizeof(SCTeamStatsCategory) * parsedResponse->mCategoriesCount);

	for (i = 0; i < parsedResponse->mCategoriesCount; i++)
	{
		if (i == 0)
		{	
			// We only move to child on the first iteration
			if (gsi_is_false(gsXmlMoveToChild(responseXml, "c")))
			{
				GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: c");
				return SCResult_RESPONSE_INVALID;
			}			
		}
		else 
		{
			// the cursor now is able to move to sibling nodes
			if (gsi_is_false(gsXmlMoveToSibling(responseXml, "c")))
			{
				GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse XML elements: c");
				return SCResult_RESPONSE_INVALID;
			}
		}
		result = sciWsParseTeamStatsCategory(responseXml, &parsedResponse->mCategories[i]);
		if (result != SCResult_NO_ERROR)
		{
			GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed to parse parsedResponse->mCategories[%d]", i);
			return result;
		}
	}

	*response = parsedResponse;
	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void sciWsPlayerStatsQueryCallback(GHTTPResult        httpResult,
								   GSXmlStreamWriter  requestData,
								   GSXmlStreamReader  responseData,
								   void*        userData)
{
	SCResult aTranslatedResult	= SCResult_HTTP_ERROR;
	SCWebServices* webServices	= (SCWebServices*)userData;
	gsi_char *textMessage		= NULL;
	int textMessageLen			= 0;

	SCPlayerStatsQueryResponse *response	= NULL;

	GS_ASSERT(webServices != NULL);

	// Check for shutdown
	if (!webServices->mInit)
		return;

	GS_ASSERT(webServices->mPlayerStatsQueryPending);
	if (httpResult == GHTTPSuccess)
	{
		if (gsiCoreGetAuthErrorCode() == GSAuthErrorCode_None)
		{
			// Success! we can now read header information to get our result code, and debug text message
			aTranslatedResult = sciWsParseHeader(responseData, &textMessage, &textMessageLen);
			if (aTranslatedResult == SCResult_NO_ERROR)
			{
				gsi_time now;

				now = current_time();
				aTranslatedResult = sciWsParsePlayerStatsQueryResponse(responseData, &response);
				if (aTranslatedResult != SCResult_NO_ERROR)
				{
					scDestroyPlayerStatsQueryResponse(&response);
				}

				now = current_time() - now;
				GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_Notice, "Timer to parse: %d ms\n", now);
			}
		}
		else
		{
			char authError[AUTHERROR_LENGTH];
			gsiCoreGetAuthError(authError);
#ifdef GSI_UNICODE
			AsciiToUCS2String(goastrdup(authError), textMessage);
#else
			textMessage = goastrdup(authError);
#endif
			textMessageLen = _tcslen(textMessage);
			aTranslatedResult = sciAuthErrorToSCResult(gsiCoreGetAuthErrorCode());
		}
	}

	webServices->mPlayerStatsQueryPending = gsi_false;
	// we've checked this as part of the request and the webServices object below belongs to us.
	// so no need to check callback ptr
	webServices->mPlayerStatsQueryCallback(webServices->mInterface, 
		httpResult, 
		aTranslatedResult, 
		textMessage, 
		textMessageLen,
		response, 
		webServices->mPlayerStatsQueryUserData);
	gsifree(textMessage);
	GSI_UNUSED(requestData);
	GSI_UNUSED(responseData);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void sciWsGameStatsQueryCallback(GHTTPResult        httpResult,
								 GSXmlStreamWriter  requestData,
								 GSXmlStreamReader  responseData,
								 void*        userData)
{
	SCResult aTranslatedResult	= SCResult_HTTP_ERROR;
	SCWebServices* webServices	= (SCWebServices*)userData;
	gsi_char *textMessage		= NULL;
	int textMessageLen			= 0;

	SCGameStatsQueryResponse *response	= NULL;

	GS_ASSERT(webServices != NULL);

	// Check for shutdown
	if (!webServices->mInit)
		return;

	GS_ASSERT(webServices->mGameStatsQueryPending);
	if (httpResult == GHTTPSuccess)
	{
		if (gsiCoreGetAuthErrorCode() == GSAuthErrorCode_None)
		{
			// Success! we can now read header information to get our result code, and debug text message
			aTranslatedResult = sciWsParseHeader(responseData, &textMessage, &textMessageLen);
			if (aTranslatedResult == SCResult_NO_ERROR)
			{
				gsi_time now;

				now = current_time();
				aTranslatedResult = sciWsParseGameStatsQueryResponse(responseData, &response);
				if (aTranslatedResult != SCResult_NO_ERROR)
				{
					scDestroyGameStatsQueryResponse(&response);
				}

				GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_Notice, "Timer to parse: %d ms\n", current_time() - now);
			}
		}
		else
		{
			char authError[AUTHERROR_LENGTH];
			gsiCoreGetAuthError(authError);
#ifdef GSI_UNICODE
			AsciiToUCS2String(goastrdup(authError), textMessage);
#else
			textMessage = goastrdup(authError);
#endif
			textMessageLen = _tcslen(textMessage);
			aTranslatedResult = sciAuthErrorToSCResult(gsiCoreGetAuthErrorCode());
		}
	}

	webServices->mGameStatsQueryPending = gsi_false;
	// we've checked this as part of the request and the webServices object below belongs to us.
	// so no need to check callback ptr
	webServices->mGameStatsQueryCallback(webServices->mInterface, 
		httpResult, 
		aTranslatedResult, 
		textMessage, 
		textMessageLen,
		response, 
		webServices->mGameStatsQueryUserData);
	gsifree(textMessage);
	GSI_UNUSED(requestData);
	GSI_UNUSED(responseData);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void sciWsTeamStatsQueryCallback(GHTTPResult        httpResult,
								 GSXmlStreamWriter  requestData,
								 GSXmlStreamReader  responseData,
								 void*        userData)
{
	SCResult aTranslatedResult	= SCResult_HTTP_ERROR;
	SCWebServices* webServices	= (SCWebServices*)userData;
	gsi_char *textMessage		= NULL;
	int textMessageLen			= 0;

	SCTeamStatsQueryResponse *response	= NULL;

	GS_ASSERT(webServices != NULL);

	// Check for shutdown
	if (!webServices->mInit)
		return;

	GS_ASSERT(webServices->mTeamStatsQueryPending);
	if (httpResult == GHTTPSuccess)
	{
		if (gsiCoreGetAuthErrorCode() == GSAuthErrorCode_None)
		{
			// Success! we can now read header information to get our result code, and debug text message
			aTranslatedResult = sciWsParseHeader(responseData, &textMessage, &textMessageLen);
			if (aTranslatedResult == SCResult_NO_ERROR)
			{
				gsi_time now = current_time();
				aTranslatedResult = sciWsParseTeamStatsQueryResponse(responseData, &response);
				if (aTranslatedResult != SCResult_NO_ERROR)
				{
					scDestroyTeamStatsQueryResponse(&response);
				}

				now = current_time() - now;
				GS_SC_LOG(GSIDebugType_Misc, GSIDebugLevel_Notice, "Timer to parse: %d ms\n", now);
			}
		}
		else
		{
			char authError[AUTHERROR_LENGTH];
			gsiCoreGetAuthError(authError);
#ifdef GSI_UNICODE
			AsciiToUCS2String(goastrdup(authError), textMessage);
#else
			textMessage = goastrdup(authError);
#endif
			textMessageLen = _tcslen(textMessage);
			aTranslatedResult = sciAuthErrorToSCResult(gsiCoreGetAuthErrorCode());
		}
	}

	webServices->mTeamStatsQueryPending = gsi_false;
	// we've checked this as part of the request and the webServices object below belongs to us.
	// so no need to check callback ptr
	webServices->mTeamStatsQueryCallback(webServices->mInterface, 
		httpResult, 
		aTranslatedResult, 
		textMessage, 
		textMessageLen,
		response, 
		webServices->mPlayerStatsQueryUserData);
	gsifree(textMessage);
	GSI_UNUSED(requestData);
	GSI_UNUSED(responseData);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult sciWsSerializeStatsQuery(GSXmlStreamWriter writer,
								  char *queryType,
								  gsi_u32 gameId,
								  const GSLoginCertificate * certificate,
								  const GSLoginPrivateData * privateData,
								  int ruleSetVersion, 
								  const char *queryId, 
								  const SCQueryParameterListPtr queryParameters)
{
	gsi_u32 i;
	// Fill in the request data
	if (gsi_is_false(gsXmlWriteOpenTag(writer, SC_DATA_SERVICE_NAMESPACE, queryType)) ||
		gsi_is_false(gsXmlWriteOpenTag(writer, SC_DATA_SERVICE_NAMESPACE, SC_REQUEST)) ||
		gsi_is_false(gsXmlWriteOpenTag(writer, SC_DATA_SERVICE_NAMESPACE, SC_CERTIFICATE)) ||
		gsi_is_false(wsLoginCertWriteXML(certificate, SC_DATA_SERVICE_NAMESPACE, writer)) ||
		gsi_is_false(gsXmlWriteCloseTag(writer, SC_DATA_SERVICE_NAMESPACE, SC_CERTIFICATE)) ||
		gsi_is_false(gsXmlWriteHexBinaryElement(writer, SC_DATA_SERVICE_NAMESPACE, SC_CERT_PROOF, 
		(const gsi_u8*)privateData->mKeyHash, GS_CRYPT_MD5_HASHSIZE)) ||
		gsi_is_false(gsXmlWriteIntElement(writer, SC_DATA_SERVICE_NAMESPACE, SC_GAME_ID, gameId)) ||
		gsi_is_false(gsXmlWriteIntElement(writer, SC_DATA_SERVICE_NAMESPACE, SC_VERSION, ruleSetVersion)) ||
		gsi_is_false(gsXmlWriteStringElement(writer, SC_DATA_SERVICE_NAMESPACE, "queryid", queryId)))
	{

		GS_SC_LOG(GSIDebugType_Memory, GSIDebugLevel_HotError, SC_MEMORY_ERROR);
		return SCResult_OUT_OF_MEMORY;
	}	

	if (queryParameters)
	{
		if (gsi_is_false(gsXmlWriteOpenTag(writer, SC_DATA_SERVICE_NAMESPACE, "parameters")))
		{
			GS_SC_LOG(GSIDebugType_Memory, GSIDebugLevel_HotError, SC_MEMORY_ERROR);
			return SCResult_OUT_OF_MEMORY;
		}	

		for(i = 0; i < queryParameters->mCount; i++)
		{
			if (gsi_is_false(gsXmlWriteOpenTag(writer, SC_DATA_SERVICE_NAMESPACE, "parameter")) ||
				gsi_is_false(gsXmlWriteAsciiStringElement(writer, SC_DATA_SERVICE_NAMESPACE, "name", queryParameters->mQueryParams[i].mName)) ||
				gsi_is_false(gsXmlWriteAsciiStringElement(writer, SC_DATA_SERVICE_NAMESPACE, "value", queryParameters->mQueryParams[i].mValue)) ||
				gsi_is_false(gsXmlWriteCloseTag(writer, SC_DATA_SERVICE_NAMESPACE, "parameter")))
			{
				GS_SC_LOG(GSIDebugType_Memory, GSIDebugLevel_HotError, SC_MEMORY_ERROR);
				return SCResult_OUT_OF_MEMORY;
			}
		}
		if (gsi_is_false(gsXmlWriteCloseTag(writer, SC_DATA_SERVICE_NAMESPACE, "parameters")))
		{
			GS_SC_LOG(GSIDebugType_Memory, GSIDebugLevel_HotError, SC_MEMORY_ERROR);
			return SCResult_OUT_OF_MEMORY;
		}
	}


	if (gsi_is_false(gsXmlWriteCloseTag(writer, SC_DATA_SERVICE_NAMESPACE, SC_REQUEST)) || 
		gsi_is_false(gsXmlWriteCloseTag(writer, SC_DATA_SERVICE_NAMESPACE, queryType)) ||
		gsi_is_false(gsXmlCloseWriter(writer)))
	{
		GS_SC_LOG(GSIDebugType_Memory, GSIDebugLevel_HotError, SC_MEMORY_ERROR);
		return SCResult_OUT_OF_MEMORY;
	}

	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult sciWsRunPlayerStatsQuery(SCWebServices*     webServices,
								  gsi_u32            gameId,
								  const GSLoginCertificate * certificate,
								  const GSLoginPrivateData * privateData,
								  int ruleSetVersion, 
								  const char *queryId, 
								  const SCQueryParameterListPtr queryParameters, 
								  SCPlayerStatsQueryCallback callback, 
								  void *userData)
{
	SCResult result;
	GSXmlStreamWriter writer = NULL;

	// Check parameters
	GS_ASSERT(webServices != NULL);

	// Check for pending request
	// We want to make sure no two asynchronous web service calls of the same type are
	// executing at the same time.  
	if (webServices->mPlayerStatsQueryPending)
	{
		return SCResult_CALLBACK_PENDING;
	}

	// Create the XML message writer
	writer = gsXmlCreateStreamWriter(SC_DATA_SERVICE_NAMESPACES, SC_DATA_SERVICE_NAMESPACES_COUNT);
	if (writer == NULL)
	{
		GS_SC_LOG(GSIDebugType_Memory, GSIDebugLevel_HotError, SC_MEMORY_ERROR);
		return SCResult_OUT_OF_MEMORY;
	}

	result = sciWsSerializeStatsQuery(writer, "GetPlayerStats", gameId, certificate, privateData, ruleSetVersion, queryId, queryParameters);
	if (result != SCResult_NO_ERROR)
	{
		gsXmlFreeWriter(writer);
		return result;
	}

	// Set callback
	webServices->mPlayerStatsQueryCallback	= callback;
	webServices->mPlayerStatsQueryUserData	= userData;
	webServices->mPlayerStatsQueryPending	= gsi_true;

	// Execute soap call
	gsiExecuteSoap(scStatsDataServiceURL, 
		SC_DATA_RUN_PLAYERSTATS_QUERY_SOAPACTION,
		writer, 
		sciWsPlayerStatsQueryCallback, 
		webServices);

	return SCResult_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SCResult sciWsRunGameStatsQuery(SCWebServices* webServices, 
								gsi_u32 gameId, 
								const GSLoginCertificate * certificate, 
								const GSLoginPrivateData * privateData, 
								int ruleSetVersion, 
								const char *queryId,
								const SCQueryParameterListPtr queryParameters,
								SCGameStatsQueryCallback callback, 
								void *userData)
{
	GSXmlStreamWriter writer = NULL;
	SCResult result;

	// Check parameters
	GS_ASSERT(webServices != NULL);

	// Check for pending request
	// We want to make sure no two asynchronous web service calls of the same type are
	// executing at the same time.  
	if (webServices->mGameStatsQueryPending)
	{
		return SCResult_CALLBACK_PENDING;
	}

	// Create the XML message writer
	writer = gsXmlCreateStreamWriter(SC_DATA_SERVICE_NAMESPACES, SC_DATA_SERVICE_NAMESPACES_COUNT);
	if (writer == NULL)
		return SCResult_OUT_OF_MEMORY;

	// Fill in the request data
	result = sciWsSerializeStatsQuery(writer, "GetGameStats", gameId, certificate, privateData, ruleSetVersion, queryId, queryParameters);
	if (result != SCResult_NO_ERROR)
	{
		return result;
	}

	// Set callback
	webServices->mGameStatsQueryCallback	= callback;
	webServices->mGameStatsQueryUserData	= userData;
	webServices->mGameStatsQueryPending		= gsi_true;

	// Execute soap call
	gsiExecuteSoap(scStatsDataServiceURL, 
		SC_DATA_RUN_GAMESTATS_QUERY_SOAPACTION,
		writer, 
		sciWsGameStatsQueryCallback, 
		webServices);

	return SCResult_NO_ERROR;
}

SCResult sciWsRunTeamStatsQuery(SCWebServices*     webServices,
								gsi_u32            gameId,
								const GSLoginCertificate * certificate,
								const GSLoginPrivateData * privateData,
								int ruleSetVersion,
								const char *queryId,
								const SCQueryParameterListPtr queryParameters,
								SCTeamStatsQueryCallback callback,
								void *userData)
{
	GSXmlStreamWriter writer = NULL;
	SCResult result;

	// Check parameters
	GS_ASSERT(webServices != NULL);

	// Check for pending request
	// We want to make sure no two asynchronous web service calls of the same type are
	// executing at the same time.  
	if (webServices->mTeamStatsQueryPending)
	{
		return SCResult_CALLBACK_PENDING;
	}

	// Create the XML message writer
	writer = gsXmlCreateStreamWriter(SC_DATA_SERVICE_NAMESPACES, SC_DATA_SERVICE_NAMESPACES_COUNT);
	if (writer == NULL)
		return SCResult_OUT_OF_MEMORY;

	// Fill in the request data
	result = sciWsSerializeStatsQuery(writer, "GetTeamStats", gameId, certificate, privateData, ruleSetVersion, queryId, queryParameters);
	if (result != SCResult_NO_ERROR)
	{
		return result;
	}

	// Set callback
	webServices->mTeamStatsQueryCallback	= callback;
	webServices->mTeamStatsQueryUserData	= userData;
	webServices->mTeamStatsQueryPending		= gsi_true;

	// Execute soap call
	gsiExecuteSoap(scStatsDataServiceURL, 
		SC_DATA_RUN_TEAMSTATS_QUERY_SOAPACTION,
		writer, 
		sciWsTeamStatsQueryCallback, 
		webServices);

	return SCResult_NO_ERROR;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
