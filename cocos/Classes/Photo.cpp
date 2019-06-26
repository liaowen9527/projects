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

	BlurPtr blurPtr = config::Instance()->GetBlur(sprintPtr->m_blur);
	if (blurPtr)
	{
		m_strShaderPath = blurPtr->m_strShaderPath;
		m_blurRadius = blurPtr->m_radius;
		m_blurSampleNum = blurPtr->m_num;
	}

	if (!__super::initWithFile(sprintPtr->m_strPath))
	{
		return false;
	}

	m_fitsize = CalFitSize();

	setAnchorPoint(sprintPtr->m_anchorPoint);
	setPosition(sprintPtr->m_postion);

	float x = 1.0, y = 1.0;
	if (sprintPtr->m_fitsize.m_enable)
	{
		CalScale(m_fitsize.width, m_fitsize.height, x, y);
		x *= sprintPtr->m_fitsize.m_scale;
		y *= sprintPtr->m_fitsize.m_scale;
	}
	else
	{
		CalScale(sprintPtr->m_width, sprintPtr->m_height, x, y);
	}
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

cocos2d::Size Photo::CalFitSize()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto contentSize = getContentSize();

	float scale = std::min(visibleSize.width / contentSize.width, visibleSize.height / contentSize.height);

	return Size(contentSize.width * scale, contentSize.height * scale);
}

void Photo::doRunAction(sprite* pConf)
{	
	if (pConf->m_action)
	{
		//runAction(Sequence::create(GetAction(pConf->m_action.get()), CCRemoveSelf::create(), nullptr));
		runAction(Sequence::create(GetAction(pConf->m_action.get()), nullptr));
	}
}

void Photo::GetActions(const std::vector<act_conf::ActionPtr>& vecActConf, Vector<FiniteTimeAction*>& vecAct)
{
	for (int i = 0; i < vecActConf.size(); ++i)
	{
		ActionPtr actionPtr = vecActConf[i];
		auto pAction = GetAction(actionPtr.get());
		if (pAction)
		{
			vecAct.pushBack(pAction);
		}
	}
}

cocos2d::FiniteTimeAction* Photo::GetAction(act_conf::action* pConf)
{
	if (dynamic_cast<act_conf::sequence*>(pConf))
	{
		return GetSequenceAction(dynamic_cast<act_conf::sequence*>(pConf));
	}
	else if (dynamic_cast<act_conf::spawn*>(pConf))
	{
		return GetSpawnAction(dynamic_cast<act_conf::spawn*>(pConf));
	}
	else if (dynamic_cast<act_conf::move*>(pConf))
	{
		return GetMoveAction(dynamic_cast<act_conf::move*>(pConf));
	}
	else if (dynamic_cast<act_conf::rotate*>(pConf))
	{
		return GetRotateAction(dynamic_cast<act_conf::rotate*>(pConf));
	}
	else if (dynamic_cast<act_conf::scale*>(pConf))
	{
		return GetScaleAction(dynamic_cast<act_conf::scale*>(pConf));
	}
	else if (dynamic_cast<act_conf::fade*>(pConf))
	{
		return GetFadeAction(dynamic_cast<act_conf::fade*>(pConf));
	}
	else if (dynamic_cast<act_conf::jump*>(pConf))
	{
		return GetJumpAction(dynamic_cast<act_conf::jump*>(pConf));
	}

	return nullptr;
}

cocos2d::FiniteTimeAction* Photo::GetSequenceAction(act_conf::sequence* pConf)
{
	Vector<FiniteTimeAction*> vecAct;
	GetActions(pConf->m_vecAct, vecAct);

	return Sequence::create(vecAct);
}

cocos2d::FiniteTimeAction* Photo::GetSpawnAction(act_conf::spawn* pConf)
{
	Vector<FiniteTimeAction*> vecAct;
	GetActions(pConf->m_vecAct, vecAct);

	return Spawn::create(vecAct);
}

cocos2d::FiniteTimeAction* Photo::GetMoveAction(act_conf::move* pConf)
{
	return MoveTo::create(pConf->m_duration, pConf->m_postion);
}

cocos2d::FiniteTimeAction* Photo::GetRotateAction(act_conf::rotate* pConf)
{
	return RotateTo::create(pConf->m_duration, pConf->m_angle);
}

cocos2d::FiniteTimeAction* Photo::GetScaleAction(act_conf::scale* pConf)
{
	float x = 1.0, y = 1.0;
	if (pConf->m_fitsize.m_enable)
	{
		CalScale(m_fitsize.width, m_fitsize.height, x, y);
		x *= pConf->m_fitsize.m_scale;
		y *= pConf->m_fitsize.m_scale;
	}
	else
	{
		CalScale(pConf->m_width, pConf->m_height, x, y);
	}

	return ScaleTo::create(pConf->m_duration, x, y);
}

cocos2d::FiniteTimeAction* Photo::GetFadeAction(act_conf::fade* pConf)
{
	return FadeTo::create(pConf->m_duration, pConf->m_opacity);
}

cocos2d::FiniteTimeAction* Photo::GetJumpAction(act_conf::jump* pConf)
{
	return JumpTo::create(pConf->m_duration, pConf->m_postion, pConf->m_height, pConf->m_times);
}
