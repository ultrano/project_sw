#include "SWActDelay.h"
#include "SWTime.h"
#include "SWObjectStream.h"

SWActDelay::SWActDelay( float delay )
	: m_delay( delay )
	, m_accum( 0 )
{
}

SWActDelay::SWActDelay( factory_constructor )
	: m_delay( 0 )
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

void SWActDelay::serialize( SWObjectWriter* writer )
{
	__super::serialize( writer );
	writer->writeFloat( m_accum );
	writer->writeFloat( m_delay );

}

void SWActDelay::deserialize( SWObjectReader* reader )
{
	__super::deserialize( reader );
	m_accum = reader->readFloat();
	m_delay = reader->readFloat();
}
