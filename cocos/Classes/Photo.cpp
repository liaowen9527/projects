#include "Photo.h"
#include "ActionConfig.h"

using namespace act_conf;
using namespace cocos2d;

Photo::Photo()
{

}

Photo::~Photo()
{

}

Photo* Photo::create(act_conf::SpritePtr conf)
{
	Photo* pRet = new Photo(); 
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

bool Photo::init(act_conf::SpritePtr conf)
{
	m_conf = conf;
	sprite* sprintPtr = m_conf.get();

	if (!__super::initWithFile(sprintPtr->m_strPath))
	{
		return false;
	}

	setAnchorPoint(sprintPtr->m_anchorPoint);
	setPosition(sprintPtr->m_postion);

	float x = 1.0, y = 1.0;
	CalScale(sprintPtr->m_width, sprintPtr->m_height, x, y);
	setScaleX(x);
	setScaleY(y);
	
	setRotation(sprintPtr->m_angle);
	setOpacity(sprintPtr->m_opacity);

	doRunAction(sprintPtr);

	return true;
}

void Photo::CalScale(int width, int height, float& x, float& y)
{
	x = width > 0 ? width / getContentSize().width : 1.0;
	y = height > 0 ? height / getContentSize().height : 1.0;
}

void Photo::doRunAction(sprite* pConf)
{
	Vector<FiniteTimeAction*> vecAct;
	for (int i = 0; i < pConf->m_vecAct.size(); ++i)
	{
		ActionPtr actionPtr = pConf->m_vecAct[i];
		vecAct.pushBack(GetAction(actionPtr.get()));
	}
	
	runAction(Sequence::create(Spawn::create(vecAct), CCRemoveSelf::create(), nullptr));

}

cocos2d::FiniteTimeAction* Photo::GetAction(act_conf::action* pConf)
{
	if (dynamic_cast<act_conf::move*>(pConf))
	{
		return GetMoveAction(dynamic_cast<act_conf::move*>(pConf));
	}
	else if (dynamic_cast<act_conf::scale*>(pConf))
	{
		return GetScaleAction(dynamic_cast<act_conf::scale*>(pConf));
	}

	return nullptr;
}

cocos2d::FiniteTimeAction* Photo::GetMoveAction(act_conf::move* pConf)
{
	return MoveTo::create(pConf->m_duration, pConf->m_postion);
}

cocos2d::FiniteTimeAction* Photo::GetScaleAction(act_conf::scale* pConf)
{
	float x = 1.0, y = 1.0;
	CalScale(pConf->m_width, pConf->m_height, x, y);
	
	return ScaleTo::create(pConf->m_duration, x, y);
}
