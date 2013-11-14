#ifndef SWTime_h__
#define SWTime_h__

#include "SWMemory.h"

#define SWTime ( __SWTime::getInstance() )
class __SWTime : public SWMemory
{
	friend class SWGameContext;

	float m_lastFrameTime;
	float m_deltaFrameTime;

	float m_accumFrame;
	float m_accumTime;
	float m_FPS;

	__SWTime();
	~__SWTime();

public:

	static __SWTime& getInstance();

	float getTime();
	float getDeltaTime();
	float getFPS();

};

#endif // SWTime_h__