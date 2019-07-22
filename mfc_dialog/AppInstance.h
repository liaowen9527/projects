#pragma once
#include "destination.h"

class STSessionMgr;
class STSession;
class Destination;

class AppInstance
{
public:
	AppInstance();
	~AppInstance();

	static AppInstance* Instance();

public:
	void InitEnv();

	STSessionMgr* GetSessionMgr();

public:
	lw_live::DestinationPtr GetDestination(STSession* pSession);

protected:
	STSessionMgr* m_pSessionMgr;
};