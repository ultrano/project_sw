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
	return true;
}

void SWActSequence::onUpdate()
{
	if ( isDone() ) return;
	
	float delta = SWTime.getDeltaTime();
	if ( m_current() != NULL )
	{
		m_current()->onUpdate();
		if ( m_current()->isDone() )
		{
			m_current = NULL;
			m_cursor += 1;
		}
	}
	

	if ( m_current() == NULL )
	{
		do
		{
			if ( isDone() ) return;
			m_current = m_acts[ m_cursor ]();
			m_current()->setAction( getAction() );
			m_current()->onStart();
			if ( m_current()->isDone() )
			{
				m_cursor += 1;
				continue;
			}
			else
			{
				m_current()->onUpdate();
				break;
			}
		} while ( true );
	}

}
