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

	class blur
	{
	public:
		blur();

		bool FromJson(const NS_RAPIDJSON::Value& val);
		void ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator);

	public:
		std::string m_strShaderPath;
		float m_radius;	//suggest 0.0 ~ 25.0
		float m_num;	//suggest 0.0 ~ 11
	};
	typedef std::shared_ptr<blur> BlurPtr;

	class fitsize
	{
	public:
		fitsize();

		bool FromJson(const NS_RAPIDJSON::Value& val);
		void ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator);

	public:
		bool m_enable;
		float m_scale;
	};

	class sprite
	{
	public:
		sprite();

		bool FromJson(const NS_RAPIDJSON::Value& val);
		void ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator);

	protected:
		void LoadAction(const NS_RAPIDJSON::Value& val);
		void LoadBlur(const NS_RAPIDJSON::Value& val);

	public:
		time_t m_tmCreate;
		std::string m_strPath;
		cocos2d::Vec2 m_anchorPoint;
		cocos2d::Vec2 m_postion;
		float m_angle;
		int m_width;
		int m_height;
		int m_opacity;
		int m_zorder;

		ActionPtr m_action;
		int m_blur;
		fitsize m_fitsize;
	};
	typedef std::shared_ptr<sprite> SpritePtr;

	class layer 
	{
	public:
		bool FromJson(const NS_RAPIDJSON::Value& val);
		void ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator);

	protected:
		void LoadSprites(const NS_RAPIDJSON::Value& val);
		void SaveSprites(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator);

	public:
		time_t m_tmCreate;
		std::multimap<time_t, SpritePtr> m_mapSprite;
	};
	typedef std::shared_ptr<layer> LayerPtr;

	class config
	{
	public:
		config();
		~config();

		static config* Instance();

		bool FromJson(const std::string& strPath);
		void ToJson(const std::string& strPath);

		BlurPtr GetBlur(int nIndex);

	protected:
		void LoadBlurs(const NS_RAPIDJSON::Value& val);
		void SaveBlurs(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator);

		void LoadLayers(const NS_RAPIDJSON::Value& val);
		void SaveLayers(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator);

	public:
		int m_width;
		int m_height;
		int m_designWidth;
		int m_designHeight;
		bool m_fullScreen;
		std::vector<BlurPtr> m_vecBlur;
		std::multimap<time_t, LayerPtr> m_mapLayer;
	};

	class action
	{
	public:
		static ActionPtr Create(const std::string& type);

	public:
		virtual bool FromJson(const NS_RAPIDJSON::Value& val) = 0;
		virtual void ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator) = 0;

		void GetActs(const NS_RAPIDJSON::Value& val, std::vector<ActionPtr>& vecAct);
		void AddActs(NS_RAPIDJSON::Value& val, const std::vector<ActionPtr>& vecAct, NS_RAPIDJSON::Document::AllocatorType& allocator);
	};

	class sequence : public action
	{
	public:
		virtual bool FromJson(const NS_RAPIDJSON::Value& val);
		virtual void ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator);

	public:
		std::vector<ActionPtr> m_vecAct;
	};

	class spawn : public action
	{
	public:
		virtual bool FromJson(const NS_RAPIDJSON::Value& val);
		virtual void ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator);

	public:
		std::vector<ActionPtr> m_vecAct;
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

	class scale : public action
	{
	public:
		scale();

		virtual bool FromJson(const NS_RAPIDJSON::Value& val);
		virtual void ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator);

	public:
		int m_duration;
		int m_width;
		int m_height;
		fitsize m_fitsize;
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

	class jump : public action
	{
	public:
		virtual bool FromJson(const NS_RAPIDJSON::Value& val);
		virtual void ToJson(NS_RAPIDJSON::Value& val, NS_RAPIDJSON::Document::AllocatorType& allocator);

	public:
		int m_duration;
		cocos2d::Vec2 m_postion;
		int m_height;
		int m_times;
	};
}
