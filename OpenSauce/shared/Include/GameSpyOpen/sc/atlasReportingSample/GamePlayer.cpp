// Copyright (c) IGN Entertainment, Inc.  All rights reserved.  
// This software is made available only pursuant to certain license terms offered
// by IGN or its subsidiary GameSpy Industries, Inc.  Unlicensed use or use in a 
// manner not expressly authorized by IGN or GameSpy is prohibited.

#include "GamePlayer.h"
#include "../atlasSampleCommon/Utility.h"

// Initialization for various things is shown here
GamePlayer::GamePlayer(const GameData &gameData):
	m_AtlasInterface(NULL),
	m_AtlasReport(NULL),
	m_LoginCert(),
	m_LoginPrivData(),
	m_State(GET_AUTH_CERT),
	m_Data(gameData)
{
	// Core should be initialized before attempting to use ATLAS or AuthService.
	gsCoreInitialize();

	// Start initializing the SDK at this point.  We can also initialize
	// when actually creating a session or reporting an intention based 
	// on a session.
	SCResult result = scInitialize(m_Data.m_GameId, &m_AtlasInterface);
	if (result != SCResult_NO_ERROR)
	{
		ATLAS_RS_LOG(GSIDebugType_Memory, GSIDebugLevel_HotError, "App incurred error with SDK: %s\n", SCResultToString(result));
		;
	}
}

// Cleanup for various things is shown here
GamePlayer::~GamePlayer(void)
{
	// cleanup our report that was submitted, specifically after submission was complete
	// or if an error occurred during submission
	if (m_AtlasReport)
		scDestroyReport(m_AtlasReport);

	// cleanup our atlas interface object since we're done using it
	if (m_AtlasInterface)
		scShutdown(m_AtlasInterface);
	
	// core needs to be shutdown last since it's the first to be started, and it's a dependency of 
	// more than just ATLAS.  continue to do so until tasks pending are complete
	gsCoreShutdown();
	while (gsCoreIsShutdown() == GSCore_SHUTDOWN_PENDING)
	{
		gsCoreThink(0);
		msleep(10);
	}
}

void OnObtainAuthServiceCertificate(GHTTPResult httpResult, WSLoginResponse * response, void * userData)
{
	// Once the AuthService request is complete we determine if we 
	// have received a valid AuthSerivce certificate.
	GamePlayer *caller = static_cast<GamePlayer *>(userData);
	if (httpResult != GHTTPSuccess)
	{
		ATLAS_RS_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed on player login, HTTP error: %d\n", 
			httpResult);
		caller->AdvanceState(true);
	}
	else if (response->mLoginResult != WSLogin_Success)
	{
		ATLAS_RS_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, 
			"Failed on player login, Login result: %d\n", response->mLoginResult);
		caller->AdvanceState(true);
	}
	else
	{
		_tprintf(_T("Successfully logged in as: %s\n"), response->mCertificate.mUniqueNick);
		// copy certificate and private key and let the app know that we've 
		caller->SetAuthServiceData(response->mCertificate, response->mPrivateData);
		caller->AdvanceState();
	}
	
}

void GamePlayer::ObtainAuthServiceCertificate(const std::string &email, const std::string &profile,
											const std::string &password, int partnerSpaceId, int nameSpaceId)
{
	// need to call this prior to Auth Service login
	wsSetGameCredentials(m_Data.m_AccessKey.c_str(), m_Data.m_GameId, m_Data.m_SecretKey.c_str());

	// Use this function to obtain an AuthService certificate for all ATLAS calls 
	// that happen after scInitialize
	_tprintf(_T("Obtaining auth service certificate...\n"));
	WSLoginValue loginVal = wsLoginProfile(m_Data.m_GameId, partnerSpaceId, nameSpaceId, _STR_CST(profile.c_str()),
		_STR_CST(email.c_str()), _STR_CST(password.c_str()), NULL, OnObtainAuthServiceCertificate, this);

	AdvanceState(loginVal != WSLogin_Success);
}

// Modify the sample app call this version of ObtainAuthServiceCertificate if you want 
// to run through a game that uses remote authentication
void GamePlayer::ObtainAuthServiceCertificate(const std::string &partnerChallenge, const std::string &remoteAuthToken,
											int partnerSpaceId, int nameSpaceId)
{
	// need to call this prior to Auth Service login
	wsSetGameCredentials(m_Data.m_AccessKey.c_str(), m_Data.m_GameId, m_Data.m_SecretKey.c_str());

	// Use this function to obtain an AuthService certificate for ATLAS calls 
	// that happen after scInitialize.  
	_tprintf(_T("Obtaining auth service certificate...\n"));
	WSLoginValue loginVal = wsLoginRemoteAuth(m_Data.m_GameId, partnerSpaceId, nameSpaceId, _STR_CST(remoteAuthToken.c_str()),
		_STR_CST(partnerChallenge.c_str()), OnObtainAuthServiceCertificate, this);

	AdvanceState(loginVal != WSLogin_Success);
}

void OnCreateSessionComplete(const SCInterfacePtr theInterface,
							 GHTTPResult          httpResult,
							 SCResult             result,
							 void *               userData)
{
	GamePlayer *caller = static_cast<GamePlayer *>(userData);
	if (httpResult == GHTTPSuccess && result == SCResult_NO_ERROR)
	{
		// Display the session id and connection id
		const char * sessionId    = scGetSessionId(theInterface);
		const char * connectionId = scGetConnectionId(theInterface);

		_tprintf(_T("Create ATLAS session successful\n"));
		_tprintf(_T("Session ID: %s\n"), sessionId);
		_tprintf(_T("Connection ID: %s\n"), connectionId);
		caller->AdvanceState();
		return;
	}	

	// We've failed to create a session. The error is displayed in the log.
	ATLAS_RS_LOG(GSIDebugType_Misc, GSIDebugLevel_WarmError, "Error: %s", SCResultToString(result));
	ATLAS_RS_LOG(GSIDebugType_Misc, GSIDebugLevel_WarmError, "Http Result: %d", httpResult);
	caller->AdvanceState(true);
}

void GamePlayer::CreateAtlasSession()
{
	// Using scCreateSession, start the process of creating a session.
	// This is an asynchronous call so the run loop will contain the call 
	// to process until the OnCreateSessionComplete callback gets called.
	_tprintf(_T("Creating ATLAS session...\n"));
	SCResult result;
	if (m_Data.m_Matchless)
		result = scCreateMatchlessSession(m_AtlasInterface, &m_LoginCert, &m_LoginPrivData, OnCreateSessionComplete, 0, this);	
	else 
		result = scCreateSession(m_AtlasInterface, &m_LoginCert, &m_LoginPrivData, OnCreateSessionComplete, 0, this);	
	
	AdvanceState(result != SCResult_NO_ERROR);
}

void OnSetReportIntentionComplete(SCInterfacePtr theInterface,
						  GHTTPResult          httpResult,
						  SCResult             result,
						  void *               userData)
{
	GamePlayer *caller = static_cast<GamePlayer *>(userData);
	if (result == SCResult_NO_ERROR)
	{		
		_tprintf(_T("Set report intention successful\n"));
		caller->AdvanceState();
		return;
	}	

	// We've failed to report an intention using a session. The error is displayed in the log.
	ATLAS_RS_LOG(GSIDebugType_Misc, GSIDebugLevel_WarmError, "Error: %s", SCResultToString(result));
	ATLAS_RS_LOG(GSIDebugType_Misc, GSIDebugLevel_WarmError, "Http Result: %d", httpResult);
	caller->AdvanceState(true);
	GSI_UNUSED(theInterface);
	GSI_UNUSED(httpResult);
}

void GamePlayer::SetReportIntention()
{
	// We are setting a report intention with connection id as NULL and the
	// authoritative flag set to true because we're the only submitter.
	// In dedicated server games, the report intention can be non-authoritative.
	// Also, the connection id can be a value from the previous session if the 
	// the player just reconnected to the same session.
	_tprintf(_T("Setting report intention for current ATLAS session...\n"));
	SCResult result = scSetReportIntention(m_AtlasInterface, NULL, 
		gsi_true, &m_LoginCert, &m_LoginPrivData, OnSetReportIntentionComplete, 0, this);
	AdvanceState(result != SCResult_NO_ERROR);
}

void OnSubmittAtlasReportComplete(SCInterfacePtr interfacePtr,
							GHTTPResult          httpResult,
							SCResult             result,
							void *               userData)
{
	GamePlayer *caller = static_cast<GamePlayer *>(userData);
	if (result == SCResult_NO_ERROR)
	{		
		_tprintf(_T("Submit report successful\n"));
		caller->AdvanceState();
		return;
	}	

	// We've failed to submit a report. The error is displayed in the log.
	ATLAS_RS_LOG(GSIDebugType_Misc, GSIDebugLevel_WarmError, "Error: %s", SCResultToString(result));
	ATLAS_RS_LOG(GSIDebugType_Misc, GSIDebugLevel_WarmError, "Http Result: %d", httpResult);
	caller->AdvanceState(true);
	GSI_UNUSED(interfacePtr);
	GSI_UNUSED(httpResult);
}

// Once the game host/client has created a session, and
// set a report intention, it can now submit a report for the 
// session.
void GamePlayer::SubmitAtlasReport()
{
	SCResult result;

	// Create the report with 1 player, and 0 teams
	result = scCreateReport(m_AtlasInterface, m_Data.m_RulesetVersion, 1, 0, &m_AtlasReport);
	if (result != SCResult_NO_ERROR)
	{
		ATLAS_RS_LOG(GSIDebugType_Misc, GSIDebugLevel_WarmError, "Error: %s", SCResultToString(result));
		AdvanceState(true);
		return;
	}
	
	if (m_Data.m_Matchless)
	{
		result = scReportSetAsMatchless(m_AtlasReport);
		if (result != SCResult_NO_ERROR)
		{
			ATLAS_RS_LOG(GSIDebugType_Misc, GSIDebugLevel_WarmError, "Error: %s", SCResultToString(result));
			AdvanceState(true);
			return;
		}
	}
	

	//  Begin the game's (global data) section of the report
	result = scReportBeginGlobalData(m_AtlasReport);
	if (result != SCResult_NO_ERROR)
	{
		ATLAS_RS_LOG(GSIDebugType_Misc, GSIDebugLevel_WarmError, "Error: %s", SCResultToString(result));
		AdvanceState(true);
		return;
	}

	// add the game-level key values
	for (std::vector<AtlasKey *>::const_iterator it = m_Data.m_GameKeys.begin(); 
		it != m_Data.m_GameKeys.end(); it++)
	{
		AtlasKey * key = *it;
		result = key->InsertKeyValueIntoReport(m_AtlasReport);
		if (result != SCResult_NO_ERROR)
		{
			AdvanceState(true);
			return;
		}
	}

	//  Begin the player section of the report
	result = scReportBeginPlayerData(m_AtlasReport);
	if (result != SCResult_NO_ERROR)
	{
		AdvanceState(true);
		return;
	}

	// This is the start of a new player 
	result = scReportBeginNewPlayer(m_AtlasReport);
	if (result != SCResult_NO_ERROR)
	{
		AdvanceState(true);
		return;
	}

	// Game Results are added first
	result = scReportSetPlayerData(m_AtlasReport, 0, reinterpret_cast<const gsi_u8 *>(scGetConnectionId(m_AtlasInterface)), 0, 
		SCGameResult_WIN, m_LoginCert.mProfileId, &m_LoginCert, NULL);
	if (result != SCResult_NO_ERROR)
	{
		AdvanceState(true);
		return;
	}

	// key values are added next
	for (std::vector<AtlasKey *>::const_iterator it = m_Data.m_PlayerKeys.begin(); 
		 it != m_Data.m_PlayerKeys.end(); it++)
	{
		AtlasKey * key = *it;
		result = key->InsertKeyValueIntoReport(m_AtlasReport);
		if (result != SCResult_NO_ERROR)
		{
			AdvanceState(true);
			return;
		}
	}
	// End of player stats being reported

	// Begin team section but don't report any keys
	result = scReportBeginTeamData(m_AtlasReport);
	if (result != SCResult_NO_ERROR)
	{
		AdvanceState(true);
		return;
	}
	
	// Close the report out
	result = scReportEnd(m_AtlasReport, gsi_true, SCGameStatus_COMPLETE);
	if (result != SCResult_NO_ERROR)
	{
		AdvanceState(true);
		return;
	}

	// Submit the report to the ATLAS backend, making sure that it is a 
	// authoritative report, with the local player's auth certificate and
	// private data. 
	_tprintf(_T("Submitting a report for the ATLAS session...\n"));
	result = scSubmitReport(m_AtlasInterface, m_AtlasReport, gsi_true, &m_LoginCert, 
		&m_LoginPrivData, OnSubmittAtlasReportComplete, 0, this);
	AdvanceState(result != SCResult_NO_ERROR);
}

bool GamePlayer::Run()
{	
	// we weren't able to create a valid instance of the SDK, Bail out!
	if (m_AtlasInterface == NULL)
		return false;

	// Continue to process throughout the code either till completion or 
	// if an error occurred.
	while (GetState() != ERROR_OCCURED && GetState() != REPORT_COMPLETE)
	{
		switch (GetState())
		{
			case GETTING_AUTH_CERT:
			case CREATING_SESSION:
			case SETTING_REPORT_INTENTION:
			case SUBMITTING_REPORT:
			{
				// we're still in the process of doing one of the above operations.
				break;
			}
			case GET_AUTH_CERT:
			{
				// We've just started getting an AuthService cert.
				ObtainAuthServiceCertificate(m_Data.m_Email, m_Data.m_Profile, m_Data.m_Password, m_Data.m_PartnerId, m_Data.m_NamespaceId);
				break;
			}
			
			case CREATE_SESSION:
			{
				// We're now ready to create a session
				CreateAtlasSession();
				break;
			}
			case SET_REPORT_INTENTION:
			{
				// Session created, lets now set our report intention.
				SetReportIntention();
				break;
			}
			case SUBMIT_REPORT:
			{
				// Set report intention successful, we can now submit a report for the intention.
				SubmitAtlasReport();
				break;
			}			
			default: 
			{
				ATLAS_RS_LOG(GSIDebugType_Misc, GSIDebugLevel_WarmError, "Error occured, please see previous logs.\n");
			}
		}

		if (GetState() != ERROR_OCCURED)
		{
			gsCoreThink(0);
			if (m_AtlasInterface)
				scThink(m_AtlasInterface);
		}		
	}

	return GetState() != ERROR_OCCURED;
}
