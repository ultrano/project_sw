#include "SWActRepeat.h"
#include "SWTime.h"
#include "SWMath.h"
#include "SWLog.h"


SWActRepeat::SWActRepeat( SWAct* act, tuint count )
	: m_act( act )
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
	return ret;
}

void SWActRepeat::onUpdate( float delta )
{
	if ( isDone() ) return;
	
	m_act()->onUpdate( delta );
	if ( m_act()->isDone() )
	{
		m_repeatCount += 1;
		m_act()->onStart();
		m_act()->onUpdate( delta );
	}

	SWLog( "repeat:%d", m_repeatCount );
}