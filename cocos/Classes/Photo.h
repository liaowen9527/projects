#pragma once

#include "cocos2d.h"
#include "ActionConfig.h"

class Photo : public cocos2d::Sprite
{
public:
	Photo();
	~Photo();

	static Photo* create(act_conf::SpritePtr conf);

protected:
	bool init(act_conf::SpritePtr conf);

protected:
	void CalScale(int width, int height, float& x, float& y);

	void doRunAction(act_conf::sprite* pConf);
	cocos2d::FiniteTimeAction* GetAction(act_conf::action* pConf);
	cocos2d::FiniteTimeAction* GetMoveAction(act_conf::move* pConf);
	cocos2d::FiniteTimeAction* GetScaleAction(act_conf::scale* pConf);

protected:
	act_conf::SpritePtr m_conf;
};