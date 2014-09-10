#include "SWActBunch.h"
#include "SWObjectStream.h"

SWActBunch::SWActBunch( SWAct* act1, SWAct* act2, SWAct* act3 )
	: m_countOfDone(0)
{
	if ( act1 ) addAct( act1 );
	if ( act2 ) addAct( act2 );
	if ( act3 ) addAct( act3 );
}

SWActBunch::SWActBunch( factory_constructor )
	: m_countOfDone(0)
{

}

SWActBunch::~SWActBunch()
{

}

void SWActBunch::addAct( SWAct* act )
{
	if ( act == NULL ) return;
	m_acts.push_back( act );
}

bool SWActBunch::isDone()
{
	return ( (m_acts.size() == 0) || (m_acts.size() == m_countOfDone) );
}

void SWActBunch::onStart()
{
	m_countOfDone = 0;
	for ( tuint i = 0 ; i < m_acts.size() ; ++i )
	{
		SWAct* act = m_acts[i]();
		if ( act == NULL ) continue;
		act->setAction( getAction() );
		act->onStart();
	}
}

void SWActBunch::onUpdate()
{
	if ( isDone() ) return;

	m_countOfDone = 0;
	for ( tuint i = 0 ; i < m_acts.size() ; ++i )
	{
		SWAct* act = m_acts[i]();
		if ( act == NULL || act->isDone() )
		{
			m_countOfDone += 1;
			continue;
		}
		act->onUpdate();
	}
}

void SWActBunch::serialize( SWObjectWriter* ow )
{
	__super::serialize( ow );
	ow->writeUInt( m_acts.size() );
	for ( tuint i = 0 ; i < m_acts.size() ; ++i )
	{
		ow->writeObject( m_acts[i]() );
	}
}

void SWActBunch::deserialize( SWObjectReader* or )
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
