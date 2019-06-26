#include "ActionConfig.h"

#include "json/stringbuffer.h"
#include "json/writer.h"
#include "platform/CCFileUtils.h"
#include "compare.cpp"

#define NS_RAPIDJSON RAPIDJSON_NAMESPACE

using namespace cocos2d;

namespace act_conf {

	sprite s_default_sprite;

	void JsonToVec2(const NS_RAPIDJSON::Value& val, cocos2d::Vec2& vec2, const std::string& prefix = "")
	{
		std::string x = prefix.empty() ? "x" : prefix + ".x";
		std::string y = prefix.empty() ? "y" : prefix + ".y";

		if (val.HasMember(x.c_str())) vec2.x = val[x.c_str()].GetFloat();
		if (val.HasMember(y.c_str())) vec2.y = val[y.c_str()].GetFloat();
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

	blur::blur()
	{

	}

	bool blur::FromJson(const NS_RAPIDJSON::Value& val)
	{
		m_strShaderPath = val["shader"].GetString();
		m_radius = val["radius"].GetFloat();
		m_num = val["num"].GetFloat();

		return true;
	}

	void blur::ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator)
	{
		val.AddMember("shader", NS_RAPIDJSON::StringRef<char>(m_strShaderPath.c_str()), allocator);
		val.AddMember("radius", m_radius, allocator);
		val.AddMember("radius", m_num, allocator);
	}

	fitsize::fitsize()
		: m_enable(true)
		, m_scale(1.2f)
	{

	}

	bool fitsize::FromJson(const NS_RAPIDJSON::Value& val)
	{
		if (val.HasMember("fitsize")) m_enable = val["fitsize"].GetBool();
		if (val.HasMember("fitsize.scale")) m_scale = val["fitsize.scale"].GetFloat();
		return true;
	}

	void fitsize::ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator)
	{
		if (!compare::is_equal(m_enable, s_default_sprite.m_fitsize.m_enable)) val.AddMember("fitsize", m_enable, allocator);
		if (!compare::is_equal(m_scale, s_default_sprite.m_fitsize.m_scale)) val.AddMember("fitsize.scale", m_scale, allocator);
	}

	sprite::sprite()
		: m_tmCreate(0)
		, m_width(-1)
		, m_height(-1)
		, m_opacity(255)
		, m_angle(0)
		, m_blur(-1)
		, m_zorder(1)
	{

	}

	bool sprite::FromJson(const NS_RAPIDJSON::Value& val)
	{
		if (!val.IsObject())
		{
			return false;
		}

		if (val.HasMember("create_time")) m_tmCreate = val["create_time"].GetInt();
		m_strPath = val["res"].GetString();

		JsonToVec2(val, m_anchorPoint, "anchor_point");
		JsonToVec2(val, m_postion);

		if (val.HasMember("width")) m_width = val["width"].GetInt();
		if (val.HasMember("height")) m_height = val["height"].GetInt();
		if (val.HasMember("angle")) m_angle = val["angle"].GetFloat();
		if (val.HasMember("opacity")) m_opacity = val["opacity"].GetInt();
		if (val.HasMember("zorder")) m_zorder = val["zorder"].GetInt();

		LoadAction(val);
		LoadBlur(val);

		m_fitsize.FromJson(val);
		
		return true;
	}

	void sprite::ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator)
	{
		if(!compare::is_equal(m_tmCreate, s_default_sprite.m_tmCreate)) val.AddMember("create_time", m_tmCreate, allocator);
		if (!compare::is_equal(m_strPath, s_default_sprite.m_strPath)) val.AddMember("res", NS_RAPIDJSON::StringRef<char>(m_strPath.c_str()), allocator);

		if (!compare::is_equal(m_anchorPoint, s_default_sprite.m_anchorPoint)) Vec2ToJson(m_anchorPoint, val, allocator, "anchor_point");
		if (!compare::is_equal(m_postion, s_default_sprite.m_postion)) Vec2ToJson(m_postion, val, allocator);

		if (!compare::is_equal(m_angle, s_default_sprite.m_angle)) val.AddMember("angle", m_angle, allocator);
		if (!compare::is_equal(m_width, s_default_sprite.m_width)) val.AddMember("width", m_width, allocator);
		if (!compare::is_equal(m_height, s_default_sprite.m_height)) val.AddMember("height", m_height, allocator);
		if (!compare::is_equal(m_opacity, s_default_sprite.m_opacity)) val.AddMember("opacity", m_opacity, allocator);
		if (!compare::is_equal(m_zorder, s_default_sprite.m_zorder)) val.AddMember("zorder", m_zorder, allocator);

		if (m_action)
		{
			NS_RAPIDJSON::Value actJson(NS_RAPIDJSON::kObjectType);
			m_action->ToJson(actJson, allocator);

			val.AddMember("act", actJson, allocator);
		}
		
		if (!compare::is_equal(m_blur, s_default_sprite.m_blur)) val.AddMember("blur", m_blur, allocator);

		m_fitsize.ToJson(val, allocator);
	}

	void sprite::LoadAction(const NS_RAPIDJSON::Value& val)
	{
		if (!val.HasMember("act"))
		{
			return;
		}
		
		const NS_RAPIDJSON::Value& actJson = val["act"];

		if (!actJson.HasMember("type"))
		{
			return;
		}

		m_action = action::Create(actJson["type"].GetString());
		m_action->FromJson(actJson);
	}

	void sprite::LoadBlur(const NS_RAPIDJSON::Value& val)
	{
		if (!val.HasMember("blur"))
		{
			return;
		}

		const NS_RAPIDJSON::Value& valJson = val["blur"];

		m_blur = valJson.GetInt();
	}

	bool layer::FromJson(const NS_RAPIDJSON::Value& val)
	{
		m_tmCreate = val["create_time"].GetInt();
		LoadSprites(val);
		return true;
	}

	void layer::ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator)
	{
		val.AddMember("create_time", m_tmCreate, allocator);
		SaveSprites(val, allocator);
	}

	void layer::LoadSprites(const NS_RAPIDJSON::Value& val)
	{
		if (!val.HasMember("sprites"))
		{
			return;
		}

		const NS_RAPIDJSON::Value& arrJson = val["sprites"];
		if (!arrJson.IsArray() || arrJson.Size() <= 0)
		{
			return;
		}

		int nCount = arrJson.Size();
		for (unsigned int i = 0; i < nCount; i++)
		{
			const NS_RAPIDJSON::Value& valJson = arrJson[i];
			SpritePtr spritePtr = std::make_shared<sprite>();
			if (spritePtr->FromJson(valJson))
			{
				m_mapSprite.insert(std::make_pair(spritePtr->m_tmCreate, spritePtr));
			}
		}
	}

	void layer::SaveSprites(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator)
	{
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
		val.AddMember("sprites", lstSprite, allocator);
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

		LoadBlurs(doc);
		LoadLayers(doc);

		return true;
	}

	void config::ToJson(const std::string& strPath)
	{
		NS_RAPIDJSON::Document doc(NS_RAPIDJSON::kObjectType);
		NS_RAPIDJSON::Document::AllocatorType& allocator = doc.GetAllocator();

		doc.AddMember("width", m_width, allocator);
		doc.AddMember("height", m_height, allocator);
		doc.AddMember("design.width", m_designWidth, allocator);
		doc.AddMember("design.height", m_designHeight, allocator);
		doc.AddMember("full_screen", m_fullScreen, allocator);

		SaveBlurs(doc, allocator);
		SaveLayers(doc, allocator);

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

	act_conf::BlurPtr config::GetBlur(int nIndex)
	{
		if (0 <= nIndex && nIndex < m_vecBlur.size())
		{
			return m_vecBlur[nIndex];
		}

		return nullptr;
	}

	void config::LoadBlurs(const NS_RAPIDJSON::Value& val)
	{
		if (!val.HasMember("blurs"))
		{
			return;
		}

		const NS_RAPIDJSON::Value& arrJson = val["blurs"];
		if (!arrJson.IsArray() || arrJson.Size() <= 0)
		{
			return;
		}

		int nCount = arrJson.Size();
		for (unsigned int i = 0; i < nCount; i++)
		{
			const NS_RAPIDJSON::Value& valJson = arrJson[i];
			BlurPtr blurPtr = std::make_shared<blur>();
			if (blurPtr->FromJson(valJson))
			{
				m_vecBlur.push_back(blurPtr);
			}
		}
	}

	void config::SaveBlurs(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator)
	{
		NS_RAPIDJSON::Document arrJson(NS_RAPIDJSON::kArrayType);

		for (BlurPtr blurPtr : m_vecBlur)
		{
			if (nullptr == blurPtr)
			{
				continue;
			}

			NS_RAPIDJSON::Value obj(NS_RAPIDJSON::kObjectType);
			blurPtr->ToJson(obj, allocator);
			arrJson.PushBack(obj, allocator);
		}
		val.AddMember("blurs", arrJson, allocator);
	}

	void config::LoadLayers(const NS_RAPIDJSON::Value& val)
	{
		if (!val.HasMember("layers"))
		{
			return;
		}

		const NS_RAPIDJSON::Value& arrJson = val["layers"];
		if (!arrJson.IsArray() || arrJson.Size() <= 0)
		{
			return;
		}

		int nCount = arrJson.Size();
		for (unsigned int i = 0; i < nCount; i++)
		{
			const NS_RAPIDJSON::Value& valJson = arrJson[i];
			LayerPtr layerPtr = std::make_shared<layer>();
			if (layerPtr->FromJson(valJson))
			{
				m_mapLayer.insert(std::make_pair(layerPtr->m_tmCreate, layerPtr));
			}
		}
	}

	void config::SaveLayers(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator)
	{
		NS_RAPIDJSON::Document arrJson(NS_RAPIDJSON::kArrayType);

		for (auto itr = m_mapLayer.begin(); itr != m_mapLayer.end(); ++itr)
		{
			LayerPtr layerPtr = itr->second;
			if (nullptr == layerPtr)
			{
				continue;
			}

			NS_RAPIDJSON::Value obj(NS_RAPIDJSON::kObjectType);
			layerPtr->ToJson(obj, allocator);
			arrJson.PushBack(obj, allocator);
		}
		val.AddMember("layers", arrJson, allocator);
	}

	ActionPtr action::Create(const std::string& type)
	{
		if (type == "sequence") return std::make_shared<sequence>();
		if (type == "spawn") return std::make_shared<spawn>();
		if (type == "move") return std::make_shared<move>();
		if (type == "rotate") return std::make_shared<rotate>();
		if (type == "scale") return std::make_shared<scale>();
		if (type == "fade") return std::make_shared<fade>();
		if (type == "jump") return std::make_shared<jump>();
		
		return nullptr;
	}

	void action::GetActs(const NS_RAPIDJSON::Value& val, std::vector<ActionPtr>& vecAct)
	{
		if (!val.HasMember("acts"))
		{
			return;
		}

		const NS_RAPIDJSON::Value& lstAct = val["acts"];
		if (!lstAct.IsArray() || lstAct.Size() <= 0)
		{
			return;
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
				vecAct.push_back(actPtr);
			}
		}
	}

	void action::AddActs(NS_RAPIDJSON::Value& val, const std::vector<ActionPtr>& vecAct, NS_RAPIDJSON::Document::AllocatorType& allocator)
	{
		NS_RAPIDJSON::Value lstAct(NS_RAPIDJSON::kArrayType);
		for (int i = 0; i < vecAct.size(); ++i)
		{
			ActionPtr actionPtr = vecAct[i];
			if (nullptr == actionPtr)
			{
				continue;
			}

			NS_RAPIDJSON::Value obj(NS_RAPIDJSON::kObjectType);
			actionPtr->ToJson(obj, allocator);
			lstAct.PushBack(obj, allocator);
		}

		val.AddMember("acts", lstAct, allocator);
	}

	bool sequence::FromJson(const NS_RAPIDJSON::Value& val)
	{
		GetActs(val, m_vecAct);
		return true;
	}

	void sequence::ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator)
	{
		val.AddMember("type", "sequence", allocator);
		AddActs(val, m_vecAct, allocator);
	}

	bool spawn::FromJson(const NS_RAPIDJSON::Value& val)
	{
		GetActs(val, m_vecAct);
		return true;
	}

	void spawn::ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator)
	{
		val.AddMember("type", "spawn", allocator);
		AddActs(val, m_vecAct, allocator);
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

	scale::scale()
		: m_width(-1)
		, m_height(-1)
	{

	}

	bool scale::FromJson(const NS_RAPIDJSON::Value& val)
	{
		m_duration = val["duration"].GetInt();
		if (val.HasMember("width")) m_width = val["width"].GetInt();
		if (val.HasMember("height")) m_height = val["height"].GetInt();

		if (compare::is_equal(m_width, -1) && compare::is_equal(m_height, -1))
		{
			m_fitsize.FromJson(val);
		}
		else
		{
			m_fitsize.m_enable = false;
		}

		return true;
	}

	void scale::ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator)
	{
		val.AddMember("type", "scale", allocator);
		val.AddMember("duration", m_duration, allocator);
		if(!compare::is_equal(m_width, -1)) val.AddMember("width", m_width, allocator);
		if (!compare::is_equal(m_height, -1)) val.AddMember("height", m_height, allocator);
		
		if (compare::is_equal(m_width, -1) && compare::is_equal(m_height, -1))
		{
			m_fitsize.ToJson(val, allocator);
		}
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

	bool jump::FromJson(const NS_RAPIDJSON::Value& val)
	{
		m_duration = val["duration"].GetInt();
		JsonToVec2(val, m_postion);
		m_height = val["height"].GetInt();
		m_times = val["times"].GetInt();

		return true;
	}

	void jump::ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator)
	{
		val.AddMember("type", "jump", allocator);
		val.AddMember("duration", m_duration, allocator);
		Vec2ToJson(m_postion, val, allocator);
		val.AddMember("height", m_height, allocator);
		val.AddMember("times", m_times, allocator);
	}
	

}

