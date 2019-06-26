#include "SpriteBlur.h"

using namespace cocos2d;


SpriteBlur::SpriteBlur()
	: m_blurRadius(0.0f)
	, m_blurSampleNum(7.0f)
{

}

SpriteBlur::~SpriteBlur()
{
}

SpriteBlur* SpriteBlur::create(const char *pszFileName)
{
	SpriteBlur* pRet = new (std::nothrow) SpriteBlur();
	if (pRet)
	{
		bool result = pRet->initWithFile("");
		log("Test call Sprite::initWithFile with bad file name result is : %s", result ? "true" : "false");
	}

	if (pRet && pRet->initWithFile(pszFileName))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool SpriteBlur::initWithTexture(Texture2D* texture, const Rect& rect)
{
	if (Sprite::initWithTexture(texture, rect))
	{
#if CC_ENABLE_CACHE_TEXTURE_DATA
		auto listener = EventListenerCustom::create(EVENT_RENDERER_RECREATED, [this](EventCustom* event) {
			initGLProgram();
		});

		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#endif

		initGLProgram();

		return true;
	}

	return false;
}

void SpriteBlur::initGLProgram()
{
	if (m_strShaderPath.empty())
	{
		return;
	}

	std::string fragSource = FileUtils::getInstance()->getStringFromFile(m_strShaderPath);

	auto program = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource.data());

	auto glProgramState = GLProgramState::getOrCreateWithGLProgram(program);
	setGLProgramState(glProgramState);

	auto size = getTexture()->getContentSizeInPixels();
	getGLProgramState()->setUniformVec2("resolution", size);
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
	getGLProgramState()->setUniformFloat("blurRadius", m_blurRadius);
	getGLProgramState()->setUniformFloat("sampleNum", m_blurSampleNum);
#endif
}

void SpriteBlur::setBlurRadius(float radius)
{
	m_blurRadius = radius;
	getGLProgramState()->setUniformFloat("blurRadius", m_blurRadius);
}

void SpriteBlur::setBlurSampleNum(float num)
{
	m_blurSampleNum = num;
	getGLProgramState()->setUniformFloat("sampleNum", m_blurSampleNum);
}