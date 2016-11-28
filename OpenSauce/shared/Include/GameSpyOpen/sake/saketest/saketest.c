///////////////////////////////////////////////////////////////////////////////
// File:	saketest.c
// SDK:		GameSpy Sake Persistent Storage SDK
//
// Copyright (c) IGN Entertainment, Inc.  All rights reserved.  
// This software is made available only pursuant to certain license terms offered
// by IGN or its subsidiary GameSpy Industries, Inc.  Unlicensed use or use in a 
// manner not expressly authorized by IGN or GameSpy is prohibited.

#include "../../common/gsCommon.h"
#include "../../common/gsAvailable.h"
#include "../../common/gsCore.h"
#include "../../common/gsSoap.h"
#include "../../sake/sake.h"
#include "../../ghttp/ghttp.h"
#include "../../GP/gp.h"
#include "../../webservices/AuthService.h"

#if defined(_WIN32) && !defined(_XBOX) && defined(_DEBUG) && defined(USE_CRTDBG)
#define CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#if defined(_NITRO)
	#include "../../common/nitro/screen.h"
	#define printf Printf
	#define vprintf VPrintf
#endif

#if 0
#define URL NULL

#define GAMENAME	_T("scotttest")
#define SECRET_KEY	_T("RPzJL7")
#define GAMEID		1600

#define PRODUCTID	0		
#define NAMESPACEID 0
#define NICKNAME    _T("saketest")
#define EMAIL       _T("saketest@saketest.com")
#define PASSWORD    _T("saketest")
#endif

#define URL NULL
#define GAMENAME	_T("gmtest")
#define SECRET_KEY	_T("HA6zkS")
#define GAMENAME_ASCII	"gmtest"
#define SECRET_KEY_ASCII	"HA6zkS"
#define GAMEID		0

//#define ACCESS_KEY     _T("d91f1c2c6a14197465b6b78b27f2d75e")
#define ACCESS_KEY     "39cf9714e24f421c8ca07b9bcb36c0f5"
#define PRODUCTID	0		
#define NAMESPACEID 1
#define NICKNAME    _T("saketest")
#define EMAIL       _T("saketest@saketest.com")
#define PASSWORD    _T("saketest")

#define CHECK_GP_RESULT(func, errString) if(func != GP_NO_ERROR) { printf("%s\n", errString); exit(0); }

#if defined(_WIN32)
	#define SAKE_UPLOAD_AMOUNT (128*1024)
	#define SAKE_DOWNLOAD_AMOUNT (128*1024*2)
#else
	#define SAKE_UPLOAD_AMOUNT (512*1024) //smaller stack size
	#define SAKE_DOWNLOAD_AMOUNT (512*1024*2)
#endif

#define MAX_NUM_TESTS	100

#ifdef _NITRO
extern int Heap_Print();
#endif


typedef struct DataStruct
{
	char pData[SAKE_UPLOAD_AMOUNT];
} DataStruct;

typedef struct TestResults
{
	gsi_bool	pass;
	char		errString[512];
	char		requestType[512];
} TestResults;

typedef struct sakeImageUlDlInfoStruct
{
    gsi_i32             gUploadedFileId;
    SAKEContentBuffer     gImageBuffer;
} sakeImageUlDlInfoStruct ;

sakeImageUlDlInfoStruct sakeTestImageData;

int NumOperations = 0;
GPConnection * pconn;
GPProfile profileid;
GSLoginCertificate certificate;
GSLoginPrivateData privateData;
int authenticated = 0;
TestResults results[MAX_NUM_TESTS];
int gIndex = 0;
int gNumOwned = 0;
SAKE sake;
FILE * file = NULL;

// Testing specific variables
int totalFailed = 0;
//=======================


#ifdef GSI_COMMON_DEBUG
	static void DebugCallback(GSIDebugCategory theCat, GSIDebugType theType,
	                          GSIDebugLevel theLevel, const char * theTokenStr,
	                          va_list theParamList)
	{
		GSI_UNUSED(theLevel);

		printf("[%s][%s] ", 
				gGSIDebugCatStrings[theCat], 
				gGSIDebugTypeStrings[theType]);

		vprintf(theTokenStr, theParamList);
	}
#endif
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//PROTOTYPES
static void Validate(char * requestType, void *inputData, void *outputData);
static void ReportFailure(char * reason);

///////////////////////////////////////////////////////////////////////////////
// file logging for Search Testing
#if !defined(NOFILE)
void logOutput(const char * buffer, int len)
{
#ifdef _NITRO
	int i;

	if(!buffer || !len)
		return;

	for(i = 0 ; i < len ; i++)
		OS_PutChar(buffer[i]);
#else
	if(!buffer || !len)
		return;

	if(file)
		fwrite(buffer, 1, len, file);
	
#endif
}
#endif

///////////////////////////////////////////////////////////////////////////////
//Big Validation function to ensure integrity of data received
static void Validate(char * requestType, void *inputData, void *outputData)
{
	SAKEField ** records;
	SAKEField * field;
	int recordIndex;
	int fieldIndex;

	if (!results[gIndex].pass) //if already failed, dont change failure reason
		return;
	

	if (strcmp(requestType,"GetMyRecords") == 0)
	{
		SAKEGetMyRecordsInput * input = (SAKEGetMyRecordsInput *)inputData;
		SAKEGetMyRecordsOutput * output = (SAKEGetMyRecordsOutput *)outputData;

		records = output->mRecords;

		//verify numrecords == numowned
		if (output->mNumRecords != gNumOwned)
			ReportFailure("num records does not match num owned");

		//verify all data has ownerid == my profileid
		for(recordIndex = 0 ; recordIndex < output->mNumRecords ; recordIndex++)
		{
			for(fieldIndex = 0 ; fieldIndex < input->mNumFields ; fieldIndex++)
			{
				field = &records[recordIndex][fieldIndex];
				if (strcmp(field->mName,"ownerid") == 0 && field->mValue.mInt != profileid)
					ReportFailure("ownerid doesnt match profileid");
			}
		}
	}

	if (strcmp(requestType,"GetSpecificRecords") == 0)
	{
		SAKEGetSpecificRecordsInput * input = (SAKEGetSpecificRecordsInput *)inputData;
		SAKEGetSpecificRecordsOutput * output = (SAKEGetSpecificRecordsOutput *)outputData;

		records = output->mRecords;

		//verify the right number of recordids where retrieved
		if (output->mNumRecords != input->mNumRecordIds)
			ReportFailure("num records does not match num record ids specified");

		//verify all recorids where retrieved
		for(recordIndex = 0 ; recordIndex < output->mNumRecords ; recordIndex++)
		{
			for(fieldIndex = 0 ; fieldIndex < input->mNumFields ; fieldIndex++)
			{
				field = &records[recordIndex][fieldIndex];
				if (strcmp(field->mName,"recordid") == 0 && field->mValue.mInt != input->mRecordIds[recordIndex])
					ReportFailure("recordid not requested");
			}
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void ProcessTasks()
{	
	printf("Procesing\n");
	if (NumOperations > 1)
		printf("WHY ARE TWO TASKS PROCESSING HERE!?");
	
	// Think to process tasks
	while(NumOperations > 0)
	{
	    printf("Thinking\n");
		msleep(5);
        ghttpThink();
		gsCoreThink(0);
	}
	printf("Processing Done  \n");
}

///////////////////////////////////////////////////////////////////////////////

static GSIACResult CheckServices(void)
{
	GSIACResult aResult;
	GSIStartAvailableCheck(GAMENAME);

	// Continue processing while the check is in progress
	do
	{
		aResult = GSIAvailableCheckThink();
		msleep(10);
	}
	while(aResult == GSIACWaiting);

	// Check the result
	switch(aResult)
	{
	case GSIACAvailable:
		printf("Online Services are available\r\n");
		break;
	case GSIACUnavailable:
		printf("Online services are unavailable\r\n");
		printf("Please visit www.mygame.com for more information.\r\n");
		break;
	case GSIACTemporarilyUnavailable:
		printf("Online services are temporarily unavailable.\r\n");
		printf("Please visit www.mygame.com for more information.\r\n");
		break;
	default:
		break;
	};
		
	return aResult;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void myLoginCallback(GHTTPResult httpResult, WSLoginResponse * theResponse, void * userData)
{
	if (httpResult != GHTTPSuccess)
	{
		printf("Failed on player login, HTTP error: %d", httpResult);
		exit(0);
	}
	else if (theResponse->mLoginResult != WSLogin_Success)
	{
		printf("Failed on player login, Login result: %d", theResponse->mLoginResult);
		exit(0);
	}
	else
	{
		// copy certificate and private key
		profileid = theResponse->mCertificate.mProfileId;
		memcpy(&certificate, &theResponse->mCertificate, sizeof(GSLoginCertificate));
		memcpy(&privateData, &theResponse->mPrivateData, sizeof(GSLoginPrivateData));

		authenticated = 1; // so we know we can stop gsCore thinking
		_tprintf(_T("Player '%s' logged in.\n"), theResponse->mCertificate.mUniqueNick);
	}
	GSI_UNUSED(userData);
}

static void LoginAndAuthenticate()
{
	wsSetGameCredentials(ACCESS_KEY, GAMEID, SECRET_KEY_ASCII);

	if (0 != wsLoginProfile(GAMEID, GP_PARTNERID_GAMESPY, NAMESPACEID, NICKNAME, EMAIL, PASSWORD, _T(""), myLoginCallback, NULL))
	{
		printf("Failed on wsLoginProfile");
		exit(0);
	}

	while(authenticated == 0)
	{
		msleep(100);
		gsCoreThink(0);
	}

	// authenticate Sake with the login ticket and the profileid returned from login
	printf("Authenticating with Sake\n");
	sakeSetGame(sake, GAMENAME, GAMEID, SECRET_KEY);
	sakeSetProfile(sake, profileid, &certificate, &privateData);
}

static void PrintResults()
{
	int i;
	int passed = 0;
	int failed = 0;

	printf("\n=======================================================\n");
	printf("==================== FINAL RESULTS ====================\n");
	printf("=======================================================\n");
	printf("\n*Note: ReportRecord is expected to fail since the record will have already been reported by the same profile.\n\n");


	for (i=0; i<gIndex; i++)
	{
		printf("#%3d - %s: ", i+1, results[i].requestType);
		if (results[i].pass)
		{
			printf("PASS\n");
			passed++;
		}
		else
		{
			printf("FAIL\t[%s]\n", results[i].errString);
			// Since Report Record should fail and it's the first test, don't fail if it does.
			if(i != 0)
			{
				failed++;
			}
			else
			{
				passed++;
			}
		}
	}
	printf("TOTAL PASSED = %d\n", passed);
	printf("TOTAL FAILED = %d\n\n", failed);

	totalFailed = failed;

	if (failed == 0)
	{
		printf("         ALL TEST PASSED - WOOHOO!!!!\n\n");
	}
}

static void ReportFailure(char * reason)
{
	sprintf(results[gIndex].errString, "%s", reason);
	results[gIndex].pass = gsi_false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static int UnicodeStringLen(const unsigned short * str)
{
	const unsigned short * end = str;
	while(*end++)
		{}
	return (end - str - 1);
}

static void PrintSeperator(void)
{
	printf("*******************\n");
}

static const char * FieldTypeToString(SAKEField * field)
{
	static char buffer[32];
	SAKEFieldType type = field->mType;

	if(type == SAKEFieldType_BYTE)
		return "byte";
	if(type == SAKEFieldType_SHORT)
		return "short";
	if(type == SAKEFieldType_INT)
		return "int";
	if(type == SAKEFieldType_FLOAT)
		return "float";
	if(type == SAKEFieldType_ASCII_STRING)
		return "asciiString";
	if(type == SAKEFieldType_UNICODE_STRING)
		return "unicodeString";
	if(type == SAKEFieldType_BOOLEAN)
		return "boolean";
	if(type == SAKEFieldType_DATE_AND_TIME)
		return "dateAndTime";
	if(type == SAKEFieldType_BINARY_DATA)
	{
		sprintf(buffer, "binaryData-%d", field->mValue.mBinaryData.mLength);
		return buffer;
	}
	if (type == SAKEFieldType_INT64)
	{
		return "int64";
	}
	return "ERROR!!  Invalid value type set";
}

static const char * FieldValueToString(SAKEField * field)
{
	static char buffer[64];
	SAKEFieldType type = field->mType;
	SAKEValue * value = &field->mValue;

	if(type == SAKEFieldType_BYTE)
		sprintf(buffer, "%d", (int)value->mByte);
	else if(type == SAKEFieldType_SHORT)
		sprintf(buffer, "%d", (int)value->mShort);
	else if(type == SAKEFieldType_INT)
		sprintf(buffer, "%d", value->mInt);
	else if(type == SAKEFieldType_FLOAT)
		sprintf(buffer, "%0.3f", value->mFloat);
	else if(type == SAKEFieldType_ASCII_STRING)
		return value->mAsciiString;
	else if(type == SAKEFieldType_UNICODE_STRING)
	{
		// cap the value if it is too long (note, this is destructive)
		if(UnicodeStringLen(value->mUnicodeString) > 20)
			value->mUnicodeString[20] = 0;
		UCS2ToAsciiString(value->mUnicodeString, buffer);
	}
	else if(type == SAKEFieldType_BOOLEAN)
		return (value->mBoolean)?"true":"false";
	else if(type == SAKEFieldType_DATE_AND_TIME)
	{
		char * str = gsiSecondsToString(&value->mDateAndTime);
		str[strlen(str) - 1] = '\0';
		return str;
	}
	else if(type == SAKEFieldType_BINARY_DATA)
	{
		int i;
		int len = GS_MIN(value->mBinaryData.mLength, 8);
		for(i = 0 ; i < len ; i++)
			sprintf(buffer + (len*2), "%0X", value->mBinaryData.mValue[i]);
		buffer[len*2] = '\0';
	}
	else if(type == SAKEFieldType_INT64)
	{
		gsiInt64ToString(buffer, value->mInt64);
	}
	else
		return "ERROR!!  Invalid value type set";

	return buffer;
}

static const char * RequestResultToString(SAKERequestResult requestResult)
{
	switch(requestResult)
	{
	case SAKERequestResult_SUCCESS:
		return "SUCCESS";
	case SAKERequestResult_CONNECTION_TIMEOUT:
		return "CONNECTION_TIMEOUT";
	case SAKERequestResult_CONNECTION_ERROR:
		return "CONNECTION_ERROR";
	case SAKERequestResult_MALFORMED_RESPONSE:
		return "MALFORMED_RESPONSE";
	case SAKERequestResult_OUT_OF_MEMORY:
		return "OUT_OF_MEMORY";
	case SAKERequestResult_DATABASE_UNAVAILABLE:
		return "DATABASE_UNAVAILABLE";
	case SAKERequestResult_LOGIN_TICKET_INVALID:
		return "LOGIN_TICKET_INVALID";
	case SAKERequestResult_LOGIN_TICKET_EXPIRED:
		return "LOGIN_TICKET_EXPIRED";
	case SAKERequestResult_TABLE_NOT_FOUND:
		return "TABLE_NOT_FOUND";
	case SAKERequestResult_RECORD_NOT_FOUND:
		return "RECORD_NOT_FOUND";
	case SAKERequestResult_FIELD_NOT_FOUND:
		return "FIELD_NOT_FOUND";
	case SAKERequestResult_FIELD_TYPE_INVALID:
		return "FIELD_TYPE_INVALID";
	case SAKERequestResult_NO_PERMISSION:
		return "NO_PERMISSION";
	case SAKERequestResult_RECORD_LIMIT_REACHED:
		return "RECORD_LIMIT_REACHED";
	case SAKERequestResult_ALREADY_RATED:
		return "ALREADY_RATED";
	case SAKERequestResult_ALREADY_REPORTED:
		return "ALREADY_REPORTED";
	case SAKERequestResult_NOT_RATEABLE:
		return "NOT_RATEABLE";
	case SAKERequestResult_NOT_OWNED:
		return "NOT_OWNED";
	case SAKERequestResult_FILTER_INVALID:
		return "FILTER_INVALID";
	case SAKERequestResult_SORT_INVALID:
		return "SORT_INVALID";
	case SAKERequestResult_TARGET_FILTER_INVALID:
		return "TARGET_FILTER_INVALID";
	case SAKERequestResult_CERTIFICATE_INVALID:
		return "CERTIFICATE_INVALID";
	case SAKERequestResult_UNKNOWN_ERROR:
		return "UNKNOWN_ERROR";
    case SAKERequestResult_REQUEST_CANCELLED:
        return "REQUEST_CANCELLED";
	case SAKERequestResult_CONTENTSERVER_FAILURE:
	    return "CONTENTSERVER_FAILURE";
	case SAKERequestResult_INVALID_GAMEID:
		return "INVALID_GAMEID";
	case SAKERequestResult_INVALID_SESSIONTOKEN:
		return "INVALID_SESSIONTOKEN";
	case SAKERequestResult_SESSIONTOKEN_EXPIRED:
		return "SESSIONTOKEN_EXPIRED";
	default:
		break;
	}

	return "Unrecognized error";
}

// prints request result, returns gsi_false for errors
static gsi_bool HandleRequestResult(SAKERequestResult requestResult, const char * requestType)
{

    printf("%s Callback Result %s\n", requestType, RequestResultToString(requestResult));
    
	sprintf(results[gIndex].requestType, "%s", requestType);
	
	if(requestResult != SAKERequestResult_SUCCESS)
	{
		sprintf(results[gIndex].errString, "Error - %s", RequestResultToString(requestResult));
		results[gIndex].pass = gsi_false;
		return gsi_false;
	}

	results[gIndex].pass = gsi_true;
	return gsi_true;
}

static void DisplayReadResults(SAKEField ** records, int numRecords, int numFields)
{
	SAKEField * field;
	int recordIndex;
	int fieldIndex;

	PrintSeperator();
	printf("Num Records: %d\n", numRecords);
	PrintSeperator();

	for(recordIndex = 0 ; recordIndex < numRecords ; recordIndex++)
	{
	   field = &records[recordIndex][0];
			printf("%s[%s]=%s\n", field->mName, FieldTypeToString(field), FieldValueToString(field));

		for(fieldIndex = 0 ; fieldIndex < numFields ; fieldIndex++)
		{
			field = &records[recordIndex][fieldIndex];
			printf("%s[%s]=%s\n", field->mName, FieldTypeToString(field), FieldValueToString(field));
		}

		PrintSeperator();
	}
}


static void LogReadResults(SAKEField ** records, int numRecords, int numFields)
{
	SAKEField * field;
	int recordIndex = 0;
	int fieldIndex;
	char buffer[4086] = {0};

#if !defined(NOFILE)
	sprintf(buffer,"*******************\r\nNum Records: %d\r\n*******************\r\n", numRecords);
	logOutput(buffer, strlen(buffer));

	if (numRecords == 0)
		return;

	//first pass, write the fields out
	for(fieldIndex = 0 ; fieldIndex < numFields ; fieldIndex++)
	{
		field = &records[recordIndex][fieldIndex];
		sprintf(buffer,"%15s|", field->mName);
		logOutput(buffer, strlen(buffer));
	}
	sprintf(buffer,"\r\n");
	logOutput(buffer, strlen(buffer));


	for(recordIndex = 0 ; recordIndex < numRecords ; recordIndex++)
	{
		for(fieldIndex = 0 ; fieldIndex < numFields ; fieldIndex++)
		{
			field = &records[recordIndex][fieldIndex];
			sprintf(buffer,"%15s|", FieldValueToString(field));
			logOutput(buffer, strlen(buffer));
		}
		sprintf(buffer,"\r\n");
		logOutput(buffer, strlen(buffer));
	}
#else
	sprintf(buffer,"*******************\r\nNum Records: %d\r\n*******************\r\n", numRecords);
	printf("%s",buffer);

	if (numRecords == 0)
		return;

	//first pass, write the fields out
	for(fieldIndex = 0 ; fieldIndex < numFields ; fieldIndex++)
	{
		field = &records[recordIndex][fieldIndex];
		sprintf(buffer,"%15s|", field->mName);
		printf("%s",buffer);
	}
	sprintf(buffer,"\r\n");
	printf("%s",buffer);


	for(recordIndex = 0 ; recordIndex < numRecords ; recordIndex++)
	{
		for(fieldIndex = 0 ; fieldIndex < numFields ; fieldIndex++)
		{
			field = &records[recordIndex][fieldIndex];
			sprintf(buffer,"%15s|", FieldValueToString(field));
			printf("%s",buffer);
		}
		sprintf(buffer,"\r\n");
		printf("%s",buffer);
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void ReportRecordCallback(SAKE sake, SAKERequest request, SAKERequestResult result, void *inputData, void *outputData, void *userData)
{
	NumOperations--;

	GSI_UNUSED(sake);
	GSI_UNUSED(request);
	GSI_UNUSED(inputData);
	GSI_UNUSED(outputData);
	GSI_UNUSED(userData);

	if(HandleRequestResult(result, "ReportRecord") == gsi_false)
		return;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void CreateRecordCallback(SAKE sake, SAKERequest request, SAKERequestResult result, void *inputData, void *outputData, void *userData)
{
	SAKECreateRecordOutput * output = (SAKECreateRecordOutput *)outputData;
	int * recordid = (int *)userData;

	NumOperations--;

	if(HandleRequestResult(result, "CreateRecord") == gsi_false)
		return;
	
	*recordid = output->mRecordId;
	printf("Created recordid %d\n", output->mRecordId);

	GSI_UNUSED(sake);
	GSI_UNUSED(request);
	GSI_UNUSED(inputData);
	GSI_UNUSED(userData);
}

static void UpdateRecordCallback(SAKE sake, SAKERequest request, SAKERequestResult result, void *inputData, void *outputData, void *userData)
{
	NumOperations--;
	
	GSI_UNUSED(sake);
	GSI_UNUSED(request);
	GSI_UNUSED(inputData);
	GSI_UNUSED(outputData);
	GSI_UNUSED(userData);

	if(HandleRequestResult(result, "UpdateRecord") == gsi_false)
		return;
}

static void DeleteRecordCallback(SAKE sake, SAKERequest request, SAKERequestResult result, void *inputData, void *outputData, void *userData)
{
	NumOperations--;

	GSI_UNUSED(sake);
	GSI_UNUSED(request);
	GSI_UNUSED(inputData);
	GSI_UNUSED(outputData);
	GSI_UNUSED(userData);

	if(HandleRequestResult(result, "DeleteRecord") == gsi_false)
		return;
}

static void SearchForRecordsCallback(SAKE sake, SAKERequest request, SAKERequestResult result, void *inputData, void *outputData, void *userData)
{
	SAKESearchForRecordsInput * input = (SAKESearchForRecordsInput *)inputData;
	SAKESearchForRecordsOutput * output = (SAKESearchForRecordsOutput *)outputData;

	NumOperations--;

	GSI_UNUSED(sake);
	GSI_UNUSED(request);
	GSI_UNUSED(userData);

	if(HandleRequestResult(result, "SearchForRecords") != gsi_false)
    {
	    LogReadResults(output->mRecords, output->mNumRecords, input->mNumFields);
    }

    // release the memory allocated for search
    if (input->mOwnerIds) gsifree(input->mOwnerIds);
    if (input->mFieldNames) gsifree(input->mFieldNames);
}

static void GetMyRecordsCallback(SAKE sake, SAKERequest request, SAKERequestResult result, void *inputData, void *outputData, void *userData)
{
	SAKEGetMyRecordsInput * input = (SAKEGetMyRecordsInput *)inputData;
	SAKEGetMyRecordsOutput * output = (SAKEGetMyRecordsOutput *)outputData;

	NumOperations--;

	GSI_UNUSED(sake);
	GSI_UNUSED(request);
	GSI_UNUSED(userData);

	if(HandleRequestResult(result, "GetMyRecords") == gsi_false)
		return;

	DisplayReadResults(output->mRecords, output->mNumRecords, input->mNumFields);

	//Now validate the data and ensure its ours
	Validate("GetMyRecords", input, output);
}

/*
static void GetSpecificRecordsCallback(SAKE sake, SAKERequest request, SAKERequestResult result, void *inputData, void *outputData, void *userData)
{
	SAKEGetSpecificRecordsInput * input = (SAKEGetSpecificRecordsInput *)inputData;
	SAKEGetSpecificRecordsOutput * output = (SAKEGetSpecificRecordsOutput *)outputData;

	NumOperations--;

	GSI_UNUSED(sake);
	GSI_UNUSED(request);
	GSI_UNUSED(userData);

	if(HandleRequestResult(result, "GetSpecificRecords") == gsi_false)
		return;

	if(output->mNumRecords > 0)
	{
		SAKEField * field;
		field = sakeGetFieldByName("score", output->mRecords[0], input->mNumFields);
		GSI_UNUSED(field);
	}
	else
	{
		printf("No record found\n");
	}

	DisplayReadResults(output->mRecords, output->mNumRecords, input->mNumFields);

	//Now validate the data and ensure its the records we requested
	Validate("GetSpecificRecords", input, output);
}

static void GetRandomRecordCallback(SAKE sake, SAKERequest request, SAKERequestResult result, void *inputData, void *outputData, void *userData)
{
	SAKEGetRandomRecordInput * input = (SAKEGetRandomRecordInput *)inputData;
	SAKEGetRandomRecordOutput * output = (SAKEGetRandomRecordOutput *)outputData;
	SAKEField *records[1];
	int        numRecords;

	NumOperations--;

	GSI_UNUSED(sake);
	GSI_UNUSED(request);
	GSI_UNUSED(userData);

	if(HandleRequestResult(result, "GetRandomRecord") == gsi_false)
		return;

	records[0] = output->mRecord;
	if(records[0] != NULL)
		numRecords = 1;
	else
		numRecords = 0;

	DisplayReadResults(records, numRecords, input->mNumFields);
}
*/

static void RateRecordCallback(SAKE sake, SAKERequest request, SAKERequestResult result, void *inputData, void *outputData, void *userData)
{
	SAKERateRecordOutput * output = (SAKERateRecordOutput *)outputData;

	NumOperations--;

	GSI_UNUSED(sake);
	GSI_UNUSED(request);
	GSI_UNUSED(inputData);
	GSI_UNUSED(userData);

	if(HandleRequestResult(result, "RateRecord") == gsi_false)
		return;

	printf("NumRatings: %d\n", output->mNumRatings);
	printf("AverageRating: %0.3f\n", output->mAverageRating);
}

static void GetRecordLimitCallback(SAKE sake, SAKERequest request, SAKERequestResult result, void *inputData, void *outputData, void *userData)
{
	SAKEGetRecordLimitOutput * output = (SAKEGetRecordLimitOutput *)outputData;

	NumOperations--;

	GSI_UNUSED(sake);
	GSI_UNUSED(request);
	GSI_UNUSED(inputData);
	GSI_UNUSED(userData);

	if(HandleRequestResult(result, "GetRecordLimit") == gsi_false)
		return;

	printf("LimitPerOwner: %d\n", output->mLimitPerOwner);
	printf("NumOwned: %d\n", output->mNumOwned);

	gNumOwned = output->mNumOwned;
}

static void GetRecordCountCallback(SAKE sake, SAKERequest request, SAKERequestResult result, void *inputData, void *outputData, void *userData)
{
	SAKEGetRecordCountOutput * output = (SAKEGetRecordCountOutput *)outputData;
	int * count = (int *)userData;

	NumOperations--;
    
	GSI_UNUSED(sake);
	GSI_UNUSED(request);
	GSI_UNUSED(inputData);
	GSI_UNUSED(userData);

	if(HandleRequestResult(result, "GetRecordCount") == gsi_false)
		return;

	*count = output->mCount;
	printf("Record Count: %d\n", output->mCount);
}

static GHTTPBool gUploadResult;
/*static int gUploadedFileId;
// we shan't need this since we have upload content
static GHTTPBool UploadCompletedCallback(GHTTPRequest request, GHTTPResult result, char * buffer, GHTTPByteCount bufferLen, char * headers, void * param)
{
	SAKEFileResult fileResult;

	gUploadResult = GHTTPFalse;
	NumOperations--;

	GSI_UNUSED(param);
	GSI_UNUSED(bufferLen);
	GSI_UNUSED(buffer);
	GSI_UNUSED(headers);

	sprintf(results[gIndex].requestType, "UploadFile");
	results[gIndex].pass = gsi_false;

	if(result != GHTTPSuccess)
	{
		printf("File Upload: GHTTP Error: %d\n", result);
		return GHTTPTrue;
	}

	if(!sakeGetFileResultFromHeaders(ghttpGetHeaders(request), &fileResult))
	{
		printf("File Upload: Failed to find Sake-File-Result header\n");
		return GHTTPTrue;
	}

	if(fileResult != SAKEFileResult_SUCCESS)
	{
		printf("File Upload: SakeFileResult != success: %d\n", fileResult);
		return GHTTPTrue;
	}

	if(!sakeGetFileIdFromHeaders(ghttpGetHeaders(request), &gUploadedFileId))
	{
		printf("File Upload: Unable to parse Sake-File-Id header\n");
		return GHTTPTrue;
	}

	printf("File Upload: Uploaded fileId: %d\n", gUploadedFileId);
	gUploadResult = GHTTPTrue;
	results[gIndex].pass = gsi_true;

	return GHTTPTrue;
}*/

static void UploadContentProgressCallback(SAKE sake, 
                                          gsi_u32 bytesTransfered, 
                                          gsi_u32 totalSize, 
                                          void *userData)
{
    printf("Uploaded %d out of %d bytes....\n", bytesTransfered, totalSize);
    GSI_UNUSED(sake);
    GSI_UNUSED(userData);
}

static void UploadContentCompletedCallback(SAKE sake, SAKERequestResult result, gsi_i32 fileid, SAKEFileResult fileResult, void *userData)
{

    gUploadResult = GHTTPFalse;
    NumOperations--;

    printf("============ Upload Content Response START =============\n");

    if(HandleRequestResult(result, "UploadContent") == gsi_false)
    {
        printf("Upload Content failed .\n");
    }
    else
    {
        if(fileid != 0)
        {
            sakeTestImageData.gUploadedFileId = fileid;
            printf("Success - uploaded File ID %d \n", fileid);
            gUploadResult = GHTTPTrue;
        }
        else
        {
            printf("Update of file has failed!!!!, file id is 0, file result %d\n", fileResult);
            results[gIndex].pass = gsi_false;
            sprintf(results[gIndex].errString, "Error - file id is 0, file result %d", fileResult);
        }
    }
    printf("============ Upload Content Response END  =============\n");
   
    GSI_UNUSED(userData);
    GSI_UNUSED(sake);
}

/*To be deprecated
#ifndef NOFILE

static GHTTPBool DownloadCompletedCallback(GHTTPRequest request, GHTTPResult result, char * buffer, GHTTPByteCount bufferLen, char * headers, void * param)
{
	SAKEFileResult fileResult;
	int * numBytes = (int *)param;

	NumOperations--;

	GSI_UNUSED(param);
	GSI_UNUSED(buffer);
	GSI_UNUSED(headers);

	sprintf(results[gIndex].requestType, "DownloadFile");
	results[gIndex].pass = gsi_false;

	if(result != GHTTPSuccess)
	{
		printf("File Download: GHTTP Error: %d\n", result);
		return GHTTPTrue;
	}

	if(!sakeGetFileResultFromHeaders(ghttpGetHeaders(request), &fileResult))
	{
		printf("File Download: Failed to find Sake-File-Result header\n");
		return GHTTPTrue;
	}

	if(fileResult != SAKEFileResult_SUCCESS)
	{
		printf("File Download: SakeFileResult != success: %d\n", fileResult);
		return GHTTPTrue;
	}

	printf("File Download: Downloaded %d byte file\n", bufferLen);

	*numBytes = (int)bufferLen;
	results[gIndex].pass = gsi_true;
 
	return GHTTPTrue;
}
#endif
*/



static void DownloadContentProgressCallback(SAKE sake, 
                                            gsi_u32 bytesTransfered, 
                                            gsi_u32 totalSize, 
                                            void *userData)
{
    printf("Downloaded %d out of %d bytes....\n", bytesTransfered, totalSize);
    GSI_UNUSED(sake);
    GSI_UNUSED(userData);
}

static void DownloadContentCompletedCallback(SAKE     sake, 
                                             SAKERequestResult result,
                                             SAKEFileResult fileResult, 
                                             char *buffer, 
                                             gsi_i32  bufferLength, 
                                             void     *userData)
{

    sprintf(results[gIndex].requestType, "DownloadContent");
    NumOperations--;
    printf("============ Download Content Response START =============\n");

    if(HandleRequestResult(result, "DownloadContent") == gsi_false)
    {
        printf("Download Content failed, file result = %d .\n", fileResult);
    }
    else
    {

        printf("Successfully downloaded file!!\n");
        if (buffer != NULL && bufferLength > 0)
        {
            // we must have asked for a download to memory 
            // so update the sake image test data with it.
            sakeTestImageData.gImageBuffer.mBuffer = buffer;
            sakeTestImageData.gImageBuffer.mLength = bufferLength;
        }
        results[gIndex].pass = gsi_true;
    }
    printf("============ Download Content Response END  =============\n");

    GSI_UNUSED(userData);
    GSI_UNUSED(sake);
}


/*static void postCallback(GHTTPRequest request, int bytesPosted, int totalBytes, int objectsPosted, int totalObjects, void * param)
{
	printf("==============================\n");
	printf("* bytesPosted: %7d\n", bytesPosted);
	printf("* totalBytes: %7d\n", totalBytes);
	printf("* objectsPosted: %7d\n", objectsPosted);
	printf("* totalObjects: %7d\n", totalObjects);
	GSI_UNUSED(request);
	GSI_UNUSED(param);
}*/

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void ReportRecord(SAKEReportRecordInput * input)
{
	SAKERequest request;
	SAKEStartRequestResult startRequestResult;

	printf("%d) Starting ReportRecord ================\n", gIndex+1);

	request = sakeReportRecord(sake, input, ReportRecordCallback, NULL);
	if(!request)
	{
		startRequestResult = sakeGetStartRequestResult(sake);
		sprintf(results[gIndex].errString, "Failed to start request: %d", startRequestResult);
		results[gIndex].pass = gsi_false;
	}
	else
	{
		NumOperations++;
	}

	ProcessTasks();
	gIndex++; //increment global task counter
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static int CreateRecord(SAKECreateRecordInput * input)
{
		SAKERequest request;
		SAKEStartRequestResult startRequestResult;
	int recordid = 0;

	printf("%d) Starting CreateRecord ================\n", gIndex+1);

	request = sakeCreateRecord(sake, input, CreateRecordCallback, &recordid);
	if(!request)
	{
		startRequestResult = sakeGetStartRequestResult(sake);
	    sprintf(results[gIndex].errString, "Failed to start request: %d", startRequestResult);
	    results[gIndex].pass = gsi_false;
	}
	else
	{
		NumOperations++;
	}

	ProcessTasks();
	gIndex++; //increment global task counter

	return recordid;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void UpdateRecord(SAKEUpdateRecordInput * input)
{
		SAKERequest request;
		SAKEStartRequestResult startRequestResult;
		
	printf("%d) Starting UpdateRecord ================\n", gIndex+1);

	request = sakeUpdateRecord(sake, input, UpdateRecordCallback, NULL);
		if(!request)
		{
			startRequestResult = sakeGetStartRequestResult(sake);
		sprintf(results[gIndex].errString, "Failed to start request: %d", startRequestResult);
		results[gIndex].pass = gsi_false;
		}
		else
		{
			NumOperations++;
		}

	ProcessTasks();
	gIndex++; //increment global task counter
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void DeleteRecord(char * table, int recordid)
{
	SAKEDeleteRecordInput input;
		SAKERequest request;
		SAKEStartRequestResult startRequestResult;

	input.mTableId = table;
	input.mRecordId = recordid;

	printf("%d) Starting DeleteRecord ================\n", gIndex+1);

		request = sakeDeleteRecord(sake, &input, DeleteRecordCallback, NULL);
		if(!request)
		{
			startRequestResult = sakeGetStartRequestResult(sake);
		sprintf(results[gIndex].errString, "Failed to start request: %d", startRequestResult);
		results[gIndex].pass = gsi_false;
		}
		else
		{
			NumOperations++;
		}

	ProcessTasks();
	gIndex++; //increment global task counter
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void GetMyRecords(SAKEGetMyRecordsInput * input)
{

		static SAKERequest request;
		SAKEStartRequestResult startRequestResult;

	printf("%d) Starting GetMyRecords ================\n", gIndex+1);

	request = sakeGetMyRecords(sake, input, GetMyRecordsCallback, NULL);
		if(!request)
		{
			startRequestResult = sakeGetStartRequestResult(sake);
		sprintf(results[gIndex].errString, "Failed to start request: %d", startRequestResult);
		results[gIndex].pass = gsi_false;
		}
		else
		{
			NumOperations++;
		}

	ProcessTasks();
	gIndex++; //increment global task counter
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
static void GetSpecificRecords(SAKEGetSpecificRecordsInput * input)
{
		static SAKERequest request;
		SAKEStartRequestResult startRequestResult;

	printf("%d) Starting GetSpecificRecords ================\n", gIndex+1);

	request = sakeGetSpecificRecords(sake, input, GetSpecificRecordsCallback, NULL);
		if(!request)
		{
			startRequestResult = sakeGetStartRequestResult(sake);
		sprintf(results[gIndex].errString, "Failed to start request: %d", startRequestResult);
		results[gIndex].pass = gsi_false;
		}
		else
		{
			NumOperations++;
		}

	ProcessTasks();
	gIndex++; //increment global task counter
}
*/

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void SearchForRecords(SAKESearchForRecordsInput * input, const char * testType)
{
		static SAKERequest request;
		SAKEStartRequestResult startRequestResult;
	char buffer[512] = {0};

	if (testType == NULL || strlen(testType) == 0)
		sprintf(buffer, "\r\n%d) SearchForRecordsUnit Test\r\n", gIndex+1);
	else
		sprintf(buffer, "\r\n%d) SearchForRecords: TestType === %s ===\r\n", gIndex+1, testType);
#if !defined(NOFILE)
	logOutput(buffer, strlen(buffer));
#else
	printf("%s", buffer);
#endif

	request = sakeSearchForRecords(sake, input, SearchForRecordsCallback, NULL);
		if(!request)
		{
			startRequestResult = sakeGetStartRequestResult(sake);
		sprintf(results[gIndex].errString, "Failed to start request: %d", startRequestResult);
		results[gIndex].pass = gsi_false;
		}
		else
		{
			NumOperations++;
		}
	ProcessTasks();
	gIndex++; //increment global task counter
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
static void GetRandomRecord(SAKEGetRandomRecordInput * input)
{
		static SAKERequest request;
		SAKEStartRequestResult startRequestResult;

	printf("%d) Starting GetRandomRecord ================\n", gIndex+1);

	request = sakeGetRandomRecord(sake, input, GetRandomRecordCallback, NULL);
		if(!request)
		{
			startRequestResult = sakeGetStartRequestResult(sake);
		sprintf(results[gIndex].errString, "Failed to start request: %d", startRequestResult);
		results[gIndex].pass = gsi_false;
		}
		else
		{
			NumOperations++;
		}

	ProcessTasks();
	gIndex++; //increment global task counter
}
*/

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void RateRecord(char * table, int recordid, gsi_u8 rating)
{
		static SAKERateRecordInput input;
		static SAKERequest request;
		SAKEStartRequestResult startRequestResult;

	input.mTableId = table;
	input.mRecordId = recordid;
	input.mRating = rating;

	printf("%d) Starting RateRecord ================\n", gIndex+1);

		request = sakeRateRecord(sake, &input, RateRecordCallback, NULL);
		if(!request)
		{
			startRequestResult = sakeGetStartRequestResult(sake);
		sprintf(results[gIndex].errString, "Failed to start request: %d", startRequestResult);
		results[gIndex].pass = gsi_false;
		}
		else
		{
			NumOperations++;
		}

	ProcessTasks();
	gIndex++; //increment global task counter
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void GetRecordLimit(char * table)
{
		static SAKEGetRecordLimitInput input;
		static SAKERequest request;
		SAKEStartRequestResult startRequestResult;

	input.mTableId = table;

	printf("%d) Starting GetRecordLimit ================\n", gIndex+1);

		request = sakeGetRecordLimit(sake, &input, GetRecordLimitCallback, NULL);
		if(!request)
		{
			startRequestResult = sakeGetStartRequestResult(sake);
		sprintf(results[gIndex].errString, "Failed to start request: %d", startRequestResult);
		results[gIndex].pass = gsi_false;
		}
		else
		{
			NumOperations++;
		}

	ProcessTasks();
	gIndex++; //increment global task counter
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static int GetCount(char * table, gsi_char * filter, gsi_bool cache)
{
	static SAKEGetRecordCountInput input;
	SAKERequest request;
	SAKEStartRequestResult startRequestResult;
	int count = 0;

	input.mTableId = table;
	input.mFilter = filter;
	input.mCacheFlag = cache;

	printf("%d) Starting GetCount ================\n", gIndex+1);

	request = sakeGetRecordCount(sake, &input, GetRecordCountCallback, &count);
	if(!request)
	{
		startRequestResult = sakeGetStartRequestResult(sake);
		sprintf(results[gIndex].errString, "Failed to start request: %d", startRequestResult);
		results[gIndex].pass = gsi_false;
	}
	else
	{
		NumOperations++;
	}

	ProcessTasks();
	gIndex++; //increment global task counter

	return count;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// shouldn't need this now that we have upload/download content
/*static void UploadFile()
{
	GHTTPPost post;
	GHTTPRequest request;
	gsi_char url[SAKE_MAX_URL_LENGTH];
	
	char *data = gsimalloc(SAKE_UPLOAD_AMOUNT);
	
    memset(data, '\0',SAKE_UPLOAD_AMOUNT );
	memset(data, 'a', SAKE_UPLOAD_AMOUNT-1);

    printf("%d) Starting UploadFile ================\n", gIndex+1);

	// get an upload url
	if(sakeGetFileUploadURL(sake, url))
		_tprintf(_T("Upload URL: %s\n"), url);
	else
		printf("Failed to get upload url!\n");

	// upload a file
	post = ghttpNewPost();
	ghttpPostSetCallback(post, postCallback, NULL);
	ghttpPostAddFileFromMemory(post, _T("memory.file"), data, (int)strlen(data), _T("memory.file"), NULL);
	request = ghttpPostEx(url, NULL, post, GHTTPFalse, GHTTPTrue, NULL, UploadCompletedCallback, NULL);
		
	if(request == -1)
	{
		sprintf(results[gIndex].errString, "Error starting file upload");
		results[gIndex].pass = gsi_false;
	}
	else
	{
		NumOperations++;
	}

    while(NumOperations > 0)
    {
        printf("Thinking\n");
        msleep(5);
        ghttpThink();
    }
	gIndex++; //increment global task counter
    
    gsifree(data);
}*/

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void UploadContent(SAKEContentInfo *contentInfo, gsi_char *remoteFilename)
{
    //GHTTPRequest request;
    gsi_char url[SAKE_MAX_URL_LENGTH];
    SAKEStartRequestResult requestResult;    
    gsi_char        *uploadFilename = NULL;
    SAKEUploadContentInput input;
    
    printf("%d) Starting UploadFile ================\n", gIndex+1);

    // get an upload url
    if(sakeGetFileUploadURL(sake, url))
        _tprintf(_T("Upload URL: %s\n"), url);
    else
        printf("Failed to get upload url!\n");
    

    // Reset the global uploaded file id
    sakeTestImageData.gUploadedFileId = 0;
    if (remoteFilename == NULL)
    {
        // we must me uploading from disk then check the local file name
        input.remoteFileName = contentInfo->mStorage.mFile; 
    }
    else
    {
        input.remoteFileName = remoteFilename ;
    }
    input.content = contentInfo;
    input.transferBlocking = gsi_false;
    input.progressCallback = UploadContentProgressCallback;

    // Upload from disk  
    _tprintf(_T("Uploading content. \n\t\tRemote File Name : %s\n"), uploadFilename);
    requestResult = sakeUploadContent( sake, 
//                                 uploadFilename, 
//                                 contentInfo, 
//                                 gsi_false,
// 
//                                 UploadContentProgressCallback,
                                &input,
                                UploadContentCompletedCallback,  
                                NULL);
    if(requestResult != SAKEStartRequestResult_SUCCESS)
    {
        _tprintf(_T("Failed: Could not upload file. Check if file %s exists\n"), uploadFilename );
        sprintf(results[gIndex].errString, "Failed to start request: %d", requestResult);
        results[gIndex].pass = gsi_false;
    }
    else
    {
        NumOperations++;
    }

    ProcessTasks();

    gIndex++; //increment global task counter

}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/* To be deprecated
#ifndef NOFILE
static int DownloadFile(int fileid)
{
	gsi_char url[SAKE_MAX_URL_LENGTH];
	GHTTPRequest request;
	int numBytes = 0;

	if(gUploadResult)
	{
        printf("%d) Starting DownloadFile ================\n", gIndex+1);
        //sakeSetFileDownloadURL(sake, "https://207.38.11.183/SakeFileServer/download.aspx");
        // get a download url
		if(sakeGetFileDownloadURL(sake, fileid, url))
			_tprintf(_T("Download URL: %s\n"), url);
		else
			printf("Failed to get download url!\n");

		// download a file
	//	request = ghttpSave(url, _T("downloaded.file"), GHTTPTrue, DownloadCompletedCallback, &numBytes);
	    request = ghttpGet(url, GHTTPTrue, DownloadCompletedCallback, &numBytes);
		if(request == -1)
		{
			sprintf(results[gIndex].errString, "Error starting file download");
			results[gIndex].pass = gsi_false;
		}
		else
		{
			NumOperations++;
		}

		ProcessTasks();
		gIndex++; //increment global task counter
	}
	return numBytes;
}
#endif
*/
static int DownloadContent(SAKEContentInfo *content)
{
    gsi_char url[SAKE_MAX_URL_LENGTH];
    SAKEStartRequestResult requestResult;
    SAKEDownloadContentInput input;
    if(gUploadResult)
    {
        printf("%d) Starting DownloadContent ================\n", gIndex+1);

        // get a download url
        if(sakeGetFileDownloadURL(sake, content->mFileid, url))
            _tprintf(_T("Download URL: %s\n"), url);
        else
            printf("Failed to get download url!\n");

        //Download to memory
        // Initialize the file id to be downloaded
        content->mFileid = sakeTestImageData.gUploadedFileId;
        sakeTestImageData.gImageBuffer.mBuffer = NULL;
        sakeTestImageData.gImageBuffer.mLength = 0;

        input.content = content;
        input.transferBlocking = gsi_false;
        input.progressCallback = DownloadContentProgressCallback;

        printf("Download content to memory. \n\t\tFile Id : %d \n", sakeTestImageData.gUploadedFileId);
        requestResult = sakeDownloadContent(sake, 
                                    &input,
                                    DownloadContentCompletedCallback, 
                                    NULL);
        if(requestResult != SAKEStartRequestResult_SUCCESS)
        {
            printf("Failed: Could not download file. Check if file %d exists\n", sakeTestImageData.gUploadedFileId );
            sprintf(results[gIndex].errString, "Failed to start request: %d", requestResult);
            results[gIndex].pass = gsi_false;
        }
        else
        {
            NumOperations++;
        }
    }
    ProcessTasks();
    gIndex++; //increment global task counter
    return requestResult;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void RunFullTests()
{
	// FULL rundown of generic tests of all the functionality
    //static SAKESearchForRecordsInput    searchInput;
	static SAKECreateRecordInput		createInput;
	static SAKEReportRecordInput		reportInput;
	static SAKEUpdateRecordInput		updateInput;
	static SAKEGetMyRecordsInput		myInput;
	//static SAKEGetSpecificRecordsInput	specificInput;
	//static SAKEGetRandomRecordInput		randomInput;
	static SAKEField field;
 	int recordid = 0;
  	int cachedCount = 0;
    int count = 0;
 	int numOwned = 0;

	//int numOwnerIds = 0;
#ifndef GSI_UNICODE
	//char *utf8Filter;
#endif

 	char * table = "test";	// all of the full tests should be done on the test table for ease

#ifndef GSI_UNICODE
	//utf8Filter = UCS2ToUTF8StringAlloc((UCS2String)L"MyUnicodeString LIKE N'uni%'");
#endif
	//#1

	// *** report a record in the Content Moderation test table ***
	reportInput.mTableId = "CM_Test";
	reportInput.mReason = "ReportRecord testing..";
	reportInput.mRecordId = 1;
	reportInput.mReasonCode = 0;
	ReportRecord(&reportInput);

	cachedCount = GetCount(table, NULL, gsi_true); //get count before creation - cached
	count = GetCount(table, NULL, gsi_false); //get count before creation - cached
	GetRecordLimit(table); // Get record limit too (for num owned)
	numOwned = gNumOwned;
    if (cachedCount != count)
        ReportFailure("cached count didn't work");

	// *** create it ***
	createInput.mTableId = table;
	field.mName = "MyAsciiString";
	field.mType = SAKEFieldType_ASCII_STRING;
	field.mValue.mAsciiString = "this is a record";
	createInput.mFields = &field;
	createInput.mNumFields = 1;
	recordid = CreateRecord(&createInput);

	cachedCount = GetCount(table, NULL, gsi_true); //get count after creation - still cached

	GetRecordLimit(table); // Get record limit again

	// verify the new record was created
	if (numOwned == gNumOwned)
		ReportFailure("record not created");		


	// *** update and rate it ***
	{
		static char binaryData[] = "\x12\x34\x56\xAB\xCD";
#if defined(_PS2) || defined(_MACOSX) || defined(_LINUX) || defined(_IPHONE)
		static gsi_u16 unicodeString[] = {'u','n','i','c','o','d','e','\0'};
#else
		static gsi_u16 *unicodeString = L"unicode";
#endif
		static SAKEField fields[16];
		int index = 0;
		updateInput.mTableId = table;
		updateInput.mRecordId = recordid;
		fields[index].mName = "MyByte";
		fields[index].mType = SAKEFieldType_BYTE;
		fields[index].mValue.mByte = 123;
		index++;
		fields[index].mName = "MyShort";
		fields[index].mType = SAKEFieldType_SHORT;
		fields[index].mValue.mShort = 12345;
		index++;
		fields[index].mName = "MyInt";
		fields[index].mType = SAKEFieldType_INT;
		fields[index].mValue.mInt = 987654321;
		index++;
		fields[index].mName = "MyFloat";
		fields[index].mType = SAKEFieldType_FLOAT;
		fields[index].mValue.mFloat = 3.14159265f;
		index++;
		fields[index].mName = "MyAsciiString";
		fields[index].mType = SAKEFieldType_ASCII_STRING;
		fields[index].mValue.mAsciiString = "ascii";
		index++;
		fields[index].mName = "MyUnicodeString";
		fields[index].mType = SAKEFieldType_UNICODE_STRING;
		fields[index].mValue.mUnicodeString = unicodeString;
		index++;
		fields[index].mName = "MyBoolean";
		fields[index].mType = SAKEFieldType_BOOLEAN;
		fields[index].mValue.mBoolean = gsi_true;
		index++;
		fields[index].mName = "MyDateAndTime";
		fields[index].mType = SAKEFieldType_DATE_AND_TIME;
		fields[index].mValue.mDateAndTime = time(NULL);
		index++;
		fields[index].mName = "MyBinaryData";
		fields[index].mType = SAKEFieldType_BINARY_DATA;
		fields[index].mValue.mBinaryData.mValue = (gsi_u8*) binaryData;
		fields[index].mValue.mBinaryData.mLength = (int)strlen(binaryData);
		index++;
		updateInput.mFields = fields;
		updateInput.mNumFields = index;


		UpdateRecord(&updateInput);

	}

	RateRecord(table, recordid, 133);
	
	// *** retrieval ***
	{
		static char *fieldNames[] = { "recordid", "ownerid", "MyByte", "MyShort",
		                              "MyInt", "MyFloat", "MyAsciiString",
		                              "MyUnicodeString", "MyBoolean", "MyDateAndTime",
		                              "MyBinaryData", "MyFileID" };
		myInput.mTableId = table;
		myInput.mFieldNames = fieldNames;
		myInput.mNumFields = (sizeof(fieldNames) / sizeof(fieldNames[0]));

		GetMyRecords(&myInput);
	} 
    DeleteRecord("test",recordid );

    {
		SAKESearchForRecordsInput mySearch = {	NULL, 
                                                NULL, 
                                                0, 
                                                NULL, 
												NULL, 
                                                0, 
                                                0, 
												NULL, 
												0, 
												NULL, 
                                                0, 
												gsi_true}; 
        mySearch.mTableId   = table;
        mySearch.mNumFields = 1;
        mySearch.mFieldNames = (char **) gsimalloc(sizeof(char *)*mySearch.mNumFields );
        mySearch.mFieldNames[0] = "MyInt";
        mySearch.mFilter = _T("MyInt = 987654321");
        mySearch.mNumOwnerIds = 1;
        mySearch.mOwnerIds = gsimalloc(sizeof(int)*mySearch.mNumOwnerIds );
        mySearch.mOwnerIds[0] = profileid ;   
        mySearch.mMaxRecords = 1;
        SearchForRecords(&mySearch, NULL);
    }

    // *** file upload/download	***
	//UploadFile();
	
#ifndef NOFILE
	//if (DownloadFile(gUploadedFileId) != SAKE_UPLOAD_AMOUNT)
	//	ReportFailure("incorrect file size downloaded");
#endif

    {
            SAKEContentInfo content;
            char *data = gsimalloc(SAKE_UPLOAD_AMOUNT);

            memset(data, '\0',SAKE_UPLOAD_AMOUNT );
            memset(data, 'a', SAKE_UPLOAD_AMOUNT-1);

            content.mFileid = 0;
            content.mType   = SAKEContentStorageType_MEMORY;
            content.mStorage.mMemory = (SAKEContentBuffer *)gsimalloc(sizeof(SAKEContentBuffer));
            content.mStorage.mMemory->mBuffer = data;
            content.mStorage.mMemory->mLength = SAKE_UPLOAD_AMOUNT;

            UploadContent(&content, _T("myfile.txt"));

            // de-allocate the test data
            gsifree(content.mStorage.mMemory->mBuffer);
            gsifree(content.mStorage.mMemory);
    }

    if (sakeTestImageData.gUploadedFileId != 0) 
    {
			// for downloading to memory if NOFILE is defined
			char * downloadBuffer;

			// Download to either disk or memory
            SAKEContentInfo content2;
            content2.mFileid = sakeTestImageData.gUploadedFileId;
#ifndef NOFILE
            content2.mType   = SAKEContentStorageType_DISK;
			content2.mStorage.mFile = _T("./myfiledownload.txt");
			
			DownloadContent(&content2);

			GSI_UNUSED(downloadBuffer);
#else
			downloadBuffer = gsimalloc(SAKE_DOWNLOAD_AMOUNT);

			content2.mType = SAKEContentStorageType_MEMORY;
			content2.mStorage.mMemory = (SAKEContentBuffer *)gsimalloc(sizeof(SAKEContentBuffer));
			content2.mStorage.mMemory->mBuffer = downloadBuffer;
			content2.mStorage.mMemory->mLength = SAKE_DOWNLOAD_AMOUNT;

			DownloadContent(&content2);

			// de-allocate the test data
			gsifree(content2.mStorage.mMemory->mBuffer);
			gsifree(content2.mStorage.mMemory);
#endif
    } 
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if defined(_WIN32) && !defined(_XBOX) && defined(_DEBUG)
#include <crtdbg.h>
#endif

int test_main(int argc, char* argv[]);  // CW needs this prototyped

int test_main(int argc, char* argv[])
{
	SAKEStartupResult startupResult;

	GSI_UNUSED(argc);
	GSI_UNUSED(argv);

	// setup the common debugging
#ifdef GSI_COMMON_DEBUG
	gsSetDebugCallback(DebugCallback);
	gsSetDebugLevel(GSIDebugCat_All, GSIDebugType_All, GSIDebugLevel_Hardcore);
#endif

    // enable Win32 C Runtime debugging 
#if defined(_WIN32) && !defined(_XBOX) && defined(_DEBUG)
    {
        int tempFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
        _CrtSetDbgFlag(tempFlag | _CRTDBG_LEAK_CHECK_DF);
    }
#endif

	// availability check
	printf("Checking availability\n");
	if(CheckServices() != GSIACAvailable)
		return 0;

	// Init the GameSpy SDK Core (required by SOAP SDKs)
	printf("Initializing the GameSpy Core\n");
	gsCoreInitialize();

	// startup sake
	printf("Starting up Sake\n");
	startupResult = sakeStartup(&sake);
	if(startupResult != SAKEStartupResult_SUCCESS)
	{
		printf("Startup failed: %d\n", startupResult);
		return 0;
	}

     //sakeSetFileUploadURL(sake, "http://test.sake.gamespy.com/SakeFileServer/upload.aspx");
     //strcpy(sakeiSoapUrl, "http://test.sake.gamespy.com/SakeStorageServer/Public/StorageServer.asmx");
     //sakeSetFileDownloadURL(sake, "http://test.sake.gamespy.com/SakeFileServer/download.aspx");
	 //sakeSetFileUploadURL(sake, "http://localhost/SakeFileServer/upload.aspx");
	 //strcpy(sakeiSoapUrl, "http://localhost/SakeStorageServer/Public/StorageServer.asmx");
	 //sakeSetFileDownloadURL(sake, "http://localhost/SakeFileServer/download.aspx");
	// login to GP and use data to authenticate Sake
	LoginAndAuthenticate();

	// open files for logging
#if !defined(NOFILE)
	file = fopen("SakeTestResults.log", "wb");
#endif

	// run the tests
	RunFullTests();

	// keep thinking
	printf("Finalizing any incomplete tasks\n");
	ProcessTasks();

	// print out final results
	PrintResults();

    //getc(stdin);

	// close logging files
#if !defined(NOFILE)
	fclose(file);
#endif

	// shutdown sake
	printf("Shutting down Sake\n");
	sakeShutdown(sake);

	// cleanup any specific http requests
	ghttpCleanup();

	// shutdown the core
	printf("Shutting down the GameSpy Core\n");
	gsCoreShutdown();

    // Wait for core shutdown 
    //   (should be instantaneous unless you have multiple cores)
    while(gsCoreIsShutdown() == GSCore_SHUTDOWN_PENDING)
    {
        gsCoreThink(0);
        msleep(5);
    }
#if defined(_WIN32) && !defined(_XBOX) && defined(_DEBUG) && defined(USE_CRTDBG)
    _CrtMemDumpAllObjectsSince( NULL );
    if (! _CrtDumpMemoryLeaks())
	{
        printf("No Leaks\n");
	}
	else
	{
		printf("Possible Memory Leak");
		totalFailed++;
	}
#endif

#ifdef _NITRO
	Heap_Print();
#endif

	if(totalFailed != 0)
		return 1;

	return 0;
}
