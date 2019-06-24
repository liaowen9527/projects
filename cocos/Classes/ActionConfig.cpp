#include "ActionConfig.h"

#include "json/stringbuffer.h"
#include "json/writer.h"
#include "platform/CCFileUtils.h"

#define NS_RAPIDJSON RAPIDJSON_NAMESPACE

using namespace cocos2d;

namespace act_conf {

	void JsonToVec2(const NS_RAPIDJSON::Value& val, cocos2d::Vec2& vec2, const std::string& prefix = "")
	{
		std::string x = prefix.empty() ? "x" : prefix + ".x";
		std::string y = prefix.empty() ? "y" : prefix + ".y";

		vec2.x = val[x.c_str()].GetFloat();
		vec2.y = val[y.c_str()].GetFloat();
	}

	void Vec2ToJson(const cocos2d::Vec2& vec2, NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator, const std::string& prefix = "")
	{
		std::string x = prefix.empty() ? "x" : prefix + ".x";
		std::string y = prefix.empty() ? "y" : prefix + ".y";

		NS_RAPIDJSON::Value keyX(x.c_str(), x.length(), allocator);
		NS_RAPIDJSON::Value keyY(y.c_str(), y.length(), allocator);
		
		val.AddMember(keyX, vec2.x, allocator);
		val.AddMember(keyY, vec2.y, allocator);
		
	}

	sprite::sprite()
		: m_tmCreate(0)
		, m_width(-1)
		, m_height(-1)
		, m_opacity(0)
		, m_angle(0)
	{

	}

	bool sprite::FromJson(const NS_RAPIDJSON::Value& val)
	{
		if (!val.IsObject())
		{
			return false;
		}

		m_tmCreate = val["create_time"].GetInt();
		m_strPath = val["res"].GetString();

		JsonToVec2(val, m_anchorPoint, "anchor_point");
		JsonToVec2(val, m_postion);

		m_width = val["width"].GetInt();
		m_height = val["height"].GetInt();
		m_angle = val["angle"].GetFloat();
		m_opacity = val["opacity"].GetInt();

		do
		{
			if (!val.HasMember("act"))
			{
				break;
			}

			const NS_RAPIDJSON::Value& lstAct = val["act"];
			if (!lstAct.IsArray() || lstAct.Size() <= 0)
			{
				break;
			}

			int nCount = lstAct.Size();
			for (unsigned int i = 0; i < nCount; i++)
			{
				const NS_RAPIDJSON::Value& actJson = lstAct[i];

				if (!actJson.HasMember("type"))
				{
					continue;
				}

				ActionPtr actPtr = action::Create(actJson["type"].GetString());
				if (nullptr == actPtr)
				{
					continue;
				}

				if (actPtr->FromJson(actJson))
				{
					m_vecAct.push_back(actPtr);
				}
			}

		} while (0);

		return true;
	}

	void sprite::ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator)
	{
		val.AddMember("create_time", m_tmCreate, allocator);

		val.AddMember("res", NS_RAPIDJSON::StringRef<char>(m_strPath.c_str()), allocator);

		Vec2ToJson(m_anchorPoint, val, allocator, "anchor_point");
		Vec2ToJson(m_postion, val, allocator);

		val.AddMember("angle", m_tmCreate, allocator);
		val.AddMember("width", m_width, allocator);
		val.AddMember("height", m_height, allocator);
		val.AddMember("opacity", m_opacity, allocator);

		NS_RAPIDJSON::Value lstAct(NS_RAPIDJSON::kArrayType);
		for (int i = 0; i < m_vecAct.size(); ++i)
		{
			ActionPtr actionPtr = m_vecAct[i];
			if (nullptr == actionPtr)
			{
				continue;
			}

			NS_RAPIDJSON::Value obj(NS_RAPIDJSON::kObjectType);
			actionPtr->ToJson(obj, allocator);
			lstAct.PushBack(obj, allocator);
		}

		val.AddMember("act", lstAct, allocator);

	}

	config::config()
		: m_width(1024)
		, m_height(768)
		, m_designWidth(1024)
		, m_designHeight(768)
		, m_fullScreen(false)
	{

	}

	config::~config()
	{

	}

	static config* s_config;
	config* config::Instance()
	{
		if (nullptr == s_config)
		{
			s_config = new config;
		}

		return s_config;
	}

	bool config::FromJson(const std::string& strPath)
	{
		bool bret = false;

		std::string strJson = FileUtils::getInstance()->getStringFromFile(strPath);

		NS_RAPIDJSON::Document doc;
		doc.Parse<NS_RAPIDJSON::kParseDefaultFlags>(strJson.c_str());

		if (doc.HasParseError())
		{
			CCLOG("UserManage::LoadUsers parse json error!");
			return false;
		}

		m_width = doc["width"].GetInt();
		m_height = doc["height"].GetInt();
		m_designWidth = doc["design.width"].GetInt();
		m_designHeight = doc["design.height"].GetInt();
		m_fullScreen = doc["full_screen"].GetBool();

		do
		{
			if (!doc.HasMember("sprites"))
			{
				break;
			}

			const NS_RAPIDJSON::Value& lstSprite = doc["sprites"];
			if (!lstSprite.IsArray() || lstSprite.Size() <= 0)
			{
				break;
			}

			int nCount = lstSprite.Size();
			for (unsigned int i = 0; i < nCount; i++)
			{
				const NS_RAPIDJSON::Value& spriteJson = lstSprite[i];
				SpritePtr spritePtr = std::make_shared<sprite>();
				if (spritePtr->FromJson(spriteJson))
				{
					m_mapSprite.insert(std::make_pair(spritePtr->m_tmCreate, spritePtr));
				}
			}
		} while (0);

		return true;
	}

	void config::ToJson(const std::string& strPath)
	{
		NS_RAPIDJSON::Document doc(NS_RAPIDJSON::kObjectType);
		NS_RAPIDJSON::Document::AllocatorType& allocator = doc.GetAllocator();

		NS_RAPIDJSON::Document lstSprite(NS_RAPIDJSON::kArrayType);

		for (auto itr = m_mapSprite.begin(); itr != m_mapSprite.end(); ++itr)
		{
			SpritePtr spritePtr = itr->second;
			if (nullptr == spritePtr)
			{
				continue;
			}

			NS_RAPIDJSON::Value obj(NS_RAPIDJSON::kObjectType);
			spritePtr->ToJson(obj, allocator);
			lstSprite.PushBack(obj, allocator);
		}

		doc.AddMember("width", m_width, allocator);
		doc.AddMember("height", m_height, allocator);
		doc.AddMember("design.width", m_designWidth, allocator);
		doc.AddMember("design.height", m_designHeight, allocator);
		doc.AddMember("full_screen", m_fullScreen, allocator);
		
		doc.AddMember("sprites", lstSprite, allocator);

		NS_RAPIDJSON::StringBuffer buffer;
		NS_RAPIDJSON::Writer<NS_RAPIDJSON::StringBuffer> writer(buffer);
		doc.Accept(writer);

		FILE* file = fopen(strPath.c_str(), "wb");
		if (file)
		{
			fputs(buffer.GetString(), file);
			fclose(file);
		}

	}

	ActionPtr action::Create(const std::string& type)
	{
		if (type == "move") return std::make_shared<move>();
		if (type == "rotate") return std::make_shared<rotate>();
		if (type == "sequence") return std::make_shared<sequence>();
		if (type == "scale") return std::make_shared<scale>();
		if (type == "opacity") return std::make_shared<fade>();
		
		return nullptr;
	}

	bool move::FromJson(const NS_RAPIDJSON::Value& val)
	{
		m_duration = val["duration"].GetInt();
		JsonToVec2(val, m_postion);

		return true;
	}

	void move::ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator)
	{
		val.AddMember("type", "move", allocator);
		val.AddMember("duration", m_duration, allocator);
		Vec2ToJson(m_postion, val, allocator);
	}

	bool rotate::FromJson(const NS_RAPIDJSON::Value& val)
	{
		m_duration = val["duration"].GetInt();
		m_angle = val["angle"].GetFloat();

		return true;
	}

	void rotate::ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator)
	{
		val.AddMember("type", "rotate", allocator);
		val.AddMember("duration", m_duration, allocator);
		val.AddMember("angle", m_angle, allocator);
	}

	bool sequence::FromJson(const NS_RAPIDJSON::Value& val)
	{
		return true;
	}

	void sequence::ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator)
	{
		val.AddMember("type", "sequence", allocator);
	}

	bool scale::FromJson(const NS_RAPIDJSON::Value& val)
	{
		m_duration = val["duration"].GetInt();
		m_width = val["width"].GetInt();
		m_height = val["height"].GetInt();

		return true;
	}

	void scale::ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator)
	{
		val.AddMember("type", "scale", allocator);
		val.AddMember("duration", m_duration, allocator);
		val.AddMember("width", m_width, allocator);
		val.AddMember("height", m_height, allocator);
	}

	bool fade::FromJson(const NS_RAPIDJSON::Value& val)
	{
		m_duration = val["duration"].GetInt();
		m_opacity = val["opacity"].GetFloat();

		return true;
	}

	void fade::ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator)
	{
		val.AddMember("type", "scale", allocator);
		val.AddMember("duration", m_duration, allocator);
		val.AddMember("opacity", m_opacity, allocator);
	}

}

