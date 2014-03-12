#include "SWActRepeat.h"
#include "SWTime.h"
#include "SWMath.h"
#include "SWLog.h"


SWActRepeat::SWActRepeat( SWAct* act, tuint count )
	: m_act( act )
	, m_startTime(0)
	, m_accumTime(0)
	, m_repeatCount(0)
	, m_limitCount( count )
{
}

SWActRepeat::~SWActRepeat()
{
}

bool SWActRepeat::isDone()
{
	return ( m_limitCount > 0 )? ( m_repeatCount >= m_limitCount ) : false;
}

bool SWActRepeat::onStart()
{
	if ( !m_act.isValid() ) return false;
	m_act()->setAction( getAction() );
	bool ret = m_act()->onStart();
	if ( ret )
	{
		m_startTime = SWTime.getTime();
		m_accumTime = 0;
	}
	return ret;
}

void SWActRepeat::onUpdate( float delta )
{
	if ( isDone() ) return;

	m_accumTime += delta;
	if ( m_act()->isDone() )
	{
		float nowTime = SWTime.getTime();
		delta = (m_accumTime - (nowTime - m_startTime) );
		delta = SWMath.max<float>( delta, 0 );
		
		if ( delta > 0 ) SWLog( "delta: %f", delta );

		m_startTime = nowTime;
		m_accumTime = 0;
		m_repeatCount += 1;

		m_act()->onStart();
	}
	m_act()->onUpdate( delta );
}