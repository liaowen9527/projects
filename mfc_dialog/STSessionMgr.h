#pragma once
#include <unordered_map>
#include <functional>
#include "STSession.h"

class CppSQLite3DB;

class STSessionMgr
{
public:
	STSessionMgr();
	~STSessionMgr();

	std::string GetDBPath();
	void SetDBPath(const std::string& strPath);

public:
	bool LoadAll();

	void GetAll(std::unordered_map<std::string, STSession>& mapSession);
	void Add(const STSession& session, bool bSaveDB = true);
	void Delete(const std::string& strId);
	bool Update(const STSession& session);

	int MaxIndex(const std::string& name);
	bool FindByName(const std::string& name);
	bool FindByName(const std::string& strParentId, const std::string& name);

protected:
	bool QueryFromDB(std::function<bool(CppSQLite3DB* pSqlite3DB)> func);
	bool SaveToDB(std::function<bool(CppSQLite3DB* pSqlite3DB)> func);

	bool InsertToDB(const STSession& session);
	bool DeleteFromDB(const std::string& strId);
	bool UpdateToDB(const STSession& session);

	std::string GetTable();
	std::string GetCreateSql();
	std::string GetInsertSql();

protected:
	std::unordered_map<std::string, STSession> m_mapSession;
	std::string m_strFilePath;
};