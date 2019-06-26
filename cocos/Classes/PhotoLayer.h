#pragma once

#include "cocos2d.h"
#include "ActionConfig.h"

class PhotoLayer : public cocos2d::Layer
{
public:
	PhotoLayer();
	~PhotoLayer();

	static PhotoLayer* create(act_conf::LayerPtr conf);

protected:
	virtual bool init(act_conf::LayerPtr conf);
	virtual void update(float delta);

protected:
	void ShowPhotos();

protected:
	act_conf::LayerPtr m_conf;
	time_t m_timeline;	//millisecond
	std::multimap<time_t, act_conf::SpritePtr> m_mapSprite;
};