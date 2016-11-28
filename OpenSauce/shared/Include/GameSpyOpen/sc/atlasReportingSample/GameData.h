// Copyright (c) IGN Entertainment, Inc.  All rights reserved.  
// This software is made available only pursuant to certain license terms offered
// by IGN or its subsidiary GameSpy Industries, Inc.  Unlicensed use or use in a 
// manner not expressly authorized by IGN or GameSpy is prohibited.

#ifndef _GAMEDATA_H_
#define _GAMEDATA_H_
#include <string>
#include <vector>
#include "../sc.h"

// AtlasKey: A game, player or team key
// Serves as the base object for keys.
class AtlasKey
{
protected:
	gsi_u16 m_KeyId;
public:
	AtlasKey(gsi_u16 keyId): m_KeyId(keyId)
	{
	}
	virtual ~AtlasKey()
	{
	}
	virtual SCResult InsertKeyValueIntoReport(SCReportPtr &report) = 0;
};

// AtlasIntKey
class AtlasIntKey : public AtlasKey
{
	gsi_i32 m_Value;

public:
	AtlasIntKey(gsi_u16 keyId, gsi_i32 val): AtlasKey(keyId), 
		m_Value(val)
	{
	}
	
	virtual SCResult InsertKeyValueIntoReport(SCReportPtr &report)
	{
		return scReportAddIntValue(report, m_KeyId, m_Value);
	}
};

class AtlasInt64Key : public AtlasKey
{
	gsi_i64 m_Value;

public:
	AtlasInt64Key(gsi_u16 keyId, gsi_i64 val): AtlasKey(keyId), 
		m_Value(val)
	{
	}
	virtual SCResult InsertKeyValueIntoReport(SCReportPtr &report)
	{
		return scReportAddInt64Value(report, m_KeyId, m_Value);
	}
};

class AtlasStringKey : public AtlasKey
{
	std::string m_Value;

public:
	AtlasStringKey(gsi_u16 keyId, const std::string &val): AtlasKey(keyId), 
		m_Value(val)
	{
	}
	virtual SCResult InsertKeyValueIntoReport(SCReportPtr &report)
	{
		return scReportAddStringValue(report, m_KeyId, _STR_CST(m_Value.c_str()));
	}
};

class AtlasByteKey : public AtlasKey 
{
	gsi_i8 m_Value;
	
public:
	AtlasByteKey(gsi_u16 keyId, gsi_i8 val): AtlasKey(keyId), 
		m_Value(val)
	{
	}
	virtual SCResult InsertKeyValueIntoReport(SCReportPtr &report)
	{
		return scReportAddByteValue(report, m_KeyId, m_Value);
	}
};

struct GameData
{
	int m_GameId;
	std::string m_GameName;
	std::string m_SecretKey;
	std::string m_AccessKey;
	int m_RulesetVersion;

	int m_NamespaceId;
	int m_PartnerId;
	std::string m_Email;
	std::string m_Profile;
	std::string m_Password;
	bool m_Matchless;
	
	// game stats!
	std::vector<AtlasKey *> m_GameKeys;

	// player stats!
	std::vector<AtlasKey *> m_PlayerKeys;
	
	~GameData()
	{
		while(m_PlayerKeys.begin() != m_PlayerKeys.end())
		{
			delete *m_PlayerKeys.begin();
			m_PlayerKeys.erase(m_PlayerKeys.begin());			
		}

		while(m_GameKeys.begin() != m_GameKeys.end())
		{
			delete *m_GameKeys.begin();
			m_GameKeys.erase(m_GameKeys.begin());			
		}
	}
};

#endif
