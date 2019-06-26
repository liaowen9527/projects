#include "PhotoScene.h"
#include "PhotoLayer.h"

USING_NS_CC;
using namespace act_conf;

Scene* PhotoScene::scene()
{
     return PhotoScene::create();
}

// on "init" you need to initialize your instance
bool PhotoScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	m_nDefaultChildrenCount = getChildrenCount();
	schedule(schedule_selector(PhotoScene::update));
	
	Replay();

    return true;
}

void PhotoScene::update(float delta)
{
	__super::update(delta);

	m_timeline += (int)(delta * 1000);

	if (m_mapLayer.empty() && getChildrenCount() == m_nDefaultChildrenCount)
	{
		Replay();
	}
	else
	{
		ShowLayers();
	}
}

void PhotoScene::ShowLayers()
{
	auto itrEnd = m_mapLayer.upper_bound(m_timeline);
	for (auto itr = m_mapLayer.begin(); itr != itrEnd; )
	{
		LayerPtr ptr = itr->second;
		PhotoLayer* pLayer = PhotoLayer::create(ptr);

		addChild(pLayer);

		auto itrOld = itr;
		itr++;
		m_mapLayer.erase(itrOld);
	}
}

void PhotoScene::Replay()
{
	m_timeline = 0;
	m_mapLayer = config::Instance()->m_mapLayer;

	ShowLayers();
}
