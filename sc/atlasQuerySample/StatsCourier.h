// Copyright (c) IGN Entertainment, Inc.  All rights reserved.  
// This software is made available only pursuant to certain license terms offered
// by IGN or its subsidiary GameSpy Industries, Inc.  Unlicensed use or use in a 
// manner not expressly authorized by IGN or GameSpy is prohibited.

#ifndef _STATSCOURIER_H_
#define _STATSCOURIER_H_

#include <string>

#include "../../webservices/AuthService.h"
#include "../sc.h"

#include "QueryData.h"

class StatsCourier
{
	enum State
	{
		GET_AUTH_CERT,
		GETTING_AUTH_CERT,
		RUN_QUERY1,
		RUNNING_QUERY1,
		RUN_QUERY2,
		RUNNING_QUERY2,
		RUN_QUERY3,
		RUNNING_QUERY3,
		RUN_QUERY4,
		RUNNING_QUERY4,
		QUERIES_COMPLETE,
		// The state to cause the app to gracefully shutdown
		// if errors occur
		ERROR_OCCURED
	};
	const QueryData	&m_Data;
	SCInterfacePtr		    m_AtlasInterface;
	State		m_State;
	SCQueryParameterListPtr m_QueryParamsList;
	GSLoginCertificate	    m_LoginCert;
	GSLoginPrivateData	    m_LoginPrivData;

	State GetState() { return m_State; }

	// This code prevents copying of StatsCourior objects. We don't want to have copies of the same object.
	StatsCourier & operator=(const StatsCourier &); 
	
public:
	StatsCourier(const QueryData &data);
	~StatsCourier();
	
	void ObtainAuthServiceCertificate(const std::string &email, const std::string &profile, 
									  const std::string &password, int partnerSpaceId, int nameSpaceId);	
	void ObtainAuthServiceCertificate(const std::string &partnerChallenge, const std::string &remoteAuthToken, 
									  int partnerSpaceId, int nameSpaceId);
	void RunQuery1();
	void RunQuery2();
	void RunQuery3();
	void RunQuery4();
	
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
				m_State = RUN_QUERY1;
				break;
			}
		case RUN_QUERY1:
			{
				m_State = RUNNING_QUERY1;
				break;
			}
		case RUNNING_QUERY1:
			{
				m_State = RUN_QUERY2;
				break;
			}
		case RUN_QUERY2:
			{
				m_State = RUNNING_QUERY2;
				break;
			}
		case RUNNING_QUERY2:
			{
				m_State = RUN_QUERY3;
				break;
			}
		case RUN_QUERY3:
			{
				m_State = RUNNING_QUERY3;
				break;
			}
		case RUNNING_QUERY3:
			{
				m_State = RUN_QUERY4;
				break;
			}
		case RUN_QUERY4:
			{
				m_State = RUNNING_QUERY4;
				break;
			}
		case RUNNING_QUERY4:
			{
				m_State = QUERIES_COMPLETE;
				break;
			}
		default:
			{

			}
		}
	}
};

#endif
