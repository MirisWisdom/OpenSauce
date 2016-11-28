///////////////////////////////////////////////////////////////////////////////
// File:	atlasReportingSample.cpp
// Sample:	GameSpy ATLAS SDK Sample App for reporting data
//
// Copyright (c) IGN Entertainment, Inc.  All rights reserved.  
// This software is made available only pursuant to certain license terms offered
// by IGN or its subsidiary GameSpy Industries, Inc.  Unlicensed use or use in a 
// manner not expressly authorized by IGN or GameSpy is prohibited.
#include <vector>
#include "../../common/gsCommon.h"
#include "../../common/gsAvailable.h"
#include "../atlasSampleCommon/atlas_atlas_samples_v1.h"
#include "QueryData.h"
#include "StatsCourier.h"
#include "../atlasSampleCommon/Utility.h"


///////////////////////////////////////////////////////////////////////////////
// This sample app is organized as follows: 
// A StatsCourier object is given QueryData to query stats and leaderboards 
// from the ATLAS backend.  The StatsCourier object will take care of 
// all prerequisites as well as ATLAS tasks that need to happen when 
// querying for stats.


// This function allows us to configure a query data structure 
extern "C" void SetupStatsData(QueryData &queryStats)
{
	// game and player info
	queryStats.gameId = 1649; 
	queryStats.gameName = "atlasSamples";
	queryStats.secretKey = "Zc0eM6";
	queryStats.accessKey = "39cf9714e24f421c8ca07b9bcb36c0f5";
	queryStats.partnerId = 0;
	queryStats.namespaceId = 1;
	queryStats.email = "sctest@gamespy.com";
	queryStats.profile = "sctest01";
	queryStats.password = "gspy";

	queryStats.rulesetVer = ATLAS_RULE_SET_VERSION;
	
	// query parameters for retrieving your own player stats record
	queryStats.query1ParamCount = 1;
	queryStats.query1ParamName = _T("profileid"); 
	queryStats.query1ParamValue = _T("64880026");  // "<profileid num>"
	sprintf(queryStats.query1QueryID, "%s", ATLAS_Query_STATS_Player_PROFILE_STATS_PLAYER);

	// query parameters for retrieving a leaderboard of top 10 records sorted by BEST_RACE_TIME
	queryStats.query2ParamCount = 2;
	queryStats.query2Param1Name = _T("pageindex");
	queryStats.query2Param1Value = _T("1");
	queryStats.query2Param2Name = _T("pagesize");  
	queryStats.query2Param2Value = _T("10"); 
	sprintf(queryStats.query2QueryID, "%s",ATLAS_Query_STATS_Player_LEADERBOARD_PLAYER_RACETIME);

	// query parameters for retrieving a leaderboard of the next 10 records (11-20) sorted by BEST_RACE_TIME
	// *note that this is identical to the above query parameters except that 2 is passed for pageindex
	queryStats.query3ParamCount = 2;
	queryStats.query3Param1Name = _T("pageindex");
	queryStats.query3Param1Value = _T("2");
	queryStats.query3Param2Name = _T("pagesize");  
	queryStats.query3Param2Value = _T("10"); 
	sprintf(queryStats.query3QueryID,"%s", ATLAS_Query_STATS_Player_LEADERBOARD_PLAYER_RACETIME);

	// query parameters for retrieving the game stats
	queryStats.query4ParamCount = 0;
	sprintf(queryStats.query4QueryID,"%s", ATLAS_Query_METRICS_Global_METRICS_GLOBAL);

	GSI_UNUSED(ATLAS_Query_STATS_Player_PLAYER_LB);
	GSI_UNUSED(ATLAS_Query_STATS_Player_LEADERBOARD_PLAYER_RANK);

}

extern "C" int RunThroughSampleQueries(std::vector<std::string> &args)
{
	// will be used in the future, simply getting rid of warnings
	GSI_UNUSED(args);

	QueryData myQueryData;
	SetupStatsData(myQueryData);

	SetDebugLogPreferences();

	// Availability check needs to be performed once when initializing either the game 
	// or when transitioning to multi-player.  Regardless of which, it's best to perform this 
	// on a separate thread to prevent DNS queries from blocking. 
	// NOTE that this is just a sample of how to call things in a very simple manner.  
	// Each game's actual implementation will vary!  The loop can be part of a network thread 
	// which eliminates blocking the UI rendering code.

	GSIStartAvailableCheck(_T(myQueryData.gameName.c_str()));
	GSIACResult availabilityResult = GSIAvailableCheckThink();
	while (availabilityResult == GSIACWaiting)
	{
		availabilityResult = GSIAvailableCheckThink();
	}
	switch(availabilityResult)
	{
	case GSIACUnavailable:
		{
			ATLAS_QS_LOG(GSIDebugType_Misc, GSIDebugLevel_WarmError, "Stats system not available.\n");
			return -1;
		}
	case GSIACTemporarilyUnavailable:
		{
			ATLAS_QS_LOG(GSIDebugType_Misc, GSIDebugLevel_WarmError, "Stats system temporarily down.\n");
			return -1;
		}
	case GSIACAvailable:
		{
			ATLAS_QS_LOG(GSIDebugType_Misc, GSIDebugLevel_Debug, "Stats system available.\n");
			break;
		}
	default:
		{
			ATLAS_QS_LOG(GSIDebugType_Misc, GSIDebugLevel_WarmError, "Stats system unexpected state %i.\n",availabilityResult );
		}			
	}


	// StatsCourier object will run through a few sample queries
	StatsCourier statsCourier(myQueryData);
	if (!statsCourier.Run())
	{
		ATLAS_QS_LOG(GSIDebugType_Misc, GSIDebugLevel_WarmError, "App failed to successfully query stats.\n");
		return -1;
	}
	return 0; // a successful run occurred
}

#if (defined _UNIX || defined _PS3) && !defined _IPHONE
extern "C" int test_main(int argc, char *argv[])
#else
extern "C" int main(int argc, char *argv[])
#endif
{
	std::vector<std::string> args;
	for (int i = 0; i < argc; i++)
	{
		if (argv[i])
			args.push_back(argv[i]);
	}
	return RunThroughSampleQueries(args);
}
