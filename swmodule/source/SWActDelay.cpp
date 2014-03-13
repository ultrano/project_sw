#include "SWActDelay.h"

SWActDelay::SWActDelay( float delay )
	: m_delay( delay )
	, m_accum( 0 )
{
}

SWActDelay::~SWActDelay()
{
}

bool SWActDelay::isDone()
{
	return ( m_accum >= m_delay );
}

bool SWActDelay::onStart()
{
	m_accum  = 0;
	return true;
}

void SWActDelay::onUpdate( float delta )
{
	if ( isDone() ) return;
	
	m_accum += delta;

}
