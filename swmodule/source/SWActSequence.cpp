#include "SWActSequence.h"
#include "SWAction.h"
#include "SWTime.h"
#include "SWMath.h"

SWActSequence::SWActSequence()
	: m_current( NULL )
	, m_cursor(0)
{

}

SWActSequence::~SWActSequence()
{

}

void SWActSequence::addAct( SWAct* act )
{
	if ( act == NULL ) return;
	m_acts.push_back( act );
}

bool SWActSequence::isDone()
{
	return ( ( m_acts.size() == 0 ) || (m_acts.size() == m_cursor) );
}

bool SWActSequence::onStart()
{
	m_cursor  = 0;
	m_current = NULL;
	m_startTime = SWTime.getTime();
	m_accumTime = 0;
	return true;
}

void SWActSequence::onUpdate( float delta )
{
	if ( isDone() ) return;
	
	if ( m_current() != NULL && m_current()->isDone() )
	{
		m_current = NULL;
		m_cursor += 1;
	}
	
	if ( isDone() ) return;
	
	m_accumTime += delta;
	if ( m_current() == NULL )
	{
		float nowTime = SWTime.getTime();
		delta = (m_accumTime - (nowTime - m_startTime) );
		delta = SWMath.max<float>( delta, 0 );

		m_startTime = nowTime;
		m_accumTime = 0;

		m_current = m_acts[ m_cursor ]();
		m_current()->setAction( getAction() );
		m_current()->onStart();
	}

	m_current()->onUpdate( delta );
}
