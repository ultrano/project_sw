#include "SWTime.h"
#include "SWUtil.h"

__SWTime::__SWTime()
	: m_lastFrameTime(0)
	, m_deltaFrameTime(0)
	, m_accumFrame(0)
	, m_accumTime(0)
	, m_FPS(0)
{

}

__SWTime::~__SWTime()
{

}

__SWTime& __SWTime::getInstance()
{
	static __SWTime instance;
	return instance;
}

float __SWTime::getTime()
{
	return ((float)SWUtil.getTickCount()) / 1000.0f;
}

float __SWTime::getDeltaTime()
{
	return m_deltaFrameTime;
}

float __SWTime::getFPS()
{
	return m_FPS;
}
