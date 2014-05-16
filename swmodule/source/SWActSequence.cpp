#include "SWActSequence.h"
#include "SWAction.h"
#include "SWTime.h"
#include "SWMath.h"
#include "SWObjectStream.h"

SWActSequence::SWActSequence()
	: m_current( NULL )
	, m_cursor(0)
{

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

void SWActSequence::serialize( SWObjectWriter* ow )
{
	__super::serialize( ow );
	ow->writeUInt( m_acts.size() );
	for ( tuint i = 0 ; i < m_acts.size() ; ++i )
	{
		ow->writeObject( m_acts[i]() );
	}

}

void SWActSequence::deserialize( SWObjectReader* or )
{
	__super::deserialize( or );
	tuint count = or->readUInt();
	m_acts.reserve( count );
	for ( tuint i = 0 ; i < count ; ++i )
	{
		SWAct* act = swrtti_cast<SWAct>( or->readObject() );
		m_acts.push_back( act );
	}
}
