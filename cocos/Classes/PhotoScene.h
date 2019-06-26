#pragma once

#include "cocos2d.h"
#include "ActionConfig.h"

class PhotoLayer;

class PhotoScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* scene();

    // implement the "static create()" method manually
    CREATE_FUNC(PhotoScene);

protected:
	virtual bool init() override;
	virtual void update(float delta);

protected:
	void ShowLayers();
	void Replay();

protected:
	time_t m_timeline;	//millisecond

	std::multimap<time_t, act_conf::LayerPtr> m_mapLayer;
	int m_nDefaultChildrenCount;
};
