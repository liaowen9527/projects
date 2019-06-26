#pragma once

#include "cocos2d.h"
#include "SpriteBlur.h"
#include "ActionConfig.h"

class Photo : public SpriteBlur
{
public:
	Photo();
	~Photo();

	static Photo* create(act_conf::SpritePtr conf);

protected:
	bool init(act_conf::SpritePtr conf);

protected:
	void CalScale(int width, int height, float& x, float& y);
	cocos2d::Size CalFitSize();

	void doRunAction(act_conf::sprite* pConf);

	void GetActions(const std::vector<act_conf::ActionPtr>& vecActConf, cocos2d::Vector<cocos2d::FiniteTimeAction*>& vecAct);
	cocos2d::FiniteTimeAction* GetAction(act_conf::action* pConf);

	cocos2d::FiniteTimeAction* GetSequenceAction(act_conf::sequence* pConf);
	cocos2d::FiniteTimeAction* GetSpawnAction(act_conf::spawn* pConf);
	cocos2d::FiniteTimeAction* GetMoveAction(act_conf::move* pConf);
	cocos2d::FiniteTimeAction* GetRotateAction(act_conf::rotate* pConf);
	cocos2d::FiniteTimeAction* GetScaleAction(act_conf::scale* pConf);
	cocos2d::FiniteTimeAction* GetFadeAction(act_conf::fade* pConf);
	cocos2d::FiniteTimeAction* GetJumpAction(act_conf::jump* pConf);

protected:
	act_conf::SpritePtr m_conf;
	cocos2d::Size m_fitsize;
};