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
    , m_startTime(0)
{
#ifdef _MSC_VER

	m_startTime = GetTickCount();

#else

    struct timeval tick;
    gettimeofday(&tick, 0);
    m_startTime = tick.tv_sec;

#endif
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

	unsigned int count = GetTickCount() - m_startTime;
	float time = ((float)count)/1000.0f;
	return time;

#else

    struct timeval tick;
    gettimeofday(&tick, 0);
    float second  = (float)(tick.tv_sec - m_startTime);
    second += ((float)tick.tv_usec)/1000000.0f;
    return second;

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