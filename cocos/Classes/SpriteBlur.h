#pragma once

#include "cocos2d.h"

class SpriteBlur : public cocos2d::Sprite
{
public:
	SpriteBlur();
	~SpriteBlur();
	static SpriteBlur* create(const char *pszFileName);

public:
	void setBlurRadius(float radius);
	void setBlurSampleNum(float num);

protected:
	void initGLProgram();
	virtual bool initWithTexture(cocos2d::Texture2D* texture, const cocos2d::Rect&  rect);

protected:
	std::string m_strShaderPath;
	float m_blurRadius;
	float m_blurSampleNum;
};