#include "stdafx.h"
#include <sstream>
#include "STSession.h"
#include "std/guid.h"

#define TEXT_TELNET "Telnet"
#define TEXT_SSH "SSH"


STSession::STSession(bool bNewId /*= false*/)
	: m_bLocal(false)
	, m_nPort(22)
	, m_bIsFolder(false)
{
	if (bNewId)
	{
		GenerateId();
	}
}

STSession::~STSession()
{

}

void STSession::GenerateId()
{
	lw_util::GUID mockGuid = lw_util::GUID(true);
	m_strId = mockGuid.ToString();
}

void STSession::ToJson(Json::Value& json) const
{
	json["id"] = m_strId;
	json["parent_id"] = m_strParentId;
	json["is_folder"] = m_bIsFolder;
	json["is_local"] = m_bLocal;
	json["name"] = m_strName;
	json["host"] = m_strHost;
	json["ip"] = m_strIp;
	json["port"] = m_nPort;
	json["protocol"] = Protocol2Str(m_emProtocol);
	json["user"] = m_strUser;
	json["password"] = m_strPassword;
	json["private_key"] = m_strPrivateKey;
	json["keyfile_name"] = m_strKeyName;
}

bool STSession::FromJson(const Json::Value& json)
{
	try
	{
		m_strId = json["id"].asString();
		m_strParentId = json["parent_id"].asString();
		if (json.isMember("is_folder"))
		{
			m_bIsFolder = json["is_folder"].asBool();
		}
		m_bLocal = json["is_local"].asBool();
		m_strName = json["name"].asString();
		m_strHost = json["host"].asString();
		m_strIp = json["ip"].asString();
		m_nPort = json["port"].asInt();
		m_emProtocol = Str2Protocol(json["protocol"].asString());
		m_strUser = json["user"].asString();
		m_strPassword = json["password"].asString();

		if (json.isMember("private_key"))
		{
			m_strPrivateKey = json["private_key"].asString();
		}
		if (json.isMember("keyfile_name"))
		{
			m_strKeyName = json["keyfile_name"].asBool();
		}

		return true;
	}
	catch (std::exception& ex)
	{
		
	}
	catch (...)
	{
	}

	return false;
}

void STSession::ToString(std::string& str) const
{
	Json::Value json;
	ToJson(json);

	str = json.toStyledString();
}

bool STSession::FromString(const std::string& str)
{
	Json::Value json;
	Json::Reader reader;
	if (!reader.parse(str, json))
	{
		return false;
	}
	
	return FromJson(json);
}

std::string STSession::Protocol2Str(EnumProtocol emType)
{
	if (emType == emTelnet)
	{
		return TEXT_TELNET;
	}

	return TEXT_SSH;
}

STSession::EnumProtocol STSession::Str2Protocol(const std::string& str)
{
	if (str == TEXT_TELNET)
	{
		return emTelnet;
	}

	return emSsh;
}

const std::string& STSession::GetId() const
{
	return m_strId;
}

void STSession::SetId(const std::string& str)
{
	m_strId = str;
}

const std::string& STSession::GetParentId() const
{
	return m_strParentId;
}

void STSession::SetParentId(const std::string& str)
{
	m_strParentId = str;
}

bool STSession::IsFolder() const
{
	return m_bIsFolder;
}

void STSession::SetFolder(bool bValue)
{
	m_bIsFolder = bValue;
}

bool STSession::IsLocal() const
{
	return m_bLocal;
}

void STSession::SetLocal(bool bValue)
{
	m_bLocal = bValue;
}

const std::string& STSession::GetName() const
{
	return m_strName;
}

void STSession::SetName(const std::string& strName)
{
	m_strName = strName;
}

void STSession::SetName(const std::string& strName, int nIndex)
{
	if (nIndex == 0)
	{
		m_strName = strName;
		return;
	}

	std::stringstream ss;
	ss << strName.c_str() << "(" << nIndex << ")";

	m_strName = ss.str();
}

bool STSession::SplitName(std::string& strName, int& nIndex) const
{
	do 
	{
		int nPos2 = m_strName.find_last_of(")");
		if (nPos2 == std::string::npos)
		{
			break;
		}

		int nPos1 = m_strName.find_last_of("(", nPos2);
		if (nPos2 == std::string::npos)
		{
			break;
		}

		int nStart = nPos1 + 1;
		int nCount = nPos2 - nStart;
		if (nCount < 0)
		{
			break;
		}

		std::string strIndex = m_strName.substr(nStart, nCount);
		if (strIndex.find_first_not_of("0123456789") != std::string::npos)
		{
			break;
		}
		/*if (!std::all_of(strIndex.begin(), strIndex.end(), isdigit))
		{
			break;
		}*/

		strName = m_strName.substr(0, nPos1);
		nIndex = atoi(strIndex.c_str());
		return true;
	} while (0);
	
	strName = m_strName;
	return false;
}

bool STSession::SplitName(const std::string& strBy, std::string& strName, int& nIndex) const
{
	if (m_strName == strBy)
	{
		strName = strName;
		nIndex = 0;
		return true;
	}
	
	SplitName(strName, nIndex);
	if (strName != strBy)
	{
		return false;
	}

	return true;
}

std::string STSession::GetHost() const
{
	return m_strHost;
}

void STSession::SetHost(const std::string& strHost)
{
	m_strHost = strHost;
}

std::string STSession::GetIp() const
{
	return m_strIp;
}

void STSession::SetIp(const std::string& str)
{
	m_strIp = str;
}

int STSession::GetPort() const
{
	return m_nPort;
}

void STSession::SetPort(int nPort)
{
	m_nPort = nPort;
}

STSession::EnumProtocol STSession::GetProtocol() const
{
	return m_emProtocol;
}

void STSession::SetProtocol(EnumProtocol emType)
{
	m_emProtocol = emType;
}

std::string STSession::GetUser() const
{
	return m_strUser;
}

void STSession::SetUser(const std::string& strUser)
{
	m_strUser = strUser;
}

std::string STSession::GetPassword() const
{
	return m_strPassword;
}

void STSession::SetPassword(const std::string& strPassword)
{
	m_strPassword = strPassword;
}

void STSession::GetPrivateKey(std::string& strFileName, std::string& strContent) const
{
	strFileName = m_strKeyName;
	strContent = m_strPrivateKey;
}

void STSession::SetPrivateKey(const std::string& strFileName, const std::string& strContent)
{
	m_strKeyName = strFileName;
	m_strPrivateKey = strContent;
}

STSessionTree::STSessionTree()
	: m_node(nullptr)
{

}

STSessionTree::~STSessionTree()
{
	for (int i = 0; i < m_children.size(); ++i)
	{
		delete m_children[i];
		m_children[i] = nullptr;
	}
}

void STSessionTree::BuildTree(const std::unordered_map<std::string, STSession>& mapSession)
{
	std::map<std::string, STSessionTree*> mapNode;
	for (auto itr = mapSession.begin(); itr != mapSession.end(); ++itr)
	{
		const std::string& strId = itr->first;
		const STSession& session = itr->second;

		STSessionTree* pNode = new STSessionTree;
		pNode->m_node = (STSession*)(&session);
		mapNode.insert(std::make_pair(strId, pNode));
	}

	for (auto itr = mapNode.begin(); itr != mapNode.end(); ++itr)
	{
		const std::string& strId = itr->first;
		STSessionTree* pNode = itr->second;
		const std::string& strParentId = pNode->m_node->GetParentId();

		auto itrParent = mapNode.find(strParentId);
		if (itrParent == mapNode.end())
		{
			m_children.push_back(pNode);
		}
		else
		{
			STSessionTree* pParentNode = itrParent->second;
			pParentNode->m_children.push_back(pNode);
		}
	}
}

STSessionTree* STSessionTree::FindNode(const std::string& strId)
{
	if (m_node && m_node->GetId() == strId)
	{
		return this;
	}

	for (STSessionTree* pChild : m_children)
	{
		if (nullptr == pChild->m_node)
		{
			return nullptr;
		}

		STSessionTree* pRetNode = pChild->FindNode(strId);
		if (pRetNode)
		{
			return pRetNode;
		}
	}

	return nullptr;
}

std::vector<STSessionTree*> STSessionTree::GetAllChild()
{
	std::vector<STSessionTree*> vecChild = m_children;

	for (STSessionTree* pChild : m_children)
	{
		std::vector<STSessionTree*> vecTemp = pChild->GetAllChild();
		vecChild.insert(vecChild.end(), vecTemp.begin(), vecTemp.end());
	}

	return vecChild;
}
