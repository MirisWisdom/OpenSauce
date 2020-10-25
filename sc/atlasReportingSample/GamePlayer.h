// Copyright (c) IGN Entertainment, Inc.  All rights reserved.  
// This software is made available only pursuant to certain license terms offered
// by IGN or its subsidiary GameSpy Industries, Inc.  Unlicensed use or use in a 
// manner not expressly authorized by IGN or GameSpy is prohibited.

#ifndef _GAMEHOST_H_
#define _GAMEHOST_H_

#include <string>

#include "../../webservices/AuthService.h"
#include "../sc.h"

#include "GameData.h"

class GamePlayer
{
	enum State
	{
		GET_AUTH_CERT,
		GETTING_AUTH_CERT,
		CREATE_SESSION,
		CREATING_SESSION,
		SET_REPORT_INTENTION,
		SETTING_REPORT_INTENTION,
		SUBMIT_REPORT,
		SUBMITTING_REPORT,
		REPORT_COMPLETE,
		// The state to cause the app to gracefully shutdown
		// if errors occur
		ERROR_OCCURED
	};
	SCInterfacePtr		m_AtlasInterface;
	SCReportPtr			m_AtlasReport;
	GSLoginCertificate	m_LoginCert;
	GSLoginPrivateData	m_LoginPrivData;
	int			m_GameId;
	State		m_State;
	const GameData	&m_Data;
	
	State GetState() { return m_State; }

	GamePlayer & operator=(const GamePlayer &); // This code prevents copying because players are unique.
public:
	GamePlayer(const GameData &data);
	~GamePlayer();
	
	void ObtainAuthServiceCertificate(const std::string &email, const std::string &profile, 
									  const std::string &password, int partnerSpaceId, int nameSpaceId);	
	void ObtainAuthServiceCertificate(const std::string &partnerChallenge, const std::string &remoteAuthToken, 
									  int partnerSpaceId, int nameSpaceId);
	//VerifyClientAuthServiceCert();  // future additions
	//SendAtlasSessionId();			  // future additions
	void CreateAtlasSession();
	void SetReportIntention();
	void SubmitAtlasReport();
	
	void SetAuthServiceData(const GSLoginCertificate &validCert, const GSLoginPrivateData &validPrivData)
	{
		m_LoginCert = validCert;
		m_LoginPrivData = validPrivData;
	}

	bool Run();
	void AdvanceState(bool error = false)
	{
		if (error)
		{
			m_State = ERROR_OCCURED;
			return;
		}

		switch(m_State)
		{
		case GET_AUTH_CERT:
			{
				m_State = GETTING_AUTH_CERT;
				break;
			}
		case GETTING_AUTH_CERT:
			{
				m_State = CREATE_SESSION;
				break;
			}
		case CREATE_SESSION:
			{
				m_State = CREATING_SESSION;
				break;
			}
		case CREATING_SESSION:
			{
				m_State = SET_REPORT_INTENTION;
				break;
			}
		case SET_REPORT_INTENTION:
			{
				m_State = SETTING_REPORT_INTENTION;
				break;
			}
		case SETTING_REPORT_INTENTION:
			{
				m_State = SUBMIT_REPORT;
				break;
			}
		case SUBMIT_REPORT:
			{
				m_State = SUBMITTING_REPORT;
				break;
			}
		case SUBMITTING_REPORT:
			{
				m_State = REPORT_COMPLETE;
				break;
			}
		default:
			{

			}
		}
	}
};

#endif