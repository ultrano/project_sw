#include "SWActRepeat.h"
#include "SWTime.h"
#include "SWMath.h"

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

void SWActRepeat::onStart()
{
	if ( !m_act.isValid() ) return ;
	m_act()->setAction( getAction() );
	m_act()->onStart();
}

void SWActRepeat::onUpdate()
{
	if ( isDone() ) return;

	float delta = SWTime.getDeltaTime();
	m_act()->onUpdate();
	if ( m_act()->isDone() )
	{
		m_repeatCount += 1;
		m_act()->onStart();
		m_act()->onUpdate();
	}
}