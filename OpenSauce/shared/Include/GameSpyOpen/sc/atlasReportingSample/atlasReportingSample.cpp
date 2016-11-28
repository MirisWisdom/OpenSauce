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
#include "GameData.h"
#include "GamePlayer.h"
#include "../atlasSampleCommon/Utility.h"


///////////////////////////////////////////////////////////////////////////////
// This sample app is organized as follows: 
// A GamePlayer (reporter) object is given GameData to report 
// to the ATLAS backend.  The GamePlayer object will take care of 
// all prerequisites as well as ATLAS tasks that need to happen when 
// reporting a game.
// Future implementations will be split into a Host and a Client which 
// depend on parameters passed to the app.


// This function allows us to configure a game data structure 
void SetupGameData(GameData &currentGame)
{
	// Keep in mind that the data you see here is definitely something 
	// that you can play around with to get comfortable with ATLAS reporting.
	// Each item is something that can be configured to a certain extent.
	static const int MIN_RACE_TIME = 240000;
	currentGame.m_GameId = 1649; 
	currentGame.m_GameName = "atlasSamples";
	currentGame.m_SecretKey = "Zc0eM6";
	//currentGame.m_GameName = "gmtest";
	//currentGame.m_SecretKey = "HA6zkS";
	currentGame.m_AccessKey = "39cf9714e24f421c8ca07b9bcb36c0f5";
	currentGame.m_RulesetVersion = ATLAS_RULE_SET_VERSION;
	currentGame.m_PartnerId = 0;
	currentGame.m_NamespaceId = 1;
	currentGame.m_Email = "sctest@gamespy.com";
	currentGame.m_Profile = "sctest01";
	currentGame.m_Password = "gspy";
	currentGame.m_Matchless = false;

	time_t myRaceTime = rand() % INT_MAX + MIN_RACE_TIME;

	// Add the keys we want here. We can replace the used vehicle key with 
	// either ATLAS_KEY_USED_VEHICLE_1, ATLAS_KEY_USED_VEHICLE_2, or ATLAS_KEY_USED_VEHICLE_3
	// The same goes for the track used key but with the keys related to the track.
	currentGame.m_PlayerKeys.push_back(new AtlasInt64Key(ATLAS_KEY_RACE_TIME, myRaceTime));
	currentGame.m_PlayerKeys.push_back(new AtlasByteKey(ATLAS_KEY_USED_VEHICLE_1, 1));
	currentGame.m_PlayerKeys.push_back(new AtlasByteKey(ATLAS_KEY_USED_TRACK_1, 1));
	
	// note that the same key can be used in the game section and player section, as each is needed for rules
	// of the respective scope
	currentGame.m_GameKeys.push_back(new AtlasByteKey(ATLAS_KEY_USED_TRACK_1, 1));
	// note - if multiple players used vehicle 1, then this value would be > 1
	currentGame.m_GameKeys.push_back(new AtlasByteKey(ATLAS_KEY_USED_VEHICLE_1, 1));
	// note - this is the value of the winner's race time; since there's only 1 player it's the same as the player key
	currentGame.m_GameKeys.push_back(new AtlasInt64Key(ATLAS_KEY_RACE_TIME, myRaceTime));
}

int RunThroughSampleReport(std::vector<std::string> &args)
{
	// will be used in the future, simply getting rid of warnings
	GSI_UNUSED(args);

	GameData myGameData;
	SetupGameData(myGameData);
	
	SetDebugLogPreferences();
	// Availability check needs to be performed once when initializing either the game 
	// or when transitioning to multi-player.  Regardless of which, it's best to perform this 
	// on a separate thread to prevent DNS queries from blocking. 
	// NOTE that this is just a sample of how to call things in a very simple manner.  
	// Each game's actual implementation will vary!  The loop can be part of a network thread 
	// which eliminates blocking the UI rendering code.

	GSIStartAvailableCheck(_STR_CST(myGameData.m_GameName.c_str()));
	GSIACResult availabilityResult = GSIAvailableCheckThink();
	while (availabilityResult == GSIACWaiting)
	{
		availabilityResult = GSIAvailableCheckThink();
	}
	switch(availabilityResult)
	{
		case GSIACUnavailable:
		{
			ATLAS_RS_LOG(GSIDebugType_Misc, GSIDebugLevel_WarmError, "Stats system not available.\n");
			return -1;
		}
		case GSIACTemporarilyUnavailable:
		{
			ATLAS_RS_LOG(GSIDebugType_Misc, GSIDebugLevel_WarmError, "Stats system temporarily down.\n");
			return -1;
		}
		case GSIACAvailable:
		{
			_tprintf(_T("Stats system available.\n"));
			break;
		}
		default:
		{
			ATLAS_RS_LOG(GSIDebugType_Misc, GSIDebugLevel_WarmError, "Unexpected status %d\n", availabilityResult);
			return -1;
		}
	}

	// The game player object basically runs through a simple example of how to submit an atlas report.
	GamePlayer gameHost(myGameData);
	if (!gameHost.Run())
	{
		ATLAS_RS_LOG(GSIDebugType_Misc, GSIDebugLevel_WarmError, "App failed to successfully report a game.\n");
		return -1;
	}

	return 0; // a successful run occurred
}

#if (defined _UNIX || defined _PS3) && !defined _IPHONE
int test_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	std::vector<std::string> args;
	for (int i = 0; i < argc; i++)
	{
		if (argv[i])
			args.push_back(argv[i]);
	}
	return RunThroughSampleReport(args);
}
