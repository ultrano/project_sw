#include "SWTime.h"
#include "SWUtil.h"

#ifdef _MSC_VER
#include <Windows.h>
#else
#include <sys/time.h>
#endif

__SWTime::__SWTime()
	: m_lastFrameTime(0)
	, m_deltaFrameTime(0)
	, m_accumDraw(0)
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
#ifdef _MSC_VER
	unsigned int count = GetTickCount();
	float time = ((float)count)/1000.0f;
	return time;
#else
    struct timeval tick;
    gettimeofday(&tick, 0);
    float sec  = ((float)tick.tv_sec)*1000.0f;
    float usec = ((float)tick.tv_usec)/1000.0f;
    return (sec + usec)/1000.0f;
#endif
}

float __SWTime::getDeltaTime()
{
	return m_deltaFrameTime;
}

float __SWTime::getFPS()
{
	return m_FPS;
}

float __SWTime::getDPS()
{
	return m_DPS;
}
