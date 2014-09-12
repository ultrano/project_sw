#include "SWActSequence.h"
#include "SWAction.h"
#include "SWMath.h"
#include "SWObjectStream.h"

SWActSequence::SWActSequence( SWAct* act1, SWAct* act2, SWAct* act3 )
	: m_current( NULL )
	, m_cursor(0)
{
	if ( act1 ) addAct( act1 );
	if ( act2 ) addAct( act2 );
	if ( act3 ) addAct( act3 );
}

SWActSequence::SWActSequence( factory_constructor )
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

void SWActSequence::onStart()
{
	m_cursor  = 0;
	m_current = NULL;
}

void SWActSequence::onUpdate()
{
	if ( isDone() ) return;
	
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

void SWActSequence::serialize( SWObjectWriter* writer )
{
	__super::serialize( writer );
	writer->writeUInt( m_acts.size() );
	for ( tuint i = 0 ; i < m_acts.size() ; ++i )
	{
		writer->writeObject( m_acts[i]() );
	}

}

void SWActSequence::deserialize( SWObjectReader* reader )
{
	__super::deserialize( reader );
	tuint count = reader->readUInt();
	m_acts.reserve( count );
	for ( tuint i = 0 ; i < count ; ++i )
	{
		SWAct* act = swrtti_cast<SWAct>( reader->readObject() );
		m_acts.push_back( act );
	}
}
