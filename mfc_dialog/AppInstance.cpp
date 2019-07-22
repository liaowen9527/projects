#include "AppInstance.h"
#include "client_factory.h"
#include "STSessionMgr.h"
#include "ssh_client_param.h"
#include "telnet_client_param.h"

using namespace lw_live;
using namespace lw_client;

AppInstance::AppInstance()
{
	m_pSessionMgr = new STSessionMgr;
}

AppInstance::~AppInstance()
{

}

static AppInstance* s_instance = nullptr;
AppInstance* AppInstance::Instance()
{
	if (nullptr == s_instance)
	{
		s_instance = new AppInstance;
	}

	return s_instance;
}

void AppInstance::InitEnv()
{
	lw_client::ClientFactory::InitEnv();
}

STSessionMgr* AppInstance::GetSessionMgr()
{
	return m_pSessionMgr;
}

DestinationPtr AppInstance::GetDestination(STSession* pSession)
{
	DestinationPtr destPtr = std::make_shared<lw_live::Destination>();
	lw_util::Tree<LiveParamPtr>& clients = destPtr->m_clients;

	STSession::EnumProtocol emProtocol = pSession->GetProtocol();
	if ((emProtocol == STSession::emSsh) || (emProtocol == STSession::emSshKey))
	{
		//10.10.5.141
		SshClientParamPtr clientParam = std::make_shared<SshClientParam>(pSession->GetHost(), pSession->GetPort());
		clientParam->SetSshVersion(ssh1);
		clientParam->SetUsername(pSession->GetUser());
		clientParam->SetPassword(pSession->GetPassword());

		SshClientParamPtr clientParam2 = std::make_shared<SshClientParam>(pSession->GetHost(), pSession->GetPort());
		*clientParam2 = *clientParam;
		clientParam2->SetSshVersion(ssh2);

		LiveParamPtr linuxSsh1 = std::make_shared<LiveParam>(clientParam);
		LiveParamPtr linuxSsh2 = std::make_shared<LiveParam>(clientParam2);
		clients.InsertItem(linuxSsh2);
		clients.InsertItem(linuxSsh1);
	}
	else
	{
		TelnetClientParamPtr clientParam = std::make_shared<TelnetClientParam>(pSession->GetHost(), pSession->GetPort());
		clientParam->SetUsername(pSession->GetUser());
		clientParam->SetPassword(pSession->GetPassword());

		clients.InsertItem(std::make_shared<LiveParam>(clientParam));
	}

	return destPtr;
}
