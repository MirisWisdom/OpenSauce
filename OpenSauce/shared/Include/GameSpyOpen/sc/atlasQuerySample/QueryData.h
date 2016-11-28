// Copyright (c) IGN Entertainment, Inc.  All rights reserved.  
// This software is made available only pursuant to certain license terms offered
// by IGN or its subsidiary GameSpy Industries, Inc.  Unlicensed use or use in a 
// manner not expressly authorized by IGN or GameSpy is prohibited.

#ifndef _QUERYDATA_H_
#define _QUERYDATA_H_
#include <string>

struct QueryData
{
	// game and player info
	int gameId;
	std::string gameName;
	std::string secretKey;
	std::string accessKey;
	int namespaceId;
	int partnerId;
	std::string email;
	std::string profile;
	std::string password;
	
	int rulesetVer;

	// query parameters for retrieving 1 record (your own)
	int query1ParamCount;
	gsi_char * query1ParamName;  // "profileid"
	gsi_char * query1ParamValue;  // "<profileid num>"
	char query1QueryID[GS_GUID_SIZE];  // ID listed on Atlas admin site (under Query Management) 

	// query parameters for retrieving a leaderboard of top 10 records sorted by BEST_RACE_TIME
	int query2ParamCount;
	gsi_char * query2Param1Name;  // "pageindex"
	gsi_char * query2Param1Value;  // "1"
	gsi_char * query2Param2Name;  // "pagesize"
	gsi_char * query2Param2Value;  // "10"
	char query2QueryID[GS_GUID_SIZE]; // ID listed on Atlas admin site (under Query Management)

	// query parameters for retrieving a leaderboard of the next 10 records (11-20) sorted by BEST_RACE_TIME
	// *note that this is identical to the above query parameters except that 2 is passed for pageindex
	int query3ParamCount;
	gsi_char * query3Param1Name; // "pageindex"
	gsi_char * query3Param1Value; // "2"
	gsi_char * query3Param2Name; // "pagesize" 
	gsi_char * query3Param2Value; // "10" 
	char query3QueryID[GS_GUID_SIZE]; // ID listed on Atlas admin site (under Query Management)

	// query parameters for retrieving the game stats
	int query4ParamCount;
	char query4QueryID[GS_GUID_SIZE]; // ID listed on Atlas admin site (under Query Management)
};

#endif