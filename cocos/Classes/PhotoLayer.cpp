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

bool PhotoLayer::init()
{
	if (!__super::init())
	{
		return false;
	}

	//schedule(schedule_selector(PhotoLayer::update));

	m_mapSprite = config::Instance()->m_mapSprite;

	ShowPhotos();

	return true;
}

void PhotoLayer::update(float delta)
{
	__super::update(delta);

	m_timeline += (int)(delta * 1000);

	//ShowPhotos();
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
