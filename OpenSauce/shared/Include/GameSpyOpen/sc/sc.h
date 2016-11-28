///////////////////////////////////////////////////////////////////////////////
// File:	sc.h
// SDK:		GameSpy ATLAS Competition SDK
//
// Copyright (c) IGN Entertainment, Inc.  All rights reserved.  
// This software is made available only pursuant to certain license terms offered
// by IGN or its subsidiary GameSpy Industries, Inc.  Unlicensed use or use in a 
// manner not expressly authorized by IGN or GameSpy is prohibited.

#ifndef __SC_H__
#define __SC_H__


//Includes
///////////////////////////////////////////////////////////////////////////////
#include "../common/gsCommon.h"
#include "../common/gsRC4.h"
#include "../common/gsAvailable.h"
#include "../ghttp/ghttp.h"
#include "../webservices/AuthService.h"


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	// optional to explicitly use __stdcall, __cdecl, __fastcall
#define SC_CALL


	//Defines
	///////////////////////////////////////////////////////////////////////////////
	// Set this to define memory settings for the SDK
#define SC_STATIC_MEM

	// The initial (or fixed, for static memory) report buffer size
#define SC_REPORT_BUFFER_BYTES 65536

	// URL's for ATLAS services.
#define SC_SERVICE_MAX_URL_LEN 128
	extern char scServiceURL[SC_SERVICE_MAX_URL_LEN];
	extern char scGameConfigDataServiceURL[SC_SERVICE_MAX_URL_LEN];
	extern char scStatsDataServiceURL[SC_SERVICE_MAX_URL_LEN];

	// Session GUID size - must match backend
//#define SC_SESSION_GUID_SIZE 16
#define SC_AUTHDATA_SIZE 16
#define SC_SESSION_GUID_SIZE 40
#define SC_CONNECTION_GUID_SIZE 40

	// convert the 40 byte string guid into an int, 2 shorts and 8 bytes
#define SC_GUID_BINARY_SIZE 16 

	// Limit to the number of teams
#define SC_MAX_NUM_TEAMS	64

	// OPTIONS flags - first two bits reserved for authoritative / final flags
#define SC_OPTIONS_NONE					0

	// These length values come from the corresponding ATLAS DB column definitions.
#define SC_CATEGORY_MAX_LENGTH 64
#define SC_STAT_NAME_MAX_LENGTH 128



	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	// Result codes

	//////////////////////////////////////////////////////////////
	// SCResult
	// Summary
	//		SCResult is used to indicate error and failure conditions 
	//		in the ATLAS SDK.
	// Remarks
	//		Results of a call to an interface function or operation.  
	//		It can be used to see if the initial call to a function completed without 
	//		error.  The callback that is passed to interface functions will also have 
	//		a value that is of this type. The application can check this value for 
	//		failures.
	typedef enum
	{
		SCResult_NO_ERROR = 0,			// No error has occurred.
		SCResult_NO_AVAILABILITY_CHECK,	// The standard GameSpy Availability Check was not performed 
		// prior to initialization.
		SCResult_INVALID_PARAMETERS,	// Parameters passed to interface function were invalid.
		SCResult_NOT_INITIALIZED,		// The SDK was not initialized.
		SCResult_CORE_NOT_INITIALIZED,	// The core was initialized by the application.
		SCResult_OUT_OF_MEMORY,			// The SDK could not allocate memory for its resources.
		SCResult_CALLBACK_PENDING,		// Result tell the application, that the operation is still pending.

		SCResult_HTTP_ERROR,			// Error if the backend fails to respond with correct HTTP.
		SCResult_UNKNOWN_RESPONSE,		// Error if the SDK cannot understand the result.
		SCResult_RESPONSE_INVALID,		// Error if the SDK cannot read the response from the backend.
		SCResult_INVALID_DATATYPE,		// Error if an invalid datatype is received.

		SCResult_REPORT_INCOMPLETE,		// The report was incomplete.
		SCResult_REPORT_INVALID,		// Part or all of report is invalid.
		SCResult_SUBMISSION_FAILED,		// Submission of report failed.

		SCResult_QUERY_DISABLED,		// Error occurs if the query id is disabled on the backend administration site
		SCResult_QUERY_TYPE_MISMATCH,	// Error occurs if the query id passed is used for the wrong query type
		// e.g. query id is passed into a game stats query instead of player stats query
		SCResult_QUERY_INVALID,			// Error occurs if the query id is invalid or not found.  The text message will provide details.
		SCResult_QUERY_PARAMS_MISSING,	// Error occurs if a parameter or parameters for the specified query are missing.

		SCResult_QUERY_PARAMS_TOO_MANY,	// Error occurs if the number of params exceeds the expected number
		SCResult_QUERY_PARAM_TYPE_INVALID,	// Error occurs if a parameter value that is passed does not match what the query expects
		// e.g. if the query expects a number string and a non-numeric string is passed in.

		SCResult_UNKNOWN_ERROR,			// Error unknown to SDK

		// response codes dealing with errors in response headers
		SCResult_INVALID_GAMEID,			// make sure GameID is properly set with wsSetCredentials
		SCResult_INVALID_SESSIONTOKEN,     // make sure wsSetCredentials was called with valid credentials and you have logged in via AuthService
		SCResult_SESSIONTOKEN_EXPIRED,      // re-login via AuthService to refresh your 'session'

		SCResultMax						// Total number of result codes that can be returned.
	} SCResult;


	//////////////////////////////////////////////////////////////
	// SCGameResult
	// Summary
	//		Used when submitting a report for a game session to indicate
	//		the result of the game session for the current player or team.
	// Remarks
	//		Can be used for both player and a team.
	typedef enum
	{
		SCGameResult_WIN,			// The game session resulted in a win for the current player or team.
		SCGameResult_LOSS,			// The game session resulted in a loss for the current player or team.
		SCGameResult_DRAW,			// The game session resulted in a draw for the current player or team.
		SCGameResult_DISCONNECT,	// The current player or team disconnected during the game session.
		SCGameResult_DESYNC,		// The current player or team lost sync during the game session.
		SCGameResult_NONE,			// There was no result from the game session for the current player or team.
		SCGameResultMax				// The upper bound of game result codes.
	} SCGameResult;


	//////////////////////////////////////////////////////////////
	// SCGameStatus
	// Summary
	//		Indicates how the game session ended and is declared when ending a 
	//		report.
	// Remarks
	//		For <strong>SCGameStatus</strong> reporting, the game should do the following:
	//		<p />
	//		As long as the game finished properly, and no one disconnected during the 
	//		course of play, then all players in the game session should submit 
	//		<strong>SCGameStatus_COMPLETE</strong> reports. If any members disconnected 
	//		during play, but the game was finished completely, then all players in the 
	//		session should submit <strong>SCGameStatus_PARTIAL</strong> reports indicating 
	//		that disconnects occured. For any players who do not complete the session, a 
	//		<strong>SCGameStatus_BROKEN</strong> report should be submitted. 
	//		<p />
	//		Thus, if the game did not completely finish, all players will submit broken 
	//		reports. The only case that will trigger an invalid report is if reports for 
	//		the same game describe status as both <strong>SCGameStatus_COMPLETE</strong> 
	//		and <strong>SCGameStatus_PARTIAL</strong>. 
	//		<p />
	//		Since COMPLETE indicates that all players finished the game without a disconnect 
	//		and PARTIAL indicates that disconnects occured, at no time should a game 
	//		report both COMPLETE and PARTIAL -- this will be seen as an exploit and 
	//		invalidate the report.
	// Members/Constants
	//		SCGameStatus_COMPLETE	
	//		SCGameStatus_PARTIAL	
	//		SCGameStatus_BROKEN	
	//		SCGameStatusMax	
	typedef enum
	{
		SCGameStatus_COMPLETE,	// The game session came to the expected end without 
		// interruption (disconnects, desyncs).
		// This status indicates that game results are available 
		// for all players.
		SCGameStatus_PARTIAL,	// Although the game session came to the expected end, one 
		// or more players unexpectedly quit or were disconnected. 
		// Game results should explicitly report which players were 
		// disconnected to be used during normalization for possible 
		// penalty metrics.
		SCGameStatus_BROKEN,	// The game session did not reach the expected end point and 
		// is incomplete. This should be reported when there has been 
		// an event detected that makes the end result indeterminate.
		SCGameStatusMax			// The upper bound of game status codes.
	} SCGameStatus;

	//////////////////////////////////////////////////////////////
	// SCDataType
	// Summary
	//		The datatypes supported by ATLAS.
	typedef enum SCStatDataType
	{
		SCDataType_INT32,
		SCDataType_INT16,
		SCDataType_BYTE,
		SCDataType_STRING,
		SCDataType_FLOAT,
		SCDataType_INT64,
		SCDataTypeMax
	} SCDataType;

	//////////////////////////////////////////////////////////////
	// SCPlatform
	// Summary
	//		The platforms supported by the SDK.
	typedef enum SCPlatform
	{
		SCPlatform_Unknown,
		SCPlatform_PC,
		SCPlatform_PS3,
		SCPlatform_PSP,
		SCPlatform_XBox360,
		SCPlatform_DS,
		SCPlatform_Wii,
		SCPlatform_iPhone,
		SCPlatform_Unix,

		SCPlatformMax
	} SCPlatform;

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	// Data types

	//////////////////////////////////////////////////////////////
	// SCInterfacePtr
	// Summary
	//		Pointer to the instance of a game session.
	// Remarks
	//		This pointer needs to be referenced by almost all API functions.
	//      It is created with the scInitialize() function.<p>
	// See Also
	//		scInitialize
	typedef void*   SCInterfacePtr;

	//////////////////////////////////////////////////////////////
	// SCReportPtr
	// Summary
	//		Pointer to the internal reporting object for a game session.
	// Remarks
	//		This pointer needs to be referenced by all reporting functions.
	//      It is created with the scCreateReport() function.<p>
	// See Also
	//		scCreateReport
	typedef void*   SCReportPtr;
	typedef char    SCHiddenData[64];

	//////////////////////////////////////////////////////////////
	// SCQueryParameterListPtr
	// Summary
	//		User facing object used for query functions.
	// Remarks
	//		Stores the parameters so that the query functions can
	//		serialize them. This must be created and filled before passing
	//		it to a query function.  The user is responsible for destroying
	//		this object. <p>
	// See Also
	//		scCreateQueryParameterList, scAddQueryParameterToList, scDestroyQueryParameterList
	typedef struct SCQueryParameterList *SCQueryParameterListPtr;

	//////////////////////////////////////////////////////////////
	// SCStat
	// Summary
	//		User facing object that contains a stat name, value, and type.
	//		The name and type of the stat correlates to that on the backend administration 
	//		site. The type will be one of the values defined in the SCStatDataType.<p>
	// See Also
	//		SCGameStatsCategory, SCPlayer, SCTeam
	typedef struct SCStat
	{
		gsi_char *	mName;		// Name for this stat 
		SCDataType	mStatType;	// Type for this stat
		gsi_char *	mValue;		// Value for this stat
	} SCStat;

	//////////////////////////////////////////////////////////////
	// SCPlayer
	// Summary
	//		User facing object that contains player data.  Each player
	//		will have a profile id and some stats. <p>
	// See Also
	//		SCPlayerStatsCategory, SCStat
	typedef struct SCPlayer
	{
		gsi_u32 mProfileId;		// Profile ID associated with the current player
		gsi_u32 mStatsCount;	// Stats count for mStats below
		SCStat *mStats;			// Array of SCStat objects
	} SCPlayer;

	//////////////////////////////////////////////////////////////
	// SCTeam
	// Summary
	//		User facing object that contains team data.  Each team
	//		will have a team id and some stats. <p>
	// See Also
	//		SCTeamStatsCategory, SCStat
	typedef struct SCTeam
	{
		gsi_u32 mTeamId;		// Team ID associated with the current player
		gsi_u32 mStatsCount;	// Stats count for mStats below
		SCStat *mStats;			// Array of SCStat objects
	} SCTeam;

	//////////////////////////////////////////////////////////////
	// SCGameStatCategory
	// Summary
	//		Category object that contains a name and global game stats.
	//		These objects will be a part of a SCGameStatsQueryResponse object 
	//		which will be created by the SDK and passed to the user defined callback.<p>
	// See Also
	//		SCGameStatsQueryResponse, SCStat
	typedef struct SCGameStatsCategory
	{
		gsi_char *mName;		// Name of the category
		gsi_u32 mStatsCount;	// Player count for mPlayers below
		SCStat *mStats;		// Array of SCPlayer objects
	} SCGameStatsCategory;

	//////////////////////////////////////////////////////////////
	// SCPlayerStatsCategory
	// Summary
	//		Category object that contains a name and a set of players.
	//		These objects will be a part of a SCPlayerStatsQueryResponse object 
	//		which will be created by the SDK and passed to the user defined 
	//		callback.<p>
	// See Also
	//		SCPlayerStatsQueryResponse, SCPlayer
	typedef struct SCPlayerStatsCategory
	{
		gsi_char *mName;		// Name of the category
		gsi_u32 mPlayersCount;	// Player count for mPlayers below
		SCPlayer *mPlayers;		// Array of SCPlayer objects
	} SCPlayerStatsCategory;

	//////////////////////////////////////////////////////////////
	// SCTeamStatsCategory
	// Summary
	//		Category object that contains a name and a set of teams.
	//		These objects will be a part of a SCTeamStatsQueryResponse object which 
	//		will be created by the SDK and passed to the user defined callback.<p>
	// See Also
	//		SCTeamStatsQueryResponse, SCTeam
	typedef struct SCTeamStatsCategory
	{
		gsi_char *mName;		// Name of the category
		gsi_u32 mTeamsCount;	// Player count for mPlayers below
		SCTeam *mTeams;		// Array of SCPlayer objects
	} SCTeamStatsCategory;

	//////////////////////////////////////////////////////////////
	// SCGameStatsQueryResponse
	// Summary
	//		Response received from backend for game stats query.  This object
	//		is comprised of stats for the requested category or categories.
	// See Also
	//		SCPlayerStatQueryCallback, SCGameStatCategory
	typedef struct SCGameStatsQueryResponse
	{
		gsi_u32 mCategoriesCount;
		SCGameStatsCategory *mCategories;
	} SCGameStatsQueryResponse;

	//////////////////////////////////////////////////////////////
	// SCPlayerStatsQueryResponse
	// Summary
	//		Response received from backend for player stats query.  This object
	//		is comprised of stats for the requested category or categories for each player in the return set.
	// See Also
	//		SCPlayerStatQueryCallback, SCPlayerStatsCategory
	typedef struct SCPlayerStatsQueryResponse
	{
		gsi_u32 mCategoriesCount;
		SCPlayerStatsCategory *mCategories;
	} SCPlayerStatsQueryResponse;

	//////////////////////////////////////////////////////////////
	// SCTeamStatsQueryResponse
	// Summary
	//		Response received from backend for team stats query.  This object
	//		is comprised of stats for the requested category or categories for each team in the return set.
	// See Also
	//		SCTeamStatQueryCallback, SCTeamStatsCategory
	typedef struct SCTeamStatsQueryResponse
	{
		gsi_u32 mCategoriesCount;
		SCTeamStatsCategory *mCategories;
	} SCTeamStatsQueryResponse;

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	// Callbacks

	//////////////////////////////////////////////////////////////
	// SCCheckBanListCallback
	// Summary
	//		Called when scCheckBanList has completed.
	// Parameters
	//		theInterface	: [in] the pointer to the SC Interface object.  The game usually has copy of this also.
	//		httpResult  	: [in] Http result from creating a session
	//		result 		: [in] SC Result telling the application what happened when checking the game's ban list
	//		theUserData		: [in] constant pointer to user data
	//		resultProfileId		: [in] The profileid that was checked 
	//		resultPlatformId	: [in] The platformid that was checked
	//		resultProfileBannedHost: [in] The ban status
	// Remarks
	//		Only applicable if whitelisting is enabled for your title
	// See Also
	//		
	typedef void (*SCCheckBanListCallback)(const SCInterfacePtr theInterface,
		GHTTPResult          httpResult,
		SCResult             result,
		void *               userData,
		int					 resultProfileId,
		int					 resultPlatformId,
		gsi_bool             resultProfileBannedHost);

	//////////////////////////////////////////////////////////////
	// SCCreateSessionCallback
	// Summary
	//		Called when scCreateSession has completed.
	// Parameters
	//		theInterface	: [in] A pointer to the ATLAS SDK object.  
	//								The game usually has copy of this also.
	//		httpResult	: [in] The HTTP result of the most recent HTTP transaction
	//		result		: [in] The SDK result (SCResult) of this request
	//		userData		: [in] A constant pointer to user-provided data
	// Remarks
	//		Called when a game session is created. The results will determine if 
	//		the game session was successfully created. If there were any errors, 
	//		result will be set to the specific error code. Otherwise, result 
	//		will be set to SCResult_NO_ERROR. See SCResult for error codes.
	// See Also
	//		scCreateSession, SCResult
	typedef void (*SCCreateSessionCallback)(const SCInterfacePtr theInterface,
		GHTTPResult          httpResult,
		SCResult             result,
		void *               userData);

	//////////////////////////////////////////////////////////////
	// SCSetReportIntentionCallback
	// Summary
	//		Called when scReportIntention has completed.
	// Parameters
	//		theInterface	: [in] A pointer to the ATLAS SDK object. 
	//								The game usually has copy of this also.
	//		httpResult	: [in] The HTTP result of the most recent HTTP transaction
	//		result		: [in] The SDK result (SCResult) of this request
	//		userData		: [in] A constant pointer to user-provided data
	// Remarks
	//		Called when a host or client reporting its intention is complete.
	//		The results will determine if the game session was successfully created.
	//		If there were any errors, result will be set to the specific error 
	//		code. Otherwise, result will be set to SCResult_NO_ERROR.
	//		<p />
	//		See SCResult for error codes.
	// See Also
	//		scSetReportIntention, SCResult
	typedef void (*SCSetReportIntentionCallback)(const SCInterfacePtr theInterface,
		GHTTPResult          httpResult,
		SCResult             result,
		void *	              userData);

	//////////////////////////////////////////////////////////////
	// SCSubmitReportCallback
	// Summary
	//		Called when scSubmitReport completes.
	// Parameters
	//		theInterface	: [in] A pointer to the ATLAS SDK object. 
	//								The game usually has copy of this also.
	//		httpResult	: [in] The HTTP result of the most recent HTTP 
	//								transaction
	//		result		: [in] The SDK result (SCResult) of this request
	//		userData		: [in] A constant pointer to user-provided data
	// Remarks
	//		After the SDK submits the report, the backend will send back results 
	//		that will be available in this callback. If there were any errors, 
	//		result will be set to the specific error code. Otherwise, 
	//		result will be set to SCResult_NO_ERROR.
	//		<p />
	//		See SCResult for error codes.
	// See Also
	//		scSubmitReport, SCResult
	typedef void (*SCSubmitReportCallback)(const SCInterfacePtr theInterface,
		GHTTPResult          httpResult,
		SCResult             result,
		void *               userData);

	/*
	typedef void (*SCKeyCallback) (SCReportPtr theReport,
	SCReportKeyType theKeyType,
	gsi_u32 theIndex, // how many times this has been called
	void* theUserParam);
	*/

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	// Main interface functions
	//////////////////////////////////////////////////////////////
	// scInitialize
	// Summary
	//		Initializes the ATLAS SDK, providing a valid ATLAS SDK object.
	// Parameters
	//		gameId		: [in] The GameSpy Game Id issued to identify a game.
	//		theInterfaceOut	: [out] A pointer to a new ATLAS SDK object.
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		The function must be called in order to get a valid ATLAS SDK (SCInterface)
	//		object. Most other interface functions require an initialized interface object
	//		when being called. Note that if the standard GameSpy Availability Check was 
	//		not performed prior to this call, the SDK will return 
	//		SCResult_NO_AVAILABILITY_CHECK.
	// See Also
	//		scShutdown
	SCResult SC_CALL scInitialize(int gameId,
		SCInterfacePtr * theInterfaceOut);

	//////////////////////////////////////////////////////////////
	// scShutdown
	// Summary
	//		Closes down the specified ATLAS SDK object and frees memory.
	// Parameters
	//		theInterface	: [in] A valid ATLAS SDK object
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		In order to clean up all resources used by the SDK, this interface function 
	//		must be called once operations with this SDK are complete. Do not call this 
	//		function if you plan to continue reporting stats.
	// See Also
	//		scInitialize
	SCResult SC_CALL scShutdown  (SCInterfacePtr theInterface);

	//////////////////////////////////////////////////////////////
	// scThink
	// Summary
	//		Called to complete pending operations for functions with callbacks.
	// Parameters
	//		theInterface	: [in] A valid ATLAS SDK object
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		This function should be called with a valid interface object.
	//		It will take care of pending requests that have been made by the interface 
	//		functions.
	// See Also
	//		scInitialize, scCreateSession, scSetReportIntention, scSubmitReport
	SCResult SC_CALL scThink     (SCInterfacePtr theInterface);


	//////////////////////////////////////////////////////////////
	// scCheckBanList
	// Summary
	//		Checks if a given gsid account is whitelisted or not.
	// Parameters
	//		theInterface	: [in] A valid ATLAS SDK object
	//		certificate		: [in] A valid certificate obtained from the GameSpy 
	//								AuthService.
	//		privateData		: [in] Valid private data obtained from the GameSpy 
	//								AuthService.
	//		hostProfileId	: [in] profileid of the host
	//		hostPlatform	: [in] platform of the host
	//		callback		: [in] The callback function to call when this request 
	//								completes.
	//		timeoutMs		: [in] The amount of time in milliseconds to wait for this 
	//								operation to complete before timing out.
	//		userData		: [in] User data for use in callbacks.  Note that it is
	//							a constant pointer in the callback
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		Only applicable if whitelisting is enabled for your title
	// See Also
	//
	SCResult SC_CALL scCheckBanList(SCInterfacePtr				theInterface,
		const GSLoginCertificate *	certificate,
		const GSLoginPrivateData *	privateData,
		gsi_u32                     hostProfileId,
		SCPlatform                  hostPlatform,
		SCCheckBanListCallback		callback,
		gsi_time					timeoutMs,
		void *						userData);

	//////////////////////////////////////////////////////////////
	// scCreateSession
	// Summary
	//		Requests that the ATLAS service create a new game session and 
	// Parameters
	//		theInterface	: [in] A valid ATLAS SDK object
	//		certificate	: [in] A valid certificate obtained from the GameSpy 
	//								AuthService.
	//		privateData	: [in] Valid private data obtained from the GameSpy 
	//								AuthService.
	//		callback		: [in] The callback function to call when this request 
	//								completes.
	//		timeoutMs	: [in] The amount of time in milliseconds to wait for this 
	//								operation to complete before timing out.
	//		userData		: [in] User data for use in callbacks.  Note that it is
	//								a constant pointer in the callback
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		The certificate and private data may be NULL if the local client
	//		is an unauthenticated dedicated server.
	//		The function should be called by the host after initializing the SDK, 
	//		and obtaining a certificate and private data from the authentication service.
	//		The competition service creates and sends a session ID to the host.
	//		The callback passed in will get called even if the request failed.
	// See Also
	//		SCCreateSessionCallback, scInitialize, scThink
	SCResult SC_CALL scCreateSession(SCInterfacePtr             theInterface,
		const GSLoginCertificate * certificate,
		const GSLoginPrivateData * privateData,
		SCCreateSessionCallback    callback,
		gsi_time                   timeoutMs,
		void *                     userData);

	//////////////////////////////////////////////////////////////
	// scCreateMatchlessSession
	// Summary
	//		This is a variation of scCreateSession that creates a "matchless" session.
	//		A matchless session isn't sanity checked against other session data for
	//		arbitration of any statistics.  It essentially turns of detection for cheating.
	//		As data is sent in it will be immediately applied to the stats, instead of being
	//		applied when the match has finished.
	//		A "matchless" game may be more commonly known as non-arbitrated or unranked.
	// Parameters
	//		theInterface	: [in] A valid ATLAS SDK object
	//		certificate	: [in] Certificate obtained from the GameSpy AuthService.
	//		privateData	: [in] Private Data obtained from the GameSpy AuthService.
	//		callback		: [in] The callback function to call when this request completes.
	//		timeoutMs	: [in] The amount of time in milliseconds to wait for this 
	//								operation to complete before timing out.
	//		userData		: [in] User data for use in callbacks.  Note that it is 
	//								a constant pointer in the callback
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		Reports sent for matchless sessions should be 
	//		marked as such using "scReportSetAsMatchless".
	// See Also
	//		scReportSetAsMatchless, SCCreateSessionCallback, scInitialize, scThink
	SCResult SC_CALL scCreateMatchlessSession(SCInterfacePtr    theInterface,
		const GSLoginCertificate * certificate,
		const GSLoginPrivateData * privateData,
		SCCreateSessionCallback    callback,
		gsi_time                   timeoutMs,
		void *                     userData);

	//////////////////////////////////////////////////////////////
	// scSetReportIntention
	// Summary
	//		Called to tell ATLAS the type of report that the player or 
	//		host will send.
	// Parameters
	//		theInterface	: [ref] A valid ATLAS SDK object.
	//		theConnectionId	: [in]  The player's connection id. Set to NULL unless the player is
	//								rejoining a session he previously left.
	//		isAuthoritative	: [in]  flag set if the snapshot being reported will be an authoritative.
	//		certificate		: [ref] Certificate obtained from the authentication web service.
	//		privateData		: [ref] Private data obtained from the authentication web service.
	//		callback		: [ref] The callback function to call when this request completes.
	//		timeoutMs		: [in]  The amount of time in milliseconds to wait for this 
	//								operation to complete before timing out.
	//		userData		: [ref] Application data that may be used in the callback.
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		The should be called by both the host and client before sending a report.
	//		<p />
	//		The host should have created a session before calling this. It allows the 
	//		server to know ahead of time what type of report will be sent. Reports 
	//		submitted without an intention will be discarded.
	//		<p />
	//		Note: The theConnectionId argument should be set to NULL unless the player
	//		is rejoining a session he previously left.
	// See Also
	//		scCreateSession, SCSetReportIntentionCallback, scSubmitReport
	SCResult SC_CALL scSetReportIntention(const SCInterfacePtr         theInterface,
		const gsi_u8                 theConnectionId[SC_CONNECTION_GUID_SIZE],
		gsi_bool                     isAuthoritative,
		const GSLoginCertificate *   certificate,
		const GSLoginPrivateData *   privateData,
		SCSetReportIntentionCallback callback,
		gsi_time                     timeoutMs,
		void *					   userData);

	//////////////////////////////////////////////////////////////
	// scSubmitReport
	// Summary
	//		Initiates the submission of a report.
	// Parameters
	//		theInterface	: [in] A valid ATLAS SDK object.
	//		theReport		: [in] A valid SC Report object
	//		isAuthoritative	: [in] Flag to tell if the snapshot is authoritative
	//		certificate	: [in] Certificate obtained from the GameSpy AuthService.
	//		privateData	: [in] Private Data obtained from the GameSpy AuthService.
	//		callback		: [in] The callback function to call when this request completes.
	//		timeoutMs	: [in] The amount of time in milliseconds to wait for this 
	//								operation to complete before timing out.
	//		userData		: [in] Application data that may be used in the callback
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		Once the report has been completed with a call to scReportEnd, 
	//		the player or host can call this function to submit a report.
	//		The certificate and private data are both required to submit a report.
	//		Incomplete reports will be discarded.
	//		The callback passed in will tell the game the result of the operation.
	// See Also
	//		scInitialize, scCreateSession, scSetReportIntention, 
	//		scReportEnd, SCSubmitReportCallback, scThink
	SCResult SC_CALL scSubmitReport      (const SCInterfacePtr       theInterface,
		const SCReportPtr          theReport,
		gsi_bool                   isAuthoritative,
		const GSLoginCertificate * certificate,
		const GSLoginPrivateData * privateData,
		SCSubmitReportCallback     callback,
		gsi_time                   timeoutMs,
		void *                     userData);

	//////////////////////////////////////////////////////////////
	// scSetSessionId
	// Summary
	//		Used to set the session ID for the current game session.
	// Parameters
	//		theInterface	: [in] A valid ATLAS SDK object
	//		theSessionId	: [in] The session GUID of length SC_SESSION_GUID_SIZE
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		The session ID identifies a single game session between players. Players 
	//		should use the scGetSessionId function in order to obtain the session ID. 
	//		This should not be called if a session has not yet been created.
	// See Also
	//		scGetSessionId, scCreateSession
	SCResult SC_CALL scSetSessionId(const SCInterfacePtr theInterface, const gsi_u8 theSessionId[SC_SESSION_GUID_SIZE]);

	//////////////////////////////////////////////////////////////
	// scGetSessionId
	// Summary
	//		Gets the unique identifier (GUID) for the current game session.
	// Parameters
	//		theInterface	: [in] A valid ATLAS SDK object
	// Returns
	//		The session GUID for the current game session.
	// Remarks
	//		The session GUID identifies a single game session between players. 
	//		After the host creates a session, the host calls this function to 
	//		obtain the session GUID. The host then sends the session GUID to 
	//		all other participants in the game session.
	// See Also
	//		scSetSessionId, scCreateSession
	const char * scGetSessionId   (const SCInterfacePtr theInterface);

	//////////////////////////////////////////////////////////////
	// scGetConnectionId
	// Summary
	//		Used to obtain the unique connection ID for the current game session.
	// Parameters
	//		theInterface	: [in] A valid ATLAS SDK object
	// Returns
	//		The connection GUID for the current game session.
	// Remarks
	//		The connection id identifies a single player in a game session.
	//		It may be possible to have different connection ids during
	//		the same session since players can come and leave sessions.
	const char * scGetConnectionId(const SCInterfacePtr theInterface);


	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	// Report generation functions

	//////////////////////////////////////////////////////////////
	// scCreateReport
	// Summary
	//		Creates a new ATLAS report for the game session.
	// Parameters
	//		theInterface		: [in] A valid ATLAS SDK object
	//		theRulesetVersion	: [in] Ruleset version of the report as specified on the admin site
	//		thePlayerCount		: [in] Player count for allocating enough resources 
	//									and verification purposes
	//		theTeamCount		: [in] Team count for allocating enough resources and 
	//									verification purposes
	//		theReportOut		: [out] The pointer to the new SCReport object
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		You must call CreateSession and SetReportIntention at some point before 
	//		calling this function. This function should be called after a game 
	//		session has ended. The player count and team count are more accurate at 
	//		that point for dedicated server games. This function should also be called 
	//		before calling any scReport* function. The header version can be obtained 
	//		from the ATLAS administration web site where the the keys are created.
	//		<p />
	// See Also
	//		scCreateSession, scSetReportIntention,
	//		scReportBeginGlobalData, scReportBeginPlayerData, scReportBeginTeamData, 
	//		scReportBeginNewPlayer, scReportSetPlayerData, scReportBeginNewTeam, 
	//		scReportSetTeamData, scReportAddIntValue, scReportAddStringValue
	SCResult SC_CALL scCreateReport(const SCInterfacePtr theInterface, 
		gsi_u32 theRulesetVersion, 
		gsi_u32 thePlayerCount, 
		gsi_u32 theTeamCount, 
		SCReportPtr * theReportOut);

	//    - Write global data key/values
	//////////////////////////////////////////////////////////////
	// scReportBeginGlobalData
	// Summary
	//		Tells the ATLAS SDK to start writing global data to the report.
	// Parameters
	//		theReportData	: [ref] A valid SC Report Object
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		After creating a report, this function should be called prior to writing 
	//		global game data. Keys and values are reported using the individual 
	//		key/value functions.
	//		<p />
	//		ATLAS reports must include Global data first using scReportBeginGlobalData(), 
	//		then Player data using scReportBeginPlayerData(), and finally Team data 
	//		using scReportBeginTeamData(). All three must be included in this order, 
	//		even if there is no data to report for one or more of these sections.
	// See Also
	//		scCreateReport, scReportAddIntValue, scReportAddStringValue
	SCResult SC_CALL scReportBeginGlobalData(SCReportPtr theReportData);

	//////////////////////////////////////////////////////////////
	// scReportBeginPlayerData
	// Summary
	//		Tells the ATLAS SDK to start writing player data to the report.
	// Parameters
	//		theReportData	: [in] A valid SC Report Object
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		Use this function to mark the start of player data. The game can start 
	//		adding each player and its specific data after this is called.
	//		<p />
	//		ATLAS reports must include Global data first using scReportBeginGlobalData(), 
	//		then Player data using scReportBeginPlayerData(), and finally Team data 
	//		using scReportBeginTeamData(). All three must be included in this order, 
	//		even if there is no data to report for one or more of these sections.
	// See Also
	//		scCreateReport, scReportBeginNewPlayer, scReportSetPlayerData, 
	//		scReportAddIntValue, scReportAddStringValue
	SCResult SC_CALL scReportBeginPlayerData(SCReportPtr theReportData);

	//////////////////////////////////////////////////////////////
	// scReportBeginTeamData
	// Summary
	//		Tells the ATLAS SDK to start writing player data to the report.
	// Parameters
	//		theReportData	: [in] A valid SC Report Object
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		Use this function to mark the start of team data. The game can start 
	//		adding each team and its specific data after this is called.
	//		<p />
	//		Team data must come after global data and before player data.
	//		<p />
	//		ATLAS reports must include Global data first using scReportBeginGlobalData(), 
	//		then Player data using scReportBeginPlayerData(), and finally Team data 
	//		using scReportBeginTeamData(). All three must be included in this order, even 
	//		if there is no data to report for one or more of these sections.
	// See Also
	//		scCreateReport, scReportBeginGlobalData, scReportBeginPlayerData
	//		scReportBeginNewTeam, scReportSetTeamData, scReportAddIntValue, 
	//		scReportAddStringValue		
	SCResult SC_CALL scReportBeginTeamData  (SCReportPtr theReportData);


	//    - Write player auth info and key/values
	//////////////////////////////////////////////////////////////
	// scReportBeginNewPlayer
	// Summary
	//		Add a new player to the report.
	// Parameters
	//		theReportData	: [in] A valid SC Report Object
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		This function is adds a new player data to the report.
	// See Also
	//		scCreateReport, scReportBeginPlayerData, scReportSetPlayerData, 
	//		scReportAddIntValue, scReportAddStringValue
	SCResult SC_CALL scReportBeginNewPlayer(SCReportPtr  theReportData);

	//////////////////////////////////////////////////////////////
	// scReportSetPlayerData
	// Summary
	//		Sets initial player data in the report specified.
	// Parameters
	//		theReport				: [ref] A valid SC Report Object
	//		thePlayerIndex			: [in] Index of the player (0 - Number of players)
	//		thePlayerConnectionId	: [in] Connection GUID generated by the ATLAS service 
	//										and retrieved using scGetConnectionId().
	//		thePlayerTeamIndex		: [in] Team index of the player, if that player is on 
	//										a team.
	//		result				: [in] Standard SC Game result
	//		theProfileId			: [in] Profile ID of the player
	//		certificate			: [in] Certificate obtained from the GameSpy AuthService. 
	//										Note: This parameter is unused currently.
	//		theAuthData				: [in] Authentication data
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		A report must have been created prior to using this function.
	//		Each player must have a valid login certificate from the authentication 
	//		service also. This function should be called after a new player is added 
	//		to the report. Any key/value pairs that need to be added should be done 
	//		after calling this function.
	// See Also
	//		scCreateReport, scReportBeginPlayerData, scReportBeginNewPlayer,
	//		scReportAddIntValue, scReportAddStringValue
	SCResult SC_CALL scReportSetPlayerData (SCReportPtr  theReport,
		gsi_u32            thePlayerIndex,
		const gsi_u8       thePlayerConnectionId[SC_CONNECTION_GUID_SIZE],
		gsi_u32            thePlayerTeamId,
		SCGameResult       result,
		gsi_u32            theProfileId,
		const GSLoginCertificate * certificate,
		const gsi_u8       theAuthData[16]);

	//     - Write team info and key/values
	//////////////////////////////////////////////////////////////
	// scReportBeginNewTeam
	// Summary
	//		Adds a new team to the report.
	// Parameters
	//		theReportData	: [in] A valid SC Report Object
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		After the beginning of any team data is set, this function can be called 
	//		to start a new team. After this function has been called, the game can 
	//		start adding team data to the report.
	// See Also
	//		scCreateReport, scReportBeginTeamData, scReportSetPlayerData, 
	//		scReportAddIntValue, scReportAddStringValue
	SCResult SC_CALL scReportBeginNewTeam(SCReportPtr theReportData);

	//////////////////////////////////////////////////////////////
	// scReportSetTeamData
	// Summary
	//		Sets the initial team data in the report specified.
	// Parameters
	//		theReport		: [in] A valid SC Report Object
	//		theTeamIndex	: [in] The index of the team being reported
	//		result		: [in] The team's result (e.g. win, loss, draw)
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		A report must have been created prior to using this function.
	//		This function should be called after a new team is added to the report.
	//		Any key/value pairs that need to be added should be done after calling 
	//		this function.
	// See Also
	//		scCreateReport, scReportBeginTeamData, scReportBeginNewTeam, 
	//		scReportAddIntValue, scReportAddStringValue
	SCResult SC_CALL scReportSetTeamData (SCReportPtr theReport,
		gsi_u32           theTeamId,
		SCGameResult      result);

	//     - Call this when you're finished writing the report
	//////////////////////////////////////////////////////////////
	// scReportEnd
	// Summary
	//		Denotes the end of a report for the report specified.
	// Parameters
	//		theReport	: [in] A valid SC Report Object
	//		isAuth		: [in] Authoritative report
	//		theStatus	: [in] Final Status of the reported game
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		Used to set the end of a report. The report must have been properly created 
	//		and have some data. Any report being submitted requires that function be 
	//		called before the submission. Incomplete reports will be discarded.
	// See Also
	//		scCreateReport, scSubmitReport, SCGameStatus
	SCResult SC_CALL scReportEnd(SCReportPtr theReport, 
		gsi_bool    isAuth, 
		SCGameStatus theStatus);

	//////////////////////////////////////////////////////////////
	// scReportSetAsMatchless
	// Summary
	//		Called after creating the report to set it as a matchless report - 
	//		this is needed if the report is being submitted to a "matchless" game session.
	//		A "matchless" game may be more commonly known as non-arbitrated or unranked.
	// Parameters
	//		theReport	: [ref] A valid SC Report Object
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		This should not be used for a non-matchless session report.
	// See Also
	//		scCreateMatchlessSession, scCreateReport
	SCResult SC_CALL scReportSetAsMatchless(SCReportPtr theReport);


	// Utility to record key value pairs
	//////////////////////////////////////////////////////////////
	// scReportAddIntValue
	// Summary
	//		Adds an integer value to the report for a specific key.
	// Parameters
	//		theReportData	: [in] A valid SC Report object
	//		theKeyId		: [in] Key Identifier for reporting data
	//		theValue		: [in] 32 bit Integer value representation of the data
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		The host or player can call this function to add either global, player-, 
	//		or team-specific data. A report needs to be created before calling this 
	//		function. For global keys, this function can only be called after starting 
	//		global data. For player or teams, a new player or team needs to be added.
	// See Also
	//		scCreateReport, scReportBeginGlobalData, scReportBeginPlayerData, 
	//		scReportBeginTeamData, scReportBeginNewPlayer, scReportSetPlayerData, 
	//		scReportBeginNewTeam, scReportSetTeamData
	SCResult SC_CALL scReportAddIntValue(SCReportPtr theReportData,
		gsi_u16           theKeyId,
		gsi_i32           theValue);


	//////////////////////////////////////////////////////////////
	// scReportAddInt64Value
	// Summary
	//		Adds a 64-bit integer value to the report for a specific key.
	// Parameters
	//		theReportData	: [in] A valid SC Report object
	//		theKeyId		: [in] Key Identifier for reporting data
	//		theValue		: [in] 64 bit Integer value representation of the data
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		The host or player can call this function to add either global, player-, 
	//		or team-specific data. A report needs to be created before calling this 
	//		function. For global keys, this function can only be called after starting 
	//		global data. For player or teams, a new player or team needs to be added.
	// See Also
	//		scCreateReport, scReportBeginGlobalData, scReportBeginPlayerData, 
	//		scReportBeginTeamData, scReportBeginNewPlayer, scReportSetPlayerData, 
	//		scReportBeginNewTeam, scReportSetTeamData
	SCResult SC_CALL scReportAddInt64Value(SCReportPtr theReportData,
		gsi_u16           theKeyId,
		gsi_i64           theValue);

	//////////////////////////////////////////////////////////////
	// scReportAddShortValue
	// Summary
	//		Adds a short value to the report for a specific key.
	// Parameters
	//		theReportData	: [in] A valid SC Report object
	//		theKeyId		: [in] Key Identifier for reporting data
	//		theValue		: [in] 16 bit Short value representation of the data
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		The host or player can call this function to add either global, player-, 
	//		or team-specific data. A report needs to be created before calling this 
	//		function. For global keys, this function can only be called after starting 
	//		global data. For player or teams, a new player or team needs to be added.
	// See Also
	//		scCreateReport, scReportBeginGlobalData, scReportBeginPlayerData, 
	//		scReportBeginTeamData, scReportBeginNewPlayer, scReportSetPlayerData, 
	//		scReportBeginNewTeam, scReportSetTeamData
	SCResult SC_CALL scReportAddShortValue(const SCReportPtr theReportData,
		gsi_u16           theKeyId,
		gsi_i16           theValue);

	//////////////////////////////////////////////////////////////
	// scReportAddByteValue
	// Summary
	//		Adds a byte value to the report for a specific key.
	// Parameters
	//		theReportData	: [in] A valid SC Report object
	//		theKeyId		: [in] Key Identifier for reporting data
	//		theValue		: [in] 8 bit Byte value representation of the data
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		The host or player can call this function to add either global, player-, 
	//		or team-specific data. A report needs to be created before calling this 
	//		function. For global keys, this function can only be called after starting 
	//		global data. For player or teams, a new player or team needs to be added.
	// See Also
	//		scCreateReport, scReportBeginGlobalData, scReportBeginPlayerData, 
	//		scReportBeginTeamData, scReportBeginNewPlayer, scReportSetPlayerData, 
	//		scReportBeginNewTeam, scReportSetTeamData
	SCResult SC_CALL scReportAddByteValue(const SCReportPtr theReportData,
		gsi_u16           theKeyId,
		gsi_i8            theValue);

	//////////////////////////////////////////////////////////////
	// scReportAddFloatValue
	// Summary
	//		Adds a float value to the report for a specific key.
	// Parameters
	//		theReportData	: [in] A valid SC Report object
	//		theKeyId		: [in] Key Identifier for reporting data
	//		theValue		: [in] 32 bit Float value representation of the data
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		The host or player can call this function to add either global, player-, 
	//		or team-specific data. A report needs to be created before calling this 
	//		function. For global keys, this function can only be called after starting 
	//		global data. For player or teams, a new player or team needs to be added.
	// See Also
	//		scCreateReport, scReportBeginGlobalData, scReportBeginPlayerData, 
	//		scReportBeginTeamData, scReportBeginNewPlayer, scReportSetPlayerData, 
	//		scReportBeginNewTeam, scReportSetTeamData
	SCResult SC_CALL scReportAddFloatValue(const SCReportPtr theReportData,
		gsi_u16           theKeyId,
		float             theValue);

	//////////////////////////////////////////////////////////////
	// scReportAddStringValue
	// Summary
	//		Adds a string value to the report for a specific key.
	// Parameters
	//		theReportData	: [in] A valid SC Report object
	//		theKeyId		: [in] The string key's identifier
	//		theValue		: [in] The string value
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		The host or player can call this function to add either global, player-, 
	//		or team-specific data. A report needs to be created before calling this 
	//		function. For global keys, this function can only be called after starting 
	//		global data. For player or teams, a new player or team needs to be added.
	// See Also
	//		scCreateReport, scReportBeginGlobalData, scReportBeginPlayerData, 
	//		scReportBeginTeamData, scReportBeginNewPlayer, scReportSetPlayerData, 
	//		scReportBeginNewTeam, scReportSetTeamData
	SCResult SC_CALL scReportAddStringValue(const SCReportPtr theReportData,
		gsi_u16           theKeyId,
		const gsi_char *  theValue);

	//////////////////////////////////////////////////////////////
	// scDestroyReport
	// Summary
	//		Used to clean up and free the report object after it has been submitted.
	// Parameters
	//		theReport	: [in] The pointer to a created SC Report Object.
	// Returns
	//		An SCResult enum value used to indicate the specific result of the request. 
	//		This will return SCResult_NO_ERROR if the request completed successfully.
	// Remarks
	//		This should be called regardless of whether or not the report was submitted 
	//		successfully. It should only be used if the report object contains a valid 
	//		pointer from a successful call to scCreateReport.
	// See Also
	//		scCreateReport
	SCResult SC_CALL scDestroyReport(SCReportPtr theReport);

	//////////////////////////////////////////////////////////////
	// scCreateQueryParameterList
	// Summary
	//		Creates a SCQueryParameterList to use with query functions.
	// Parameters
	//		queryParams		: [out] The pointer to the query parameters created. This should point to NULL when passed.
	//		queryParamsCount: [in] The number of parameters that will be added
	// Returns
	//		Enum value used to indicate the specific result of the call. 
	//		This will return SCResult_NO_ERROR if the call completed successfully.
	// Remarks
	//		This needs to be called before performing any query function other than game queries. Any invalid input to the 
	//		function will return: SCResult_INVALID_PARAMETERS.  The user must pass in a queryParamsCount 
	//		large enough to add the parameters required by the query (as listed on the backend administration site). 
	//		The function will return a failure code if the queryParams points to a non-NULL object.
	//		When the query has completed, it is the user's responsibility to clean up the parameter list.<p>
	// See Also
	//		scAddQueryParameterToList, scDestroyQueryParameterList
	SCResult SC_CALL scCreateQueryParameterList(SCQueryParameterListPtr *queryParams, gsi_u32 queryParamsCount);

	//////////////////////////////////////////////////////////////
	// scAddQueryParameterToList
	// Summary
	//		Adds a parameter name and value to the parameter list.
	// Parameters
	//		queryParams		: [out] The pointer to the query parameters created.
	//		name			: [in] The name of the parameter
	//		value			: [in] The value of the parameter
	// Returns
	//		Enum value used to indicate the specific result of the call. 
	//		This will return SCResult_NO_ERROR if the call completed successfully.
	// Remarks
	//		Parameters need to be added to a SCQueryParameterList before passing a SCQueryParameterList
	//		to a query function.  This function should be used to add those parameters while not going
	//		over the number of parameters preallocated in scCreateQueryParameterList.  Keep in mind 
	//		that incorrect parameter names or values are not going to be handled here.  The query 
	//		function will inform the user appropriately.<p>
	// See Also
	//		scCreateQueryParameterList, scDestroyQueryParameterList
	SCResult SC_CALL scAddQueryParameterToList(SCQueryParameterListPtr queryParams, const gsi_char *name, const gsi_char *value);

	//////////////////////////////////////////////////////////////
	// scDestroyQueryParameterList
	// Summary
	//		Destroys a SCQueryParameterList object.
	// Parameters
	//		queryParams		: [out] The pointer to the query parameters created.
	// Returns
	//		Enum value used to indicate the specific result of the call. 
	//		This will return SCResult_NO_ERROR if the call completed successfully.
	// Remarks
	//		This function must be called after either a query is complete or the 
	//		SCQueryParameterList is no longer needed.  The function will free data 
	//		only if queryParams points to non-NULL data.  The parameter will also 
	//		be nulled out for the user.  <p>
	// See Also
	//		scCreateQueryParameterList
	SCResult SC_CALL scDestroyQueryParameterList(SCQueryParameterListPtr *queryParams);

	//////////////////////////////////////////////////////////////
	// scDestroyGameStatsQueryResponse
	// Summary
	//		Destroys a SCGameStatsQueryResponse object.
	// Parameters
	//		response		: [out] The pointer to the response passed into the callback.
	// Returns
	//		Enum value used to indicate the specific result of the call. 
	//		This will return SCResult_NO_ERROR if the call completed successfully.
	// Remarks
	//		Use this function to destroy and NULL out an SCGameStatsQueryResponse.  This
	//		call will only free a non-NULL response.<p>
	// See Also
	//		SCPlayerStatQueryCallback
	SCResult SC_CALL scDestroyGameStatsQueryResponse(SCGameStatsQueryResponse **response);

	//////////////////////////////////////////////////////////////
	// scDestroyPlayerStatsQueryResponse
	// Summary
	//		Destroys a SCPlayerStatsQueryResponse object.
	// Parameters
	//		response		: [out] The pointer to the response passed into the callback.
	// Returns
	//		Enum value used to indicate the specific result of the call. 
	//		This will return SCResult_NO_ERROR if the call completed successfully.
	// Remarks
	//		Use this function to destroy and NULL out an SCPlayerStatsQueryResponse.  This
	//		call will only free a non-NULL response.<p>
	// See Also
	//		SCPlayerStatQueryCallback
	SCResult SC_CALL scDestroyPlayerStatsQueryResponse(SCPlayerStatsQueryResponse **response);

	//////////////////////////////////////////////////////////////
	// scDestroyTeamStatsQueryResponse
	// Summary
	//		Destroys a scDestroyTeamStatsQueryResponse object.
	// Parameters
	//		response		: [out] The pointer to the response passed into the callback.
	// Returns
	//		Enum value used to indicate the specific result of the call. 
	//		This will return SCResult_NO_ERROR if the call completed successfully.
	// Remarks
	//		Use this function to destroy and NULL out an scDestroyTeamStatsQueryResponse.  This
	//		call will only free a non-NULL response.<p>
	// See Also
	//		SCTeamStatsQueryCallback
	SCResult SC_CALL scDestroyTeamStatsQueryResponse(SCTeamStatsQueryResponse **response);

	//////////////////////////////////////////////////////////////
	// SCGameStatsQueryCallback
	// Summary
	//		Called when the scRunGameStatsQuery request completes.
	// Parameters
	//		theInterface		: [in] A valid SCInterfacePtr.
	//		httpResult			: [in] http result used to describe lower level problems.
	//		result				: [in] Result specifying a problem with the query.
	//		msg					: [in] Pointer to text message explaining the result.
	//		msgLen				: [in] The length of msg
	//		response			: [in] A pointer to the stats response that can be traversed.
	//		userData			: [in/out] Application data that may be used in the callback.
	// Returns
	//		Enum value used to indicate the specific result of the call. 
	//		This will return SCResult_NO_ERROR if the call completed successfully.
	// Remarks
	//		The user needs to implement this function signature so that the name of the 
	//		function can be passed to scRunGameStatsQuery.  Destroying the response 
	//		is the user's responsibility.  The parameter msg is for debugging purposes only - 
	//		it should never be displayed to the player.<p>
	// See Also
	//		scRunGameStatsQuery, scDestroyGameStatsQueryResponse
	typedef void (*SCGameStatsQueryCallback)(const SCInterfacePtr theInterface,
		GHTTPResult httpResult,
		SCResult result,
		gsi_char * msg,
		int msgLen,
		SCGameStatsQueryResponse * response,
		void * userData);

	//////////////////////////////////////////////////////////////
	// scRunGameStatsQuery
	// Summary
	//		Performs a game stat query.
	// Parameters
	//		theInterface		: [in]	A valid SCInterfacePtr.
	//		certificate			: [in]	pointer to GSLoginCertificate obtained from authservice.
	//		privData			: [in]	pointer to GSLoginPrivateData obtained from authservice.
	//		ruleSetVersion		: [in]	ruleset version number created by the user on the 
	//									backend administration site.
	//		queryId				: [in]	A guid generated after building a query on the backend 
	//									administration site - you can use the variable defined in the
	//                                  auto-generated header file rather than pass the guid manually.
	//		queryParameters		: [in]	SCQueryParameterListPtr which was built with parameters 
	//									obtained from the backend administration site.
	//		callback			: [in]	user supplied callback that must be valid in order to receive stats.
	//		userData			: [in/out] Application data that may be used in the callback.
	// Returns
	//		Enum value used to indicate the specific result of the call. 
	//		This will return SCResult_NO_ERROR if the call completed successfully.
	// Remarks
	//		Queries must first be built using the backend administration site for ATLAS.  Once
	//		a query has been generated, the user can take advantage of this generic function for 
	//		game stats.  The user must pass in valid parameters or the following error will be 
	//		returned: SCResult_INVALID_PARAMETERS. The user should be calling the scThink function
	//		in order for this request to complete.<p>
	// See Also
	//		scCreateQueryParameterList
	SCResult SC_CALL scRunGameStatsQuery(SCInterfacePtr interfacePtr, 
		const GSLoginCertificate * certificate,
		const GSLoginPrivateData * privData,
		int ruleSetVersion,
		const char queryId[GS_GUID_SIZE],
		SCQueryParameterListPtr queryParameters,
		SCGameStatsQueryCallback callback,
		void * userData);



	//////////////////////////////////////////////////////////////
	// SCPlayerStatsQueryCallback
	// Summary
	//		Called when the scRunPlayerStatQuery request completes.
	// Parameters
	//		theInterface		: [in] A valid SCInterfacePtr.
	//		httpResult			: [in] http result used to describe lower level problems.
	//		result				: [in] result specifying a problem with the query.
	//		msg					: [in] Pointer to text message explaining the result.
	//		msgLen				: [in] The length of msg
	//		response			: [in] A pointer to the stats response that can be traversed.
	//		userData			: [in/out] Application data that may be used in the callback.
	// Returns
	//		Enum value used to indicate the specific result of the call. 
	//		This will return SCResult_NO_ERROR if the call completed successfully.
	// Remarks
	//		The user needs to implement this function signature so that the name of the 
	//		function can be passed to scRunPlayerStatsQuery.  Destroying the response 
	//		is the user's responsibility.  The parameter msg is for debugging purposes only -
	//		it should never be displayed to the player.<p>
	// See Also
	//		scRunPlayerStatsQuery, scDestroyPlayerStatsQueryResponse
	typedef void (*SCPlayerStatsQueryCallback)(const SCInterfacePtr theInterface, 
		GHTTPResult httpResult, 
		SCResult result, 
		gsi_char * msg,
		int msgLen,
		SCPlayerStatsQueryResponse * response, 
		void * userData);

	//////////////////////////////////////////////////////////////
	// scRunPlayerStatsQuery
	// Summary
	//		Performs a player stat query.
	// Parameters
	//		theInterface		: [in]	A valid SCInterfacePtr.
	//		certificate			: [in]	pointer to GSLoginCertificate obtained from authservice.
	//		privData			: [in]	pointer to GSLoginPrivateData obtained from authservice.
	//		ruleSetVersion		: [in]	ruleset version number created by the user on the 
	//									backend administration site.
	//		queryId				: [in]	A guid generated after building a query on the backend 
	//									administration site - you can use the variable defined in the
	//                                  auto-generated header file rather than pass the guid manually.
	//		queryParameters		: [in]	SCQueryParameterListPtr which was built with parameters 
	//									obtained from the backend administration site.
	//		callback			: [in]	user supplied callback that must be valid in order to receive stats.
	//		userData			: [in/out] Application data that may be used in the callback.
	// Returns
	//		Enum value used to indicate the specific result of the call. 
	//		This will return SCResult_NO_ERROR if the call completed successfully.
	// Remarks
	//		Queries must first be built using the backend administration site for ATLAS.  Once
	//		a query has been generated, the user can take advantage of this generic function for 
	//		player stats.  This function can be used for both single and multiple players or for a ranked leaderboard.  
	//		The user must pass in valid parameters or the following error will be returned: 
	//		SCResult_INVALID_PARAMETERS. The user should be calling the scThink function
	//		in order for this request to complete.<p>
	// See Also
	//		scCreateQueryParameterList
	SCResult SC_CALL scRunPlayerStatsQuery(SCInterfacePtr					theInterface,
		const GSLoginCertificate *		certificate,
		const GSLoginPrivateData *		privData,
		int								ruleSetVersion,
		const char						queryId[GS_GUID_SIZE],
		const SCQueryParameterListPtr	queryParameters,
		SCPlayerStatsQueryCallback		callback,
		void *							userData);


	//////////////////////////////////////////////////////////////
	// SCTeamStatsQueryCallback
	// Summary
	//		Called when the scRunTeamStatsQuery request completes.
	// Parameters
	//		theInterface		: [in] A valid SCInterfacePtr.
	//		httpResult			: [in] http result used to describe lower level problems.
	//		result				: [in] Result specifying a problem with the query.
	//		msg					: [in] Pointer to text message explaining the result.
	//		msgLen				: [in] The length of msg
	//		response			: [in] A pointer to the stats response that can be traversed.
	//		userData			: [in/out] Application data that may be used in the callback.
	// Returns
	//		Enum value used to indicate the specific result of the call. 
	//		This will return SCResult_NO_ERROR if the call completed successfully.
	// Remarks
	//		The user needs to implement this function signature so that the name of 
	//		function can be passed to scRunTeamStatsQuery.  Destroying the response 
	//		is the user's responsibility.  The parameter msg is for debugging purposes only -
	//		it should never be displayed to the player.<p>
	// See Also
	//		scRunTeamStatsQuery, scDestroyTeamStatsQueryResponse
	typedef void (*SCTeamStatsQueryCallback)(const SCInterfacePtr theInterface,
		GHTTPResult httpResult,
		SCResult result,
		gsi_char * msg,
		int msgLen,
		SCTeamStatsQueryResponse * response,
		void * userData);

	//////////////////////////////////////////////////////////////
	// scRunTeamStatsQuery
	// Summary
	//		Performs a team stat query.
	// Parameters
	//		theInterface		: [in]	A valid SCInterfacePtr.
	//		certificate			: [in]	pointer to GSLoginCertificate obtained from authservice.
	//		privData			: [in]	pointer to GSLoginPrivateData obtained from authservice.
	//		ruleSetVersion		: [in]	ruleset version number created by the user on the 
	//									backend administration site.
	//		queryId				: [in]	A guid generated after building a query on the backend 
	//									administration site - you can use the variable defined in the
	//                                  auto-generated header file rather than pass the guid manually.
	//		queryParameters		: [in]	SCQueryParameterListPtr which was built with parameters 
	//									obtained from the backend administration site.
	//		callback			: [in]	user supplied callback that must be valid in order to receive stats.
	//		userData			: [in/out] Application data that may be used in the callback.
	// Returns
	//		Enum value used to indicate the specific result of the call. 
	//		This will return SCResult_NO_ERROR if the call completed successfully.
	// Remarks
	//		Queries must first be built using the backend administration site for ATLAS.  Once
	//		a query has been generated, the user can take advantage of this generic function for 
	//		player stats.  This function can be used for both single and multiple players.  The user
	//		must pass in valid parameters or the following error will be returned: SCResult_INVALID_PARAMETERS.<p>
	// See Also
	//		scCreateQueryParameterList, SCTeamStatsQueryCallback
	SCResult SC_CALL scRunTeamStatsQuery(SCInterfacePtr					theInterface,
		const GSLoginCertificate *		certificate,
		const GSLoginPrivateData *		privData,
		int							ruleSetVersion,
		const char					queryId[GS_GUID_SIZE],
		const SCQueryParameterListPtr	queryParameters,
		SCTeamStatsQueryCallback		callback,
		void *							userData);

	//DOM-IGNORE-BEGIN

	///////////////////////////////////////////////////////////////////////////////
	// Peer to peer encryption utilities (Will probably be moved to common code)
	//     A symmetric cipher key for peer-to-peer communication
	//     Will usually have one key for sending and a second key for receiving
	typedef struct SCPeerCipher
	{
		RC4Context mRC4;
		gsi_u8  mKey[GS_CRYPT_RSA_BYTE_SIZE];
		gsi_u32 mKeyLen;
		gsi_bool mInitialized;
	} SCPeerCipher;

	typedef char SCPeerKeyExchangeMsg[GS_CRYPT_RSA_BYTE_SIZE];

	SCResult SC_CALL scPeerCipherInit(const GSLoginCertificate * theLocalCert, 
		SCPeerCipher * theCipher);

	SCResult SC_CALL scPeerCipherCreateKeyExchangeMsg(const GSLoginCertificate * theRemoteCert,
		const SCPeerCipher *       theCipher, 
		SCPeerKeyExchangeMsg       theMsgOut);

	SCResult SC_CALL scPeerCipherParseKeyExchangeMsg (const GSLoginCertificate * theLocalCert,  
		const GSLoginPrivateData * theCertPrivateData, 
		const SCPeerKeyExchangeMsg theMsg, 
		SCPeerCipher *             theCipherOut);

	// Encrypt/Decrypt in place, also the RC4 context is 
	// modified every time encryption/decryption take place
	SCResult SC_CALL scPeerCipherEncryptBuffer(SCPeerCipher * theCipher, 
		gsi_u8 * theData, 
		gsi_u32 theLen);

	SCResult SC_CALL scPeerCipherDecryptBuffer(SCPeerCipher * theCipher, 
		gsi_u8 * theData, 
		gsi_u32 theLen);

	// When using UDP (non-ordered) you must supply a message num
	//    - This is less efficient then encrypting an ordered stream
	SCResult SC_CALL scPeerCipherEncryptBufferIV(SCPeerCipher * theCipher, 
		gsi_u32 theMessageNum, 
		gsi_u8 * theData, 
		gsi_u32 theLen);

	SCResult SC_CALL scPeerCipherDecryptBufferIV(SCPeerCipher * theCipher, 
		gsi_u32 theMessageNum, 
		gsi_u8 * theData, 
		gsi_u32 theLen);

	//DOM-IGNORE-END

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#endif // __SC_H__
