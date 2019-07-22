#include "stdafx.h"
#include <functional>
#include <algorithm>
#include "STSessionMgr.h"
#include "sqlite/CppSQLite3.h"
#include "filesystem/file_system.h"

STSessionMgr::STSessionMgr()
{

}

STSessionMgr::~STSessionMgr()
{

}

bool STSessionMgr::LoadAll()
{
	m_mapSession.clear();

	auto func = [&](CppSQLite3DB* pSqlite3DB) {
		std::string strSql = "select * from " + GetTable();
		CppSQLite3Statement sqlstat = pSqlite3DB->compileStatement(strSql.c_str());

		CppSQLite3Query	query = sqlstat.execQuery();

		sqlstat.reset();
		while (!query.eof())
		{
			const char* strData = query.getStringField("data");
			if (strData && strlen(strData) > 0)
			{
				STSession session;
				if (session.FromString(strData))
				{
					Add(session, false);
				}
			}

			query.nextRow();
		}

		return true;
	};

	return QueryFromDB(func);
}

void STSessionMgr::GetAll(std::unordered_map<std::string, STSession>& mapSession)
{
	mapSession = m_mapSession;
}

void STSessionMgr::Add(const STSession& session, bool bSaveDB/* = true*/)
{
	m_mapSession.insert(std::make_pair(session.GetId(), session));
	
	if (bSaveDB)
	{
		InsertToDB(session);
	}
}

void STSessionMgr::Delete(const std::string& strId)
{
	m_mapSession.erase(strId);

	DeleteFromDB(strId);
}

bool STSessionMgr::Update(const STSession& session)
{
	std::string strId = session.GetId();
	if (m_mapSession.find(strId) == m_mapSession.end())
	{
		return false;
	}

	m_mapSession[strId] = session;

	UpdateToDB(session);

	return true;
}

int STSessionMgr::MaxIndex(const std::string& name)
{
	int nIndex = -1;
	for (auto itr = m_mapSession.begin(); itr != m_mapSession.end(); ++itr)
	{
		STSession& session = itr->second;

		std::string strTemp;
		int nTemp = 0;
		if (session.SplitName(name, strTemp, nTemp))
		{
			nIndex = std::max<int>(nIndex, nTemp);
		}
	}

	return nIndex;
}

bool STSessionMgr::FindByName(const std::string& name)
{
	for (auto itr = m_mapSession.begin(); itr != m_mapSession.end(); ++itr)
	{
		STSession& session = itr->second;
		if (session.GetName() == name)
		{
			return true;
		}
	}

	return false;
}

bool STSessionMgr::FindByName(const std::string& strParentId, const std::string& name)
{
	for (auto itr = m_mapSession.begin(); itr != m_mapSession.end(); ++itr)
	{
		STSession& session = itr->second;
		if (session.GetName() == name && session.GetParentId() == strParentId)
		{
			return true;
		}
	}

	return false;
}

bool STSessionMgr::QueryFromDB(std::function<bool(CppSQLite3DB* pSqlite3DB)> func)
{
	CppSQLite3DB Sqlite3DB;
	try
	{
		std::string strDbPath = GetDBPath();
		if (!lw_util::FileSystem::Exists(strDbPath))
		{
			return false;
		}
		std::string strTable = GetTable();

		Sqlite3DB.open(strDbPath.c_str());
		if (!Sqlite3DB.tableExists(strTable.c_str()))
		{
			return false;
		}

		return func(&Sqlite3DB);
	}
	catch (CppSQLite3Exception&  e)
	{

	}
	catch (std::exception& ex)
	{

	}
	catch (...)
	{

	}

	try
	{
		Sqlite3DB.execDML("rollback transaction;");
	}
	catch (CppSQLite3Exception&  e)
	{

	}

	return false;
}

bool STSessionMgr::SaveToDB(std::function<bool(CppSQLite3DB* pSqlite3DB)> func)
{
	CppSQLite3DB Sqlite3DB;
	try
	{
		std::string strDbPath = GetDBPath();
		std::string strTable = GetTable();

		Sqlite3DB.open(strDbPath.c_str());
		if (!Sqlite3DB.tableExists(strTable.c_str()))
		{
			std::string strSql = GetCreateSql();
			Sqlite3DB.getTable(strSql.c_str());
		}

		Sqlite3DB.execDML("begin transaction;");

		bool bRet = func(&Sqlite3DB);

		Sqlite3DB.execDML("commit transaction;");

		return bRet;
	}
	catch (CppSQLite3Exception&  e)
	{

	}
	catch (std::exception& ex)
	{

	}
	catch (...)
	{

	}

	try
	{
		Sqlite3DB.execDML("rollback transaction;");
	}
	catch (CppSQLite3Exception&  e)
	{

	}

	return false;
}

bool STSessionMgr::InsertToDB(const STSession& session)
{
	auto func = [&](CppSQLite3DB* pSqlite3DB) {
		std::string strInsertSql = GetInsertSql();
		CppSQLite3Statement sqlstat = pSqlite3DB->compileStatement(strInsertSql.c_str());

		sqlstat.bind(1, session.GetId().c_str());

		Json::Value json;
		session.ToJson(json);
		sqlstat.bind(2, json.toStyledString().c_str());

		sqlstat.execDML();

		return true;
	};

	return SaveToDB(func);
}

bool STSessionMgr::DeleteFromDB(const std::string& strId)
{
	auto func = [&](CppSQLite3DB* pSqlite3DB) {
		std::string strSql = "delete from " + GetTable() + " where _id=?";
		CppSQLite3Statement sqlstat = pSqlite3DB->compileStatement(strSql.c_str());

		sqlstat.bind(1, strId.c_str());

		sqlstat.execDML();

		return true;
	};

	return SaveToDB(func);
}

bool STSessionMgr::UpdateToDB(const STSession& session)
{
	auto func = [&](CppSQLite3DB* pSqlite3DB) {
		std::string strSql = "update " + GetTable() + " set data=? where _id=?;";
		CppSQLite3Statement sqlstat = pSqlite3DB->compileStatement(strSql.c_str());

		Json::Value json;
		session.ToJson(json);
		sqlstat.bind(1, json.toStyledString().c_str());

		sqlstat.bind(2, session.GetId().c_str());

		sqlstat.execDML();

		return true;
	};

	return SaveToDB(func);
}

std::string STSessionMgr::GetDBPath()
{
	return m_strFilePath;
}

void STSessionMgr::SetDBPath(const std::string& strPath)
{
	m_strFilePath = strPath;
}

std::string STSessionMgr::GetTable()
{
	return "SESSION";
}

std::string STSessionMgr::GetInsertSql()
{
	return "insert into " + GetTable() + "(_id, data) values(?,?);";
}

std::string STSessionMgr::GetCreateSql()
{
	return "CREATE TABLE " + GetTable() + "(_id TEXT PRIMARY KEY NOT NULL, data TEXT NOT NULL);";
}
