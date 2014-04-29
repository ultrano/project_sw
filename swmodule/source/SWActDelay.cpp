#include "SWActDelay.h"
#include "SWTime.h"

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

void SWActDelay::onStart()
{
	m_accum  = 0;
}

void SWActDelay::onUpdate()
{
	if ( isDone() ) return;

	m_accum += SWTime.getDeltaTime();

}
