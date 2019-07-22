#pragma once
#include <string>
#include <unordered_map>
#include "json/json.h"

class STSession
{
public:
	enum EnumProtocol
	{
		emTelnet = 0,
		emSsh,
        emSshKey,
	};

public:
	STSession(bool bNewId = false);
	~STSession();

	void GenerateId();

	void ToJson(Json::Value& json) const;
	bool FromJson(const Json::Value& json);

	void ToString(std::string& str) const;
	bool FromString(const std::string& str);

	static std::string Protocol2Str(EnumProtocol emType);
	static EnumProtocol Str2Protocol(const std::string& str);

public:
	const std::string& GetId() const;
	void SetId(const std::string& str);

	const std::string& GetParentId() const;
	void SetParentId(const std::string& str);

	bool IsFolder() const;
	void SetFolder(bool bValue);

	bool IsLocal() const;
	void SetLocal(bool bValue);

	const std::string& GetName() const;
	void SetName(const std::string& strName);
	void SetName(const std::string& strName, int nIndex);
	bool SplitName(std::string& strName, int& nIndex) const;
	bool SplitName(const std::string& strBy, std::string& strName, int& nIndex) const;

	std::string GetHost() const;
	void SetHost(const std::string& strHost);

	std::string GetIp() const;
	void SetIp(const std::string& strHost);

	int GetPort() const;
	void SetPort(int nPort);

	EnumProtocol GetProtocol() const;
	void SetProtocol(EnumProtocol emType);

	std::string GetUser() const;
	void SetUser(const std::string& strUser);

	std::string GetPassword() const;
	void SetPassword(const std::string& strPassword);

	void GetPrivateKey(std::string& strFileName, std::string& strContent) const;
	void SetPrivateKey(const std::string& strFileName, const std::string& strContent);

protected:
	std::string m_strId;
	std::string m_strParentId;
	bool m_bIsFolder;
	bool m_bLocal;
	
	std::string m_strName;

	std::string m_strHost;
	std::string m_strIp;
	int m_nPort;
	EnumProtocol m_emProtocol;
	std::string m_strUser;
	std::string m_strPassword;

	std::string m_strKeyName;
	std::string m_strPrivateKey;
};


class STSessionTree
{
public:
	STSessionTree();
	~STSessionTree();

public:
	void BuildTree(const std::unordered_map<std::string, STSession>& mapSession);
	STSessionTree* FindNode(const std::string& strId);

	std::vector<STSessionTree*> GetAllChild();

public:
	std::vector<STSessionTree*> m_children;
	STSession* m_node;
};