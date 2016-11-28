// Copyright (c) IGN Entertainment, Inc.  All rights reserved.  
// This software is made available only pursuant to certain license terms offered
// by IGN or its subsidiary GameSpy Industries, Inc.  Unlicensed use or use in a 
// manner not expressly authorized by IGN or GameSpy is prohibited.

#include "StatsCourier.h"
#include "../atlasSampleCommon/Utility.h"
#include "../sc.h"

// Initialization for various things is shown here
StatsCourier::StatsCourier(const QueryData &queryData):
	m_Data(queryData),	
	m_AtlasInterface(NULL),
	m_State(GET_AUTH_CERT),
	m_QueryParamsList(NULL),
	m_LoginCert(),
	m_LoginPrivData()
{
	// Core should be initialized before attempting to use ATLAS or AuthService.
	gsCoreInitialize();

	// Start initializing the SDK at this point
	SCResult result = scInitialize(m_Data.gameId, &m_AtlasInterface);
	if (result != SCResult_NO_ERROR)
	{
		ATLAS_QS_LOG(GSIDebugType_Memory, GSIDebugLevel_HotError, "App incurred error with SDK: %s\n", SCResultToString(result));
		;
	}
}

// Cleanup for various things is shown here
StatsCourier::~StatsCourier(void)
{
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
	StatsCourier *caller = static_cast<StatsCourier *>(userData);
	if (httpResult != GHTTPSuccess)
	{
		ATLAS_QS_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed on player login, HTTP error: %d\n", 
			httpResult);
		caller->AdvanceState(true);
	}
	else if (response->mLoginResult != WSLogin_Success)
	{
		ATLAS_QS_LOG(GSIDebugType_Misc, GSIDebugLevel_HotError, 
			"Failed on player login, Login result: %d\n", response->mLoginResult);
		caller->AdvanceState(true);
	}
	else
	{
		// copy certificate and private key and let the app know that we've 
		caller->SetAuthServiceData(response->mCertificate, response->mPrivateData);
		caller->AdvanceState();
	}
	
}

void StatsCourier::ObtainAuthServiceCertificate(const std::string &email, const std::string &profile,
											const std::string &password, int partnerSpaceId, int nameSpaceId)
{
	// need to call this prior to Auth Service login
	wsSetGameCredentials(m_Data.accessKey.c_str(), m_Data.gameId, m_Data.secretKey.c_str());

	// Use this function to obtain an AuthService certificate for all ATLAS calls 
	// that happen after scInitialize
	WSLoginValue loginVal = wsLoginProfile(m_Data.gameId, partnerSpaceId, nameSpaceId, _STR_CST(profile.c_str()),
		_STR_CST(email.c_str()), _STR_CST(password.c_str()), NULL, OnObtainAuthServiceCertificate, this);

	AdvanceState(loginVal != WSLogin_Success);
}

// Modify the sample app call this version of ObtainAuthServiceCertificate if you want 
// to run through a game that uses remote authentication
void StatsCourier::ObtainAuthServiceCertificate(const std::string &partnerChallenge, const std::string &remoteAuthToken,
											int partnerSpaceId, int nameSpaceId)
{
	// need to call this prior to Auth Service login
	wsSetGameCredentials(m_Data.accessKey.c_str(), m_Data.gameId, m_Data.secretKey.c_str());

	// Use this function to obtain an AuthService certificate for all ATLAS calls 
	// that happen after scInitialize
	WSLoginValue loginVal = wsLoginRemoteAuth(m_Data.gameId, partnerSpaceId, nameSpaceId, _STR_CST(remoteAuthToken.c_str()),
		_STR_CST(partnerChallenge.c_str()), OnObtainAuthServiceCertificate, this);

	AdvanceState(loginVal != WSLogin_Success);
}

void OnPlayerStatsQueryComplete(SCInterfacePtr theInterface,
								  GHTTPResult          httpResult,
								  SCResult             result,
								  gsi_char *           msg,
								  int                  msgLen,
								  SCPlayerStatsQueryResponse * response,
								  void *               userData)
{
	StatsCourier *caller = static_cast<StatsCourier *>(userData);
	unsigned int i;

	if (result == SCResult_NO_ERROR)
	{		
		ATLAS_QS_LOG(GSIDebugType_Misc, GSIDebugLevel_Debug, "Query successful: \n");
		
		// print record data here
		for (i = 0; i < response->mCategories->mPlayersCount; i++)
		{
			unsigned int j;
			//_tprintf("%d)\n", i+1);
			for (j=0; j < response->mCategories->mPlayers[i].mStatsCount; j++)
			{
				_tprintf(_WRD_CST("  %s: %s\n"), response->mCategories->mPlayers[i].mStats[j].mName, response->mCategories->mPlayers[i].mStats[j].mValue);
			}
			_tprintf(_WRD_CST("  profileid: %d\n\n"), response->mCategories->mPlayers[i].mProfileId);
		}

		//cleanup
		scDestroyPlayerStatsQueryResponse(&response);
		caller->AdvanceState();
		GSI_UNUSED(httpResult);
		return;
	}	

	ATLAS_QS_LOG(GSIDebugType_Misc, GSIDebugLevel_WarmError, "Error: %s", SCResultToString(result));
	ATLAS_QS_LOG(GSIDebugType_Misc, GSIDebugLevel_WarmError, "Http Result: %d", httpResult);
	caller->AdvanceState(true);
	GSI_UNUSED(theInterface);
	GSI_UNUSED(msg);
	GSI_UNUSED(msgLen);
}

void OnGameStatsQueryComplete(SCInterfacePtr theInterface,
								GHTTPResult          httpResult,
								SCResult             result,
								gsi_char *           msg,
								int                  msgLen,
								SCGameStatsQueryResponse * response,
								void *               userData)
{
	StatsCourier *caller = static_cast<StatsCourier *>(userData);

	if (result == SCResult_NO_ERROR)
	{		
		ATLAS_QS_LOG(GSIDebugType_Misc, GSIDebugLevel_Debug, "Query successful: \n");

		// print record data here
		unsigned int j;
		for (j=0; j < response->mCategories->mStatsCount; j++)
		{
			_tprintf(_WRD_CST("  %s: %s\n"), response->mCategories->mStats[j].mName, response->mCategories->mStats[j].mValue);
		}

		//cleanup
		scDestroyGameStatsQueryResponse(&response);
		caller->AdvanceState();
		GSI_UNUSED(httpResult);
		return;
	}	

	ATLAS_QS_LOG(GSIDebugType_Misc, GSIDebugLevel_WarmError, "Error: %s", SCResultToString(result));
	ATLAS_QS_LOG(GSIDebugType_Misc, GSIDebugLevel_WarmError, "Http Result: %d", httpResult);
	caller->AdvanceState(true);
	GSI_UNUSED(theInterface);
	GSI_UNUSED(msg);
	GSI_UNUSED(msgLen);
}

// retrieve your own player stats record
void StatsCourier::RunQuery1()
{
	// setup query parameter list and run query (then wait for callback)
	SCResult result = scCreateQueryParameterList(&m_QueryParamsList, m_Data.query1ParamCount);
	if (result == SCResult_NO_ERROR)
	{
		result = scAddQueryParameterToList(m_QueryParamsList, m_Data.query1ParamName, m_Data.query1ParamValue);

		if (result == SCResult_NO_ERROR)
		{
			ATLAS_QS_LOG(GSIDebugType_Misc, GSIDebugLevel_Debug, "Querying my PlayerStats record...\n");
			result = scRunPlayerStatsQuery(m_AtlasInterface, &m_LoginCert, &m_LoginPrivData, m_Data.rulesetVer, m_Data.query1QueryID, m_QueryParamsList, OnPlayerStatsQueryComplete, this);
		}

	}

	// cleanup
	scDestroyQueryParameterList(&m_QueryParamsList);
	AdvanceState(result != SCResult_NO_ERROR);
}

// retrieve the player stats top 10 leaderboard sorted by BEST_RACE_TIME 
void StatsCourier::RunQuery2()
{
	// setup query parameter list and run query (then wait for callback)
	SCResult result = scCreateQueryParameterList(&m_QueryParamsList, m_Data.query2ParamCount);
	if (result == SCResult_NO_ERROR)
	{
		result = scAddQueryParameterToList(m_QueryParamsList, m_Data.query2Param1Name, m_Data.query2Param1Value);
		result = scAddQueryParameterToList(m_QueryParamsList, m_Data.query2Param2Name, m_Data.query2Param2Value);

		if (result == SCResult_NO_ERROR)
		{
			ATLAS_QS_LOG(GSIDebugType_Misc, GSIDebugLevel_Debug, "Querying top page of player stats leaderboard sorted by BEST_RACE_TIME...\n");
			result = scRunPlayerStatsQuery(m_AtlasInterface, &m_LoginCert, &m_LoginPrivData, m_Data.rulesetVer, m_Data.query2QueryID, m_QueryParamsList, OnPlayerStatsQueryComplete, this);
		}

	}

	// cleanup
	scDestroyQueryParameterList(&m_QueryParamsList);
	AdvanceState(result != SCResult_NO_ERROR);
}

// retrieve the player stats leaderboard page 2 (records 11-20) sorted by BEST_RACE_TIME 
void StatsCourier::RunQuery3()
{
	// setup query parameter list and run query (then wait for callback)
	SCResult result = scCreateQueryParameterList(&m_QueryParamsList, m_Data.query3ParamCount);
	if (result == SCResult_NO_ERROR)
	{
		result = scAddQueryParameterToList(m_QueryParamsList, m_Data.query3Param1Name, m_Data.query3Param1Value);
		result = scAddQueryParameterToList(m_QueryParamsList, m_Data.query3Param2Name, m_Data.query3Param2Value);

		if (result == SCResult_NO_ERROR)
		{
			ATLAS_QS_LOG(GSIDebugType_Misc, GSIDebugLevel_Debug, "Querying page 2 of player stats leaderboard sorted by BEST_RACE_TIME...\n");
			result = scRunPlayerStatsQuery(m_AtlasInterface, &m_LoginCert, &m_LoginPrivData, m_Data.rulesetVer, m_Data.query3QueryID, m_QueryParamsList, OnPlayerStatsQueryComplete, this);
		}

	}

	// cleanup
	scDestroyQueryParameterList(&m_QueryParamsList);
	AdvanceState(result != SCResult_NO_ERROR);
}

// retrieve the game stats record
void StatsCourier::RunQuery4()
{
	SCResult result;

	// run query (then wait for callback)
	ATLAS_QS_LOG(GSIDebugType_Misc, GSIDebugLevel_Debug, "Querying GameStats record...\n");
	result = scRunGameStatsQuery(m_AtlasInterface, &m_LoginCert, &m_LoginPrivData, m_Data.rulesetVer, m_Data.query4QueryID, NULL, OnGameStatsQueryComplete, this);

	// cleanup
	AdvanceState(result != SCResult_NO_ERROR);
}

bool StatsCourier::Run()
{
	// we weren't able to create a valid instance of the SDK, bail out!
	if (m_AtlasInterface == NULL)
		return false;

	// Continue to process throughout the code either till completion or 
	// if an error occurred.
	while (GetState() != ERROR_OCCURED && GetState() != QUERIES_COMPLETE)
	{
		switch (GetState())
		{
			case GET_AUTH_CERT:
			{
				// We've just started getting an AuthService cert.
				ObtainAuthServiceCertificate(m_Data.email, m_Data.profile, m_Data.password, m_Data.partnerId, m_Data.namespaceId);
				break;
			}
			case GETTING_AUTH_CERT:
			{
				// we're still in the process of getting AuthService cert.
				break;
			}
			case RUN_QUERY1:
			{
				// setup the parameter list and run a player stats query for your own record
				RunQuery1(); 
				break;
			}
			case RUNNING_QUERY1:
			{
				// we're waiting for the callback with our query response
				break;
			}
			case RUN_QUERY2:
			{
				// setup the parameter list and run a player stats query for your top 25 records by RANK
				RunQuery2();
				break;
			}
			case RUNNING_QUERY2:
			{
				// we're waiting for the callback with our query response
				break;
			}
			case RUN_QUERY3:
				{
					// setup the parameter list and run a player stats query for your top 25 records by RANK
					RunQuery3();
					break;
				}
			case RUNNING_QUERY3:
				{
					// we're waiting for the callback with our query response
					break;
				}
			case RUN_QUERY4:
				{
					// setup the parameter list and run a player stats query for your top 25 records by RANK
					RunQuery4();
					break;
				}
			case RUNNING_QUERY4:
				{
					// we're waiting for the callback with our query response
					break;
				}
			default: 
			{
				ATLAS_QS_LOG(GSIDebugType_Misc, GSIDebugLevel_WarmError, "Error occured, please see previous logs.\n");
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
