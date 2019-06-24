#pragma once

#include "cocos2d.h"

class PhotoLayer;

class PhotoScene : public cocos2d::Scene
{
public:
    virtual bool init() override;

    static cocos2d::Scene* scene();

    // implement the "static create()" method manually
    CREATE_FUNC(PhotoScene);

protected:
	PhotoLayer* m_pLayer;
};
