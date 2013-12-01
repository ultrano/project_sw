#include "SWActSequence.h"
#include "SWAction.h"

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
	if ( act ) act->setAction( getAction() );
}

bool SWActSequence::isDone()
{
	return ( ( m_acts.size() <= 0 ) || (m_acts.size() == m_cursor) );
}

bool SWActSequence::onStart()
{
	m_cursor = 0;
	return true;
}

void SWActSequence::onUpdate()
{

}
