#include "PhotoScene.h"
#include "PhotoLayer.h"

USING_NS_CC;


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

	m_pLayer = PhotoLayer::create();
	addChild(m_pLayer);

    return true;
}
