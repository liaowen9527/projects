#pragma once
#include <memory>
#include <vector>
#include <string>
#include "json/document.h"

#include "cocos2d.h"

#define NS_RAPIDJSON RAPIDJSON_NAMESPACE

namespace act_conf
{
	class action;
	typedef std::shared_ptr<action> ActionPtr;

	class sprite
	{
	public:
		sprite();

		bool FromJson(const NS_RAPIDJSON::Value& val);
		void ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator);

	public:
		time_t m_tmCreate;
		std::string m_strPath;
		cocos2d::Vec2 m_anchorPoint;
		cocos2d::Vec2 m_postion;
		float m_angle;
		int m_width;
		int m_height;
		int m_opacity;

		std::vector<ActionPtr> m_vecAct;
	};
	typedef std::shared_ptr<sprite> SpritePtr;

	class config
	{
	public:
		config();
		~config();

		static config* Instance();

		bool FromJson(const std::string& strPath);
		void ToJson(const std::string& strPath);

	public:
		int m_width;
		int m_height;
		int m_designWidth;
		int m_designHeight;
		bool m_fullScreen;
		std::map<time_t, SpritePtr> m_mapSprite;
	};

	class action
	{
	public:
		static ActionPtr Create(const std::string& type);

	public:
		virtual bool FromJson(const NS_RAPIDJSON::Value& val) = 0;
		virtual void ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator) = 0;
	};

	class move : public action
	{
	public:
		virtual bool FromJson(const NS_RAPIDJSON::Value& val);
		virtual void ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator);

	public:
		int m_duration;
		cocos2d::Vec2 m_postion;
	};

	class rotate : public action
	{
	public:
		virtual bool FromJson(const NS_RAPIDJSON::Value& val);
		virtual void ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator);

	public:
		int m_duration;
		float m_angle;
	};

	class sequence : public action
	{
	public:
		virtual bool FromJson(const NS_RAPIDJSON::Value& val);
		virtual void ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator);

	public:
		std::vector<ActionPtr> m_vecAct;
	};

	class scale : public action
	{
	public:
		virtual bool FromJson(const NS_RAPIDJSON::Value& val);
		virtual void ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator);

	public:
		int m_duration;
		int m_width;
		int m_height;
	};

	class fade : public action
	{
	public:
		virtual bool FromJson(const NS_RAPIDJSON::Value& val);
		virtual void ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator);

	public:
		int m_duration;
		float m_opacity;
	};

}
