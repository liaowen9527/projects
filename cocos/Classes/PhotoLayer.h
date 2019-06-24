#pragma once

#include "cocos2d.h"
#include "ActionConfig.h"

class PhotoLayer : public cocos2d::Layer
{
public:
	PhotoLayer();
	~PhotoLayer();

	CREATE_FUNC(PhotoLayer);

protected:
	virtual bool init() override;
	virtual void update(float delta);

protected:
	void ShowPhotos();

protected:
	time_t m_timeline;	//millisecond

	std::map<time_t, act_conf::SpritePtr> m_mapSprite;
};