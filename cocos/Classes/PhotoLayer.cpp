#include "PhotoLayer.h"
#include "Photo.h"

using namespace act_conf;
using namespace cocos2d;

PhotoLayer::PhotoLayer()
{
	m_timeline = 0;
}

PhotoLayer::~PhotoLayer()
{

}

PhotoLayer* PhotoLayer::create(act_conf::LayerPtr conf)
{
	PhotoLayer* pRet = new PhotoLayer();
	if (pRet && pRet->init(conf))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool PhotoLayer::init(act_conf::LayerPtr conf)
{
	m_conf = conf;
	if (!__super::init())
	{
		return false;
	}

	m_mapSprite = conf->m_mapSprite;
	schedule(schedule_selector(PhotoLayer::update));

	ShowPhotos();

	return true;
}

void PhotoLayer::update(float delta)
{
	__super::update(delta);

	m_timeline += (int)(delta * 1000);

	if (m_mapSprite.empty() && getChildrenCount() == 0)
	{
		removeFromParent();
	}
	else
	{
		ShowPhotos();
	}
}

void PhotoLayer::ShowPhotos()
{
	auto itrEnd = m_mapSprite.upper_bound(m_timeline);
	for (auto itr = m_mapSprite.begin(); itr != itrEnd; )
	{
		SpritePtr ptr = itr->second;
		Photo* pPhoto = Photo::create(ptr);

		addChild(pPhoto);

		auto itrOld = itr;
		itr++;
		m_mapSprite.erase(itrOld);
	}
}
