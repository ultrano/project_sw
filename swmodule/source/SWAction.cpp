#include "SWAction.h"
#include "SWList.h"
#include "SWGameObject.h"
#include "SWTime.h"
#include "SWAct.h"
#include "SWObjectStream.h"

SWAction::SWAction()
{
}

SWAction::SWAction( factory_constructor )
{

}

SWAction::~SWAction()
{

}

void SWAction::onAwake()
{
	__super::onAwake();
	gameObject()->addUpdateDelegator( GetDelegator( onUpdate ) );
}

void SWAction::onRemove()
{
	gameObject()->removeUpdateDelegator( GetDelegator( onUpdate ) );
	__super::onRemove();
}

void SWAction::onUpdate()
{
	if ( m_next() != NULL )
	{
		m_act = m_next;
		m_act()->onStart();
		m_next = NULL;
	}
	if ( m_act() != NULL )
	{
		m_act()->onUpdate();
		if ( m_act()->isDone() ) stop();
	}
	/*
	SWWeakRef<SWAction> vital = this;
	SWList::Type itorList;
	m_actList()->copy( itorList );
	SWList::iterator itor = itorList.begin();
	for ( ; itor != itorList.end() ; ++itor )
	{
		if ( vital.isValid() == false ) break;
		SWAct* act = swrtti_cast<SWAct>( (*itor)() );
		if ( act == NULL ) break;
		if ( act->isPaused() ) continue;
		if ( act->isDone() )
		{
			stopAct( act );
			continue;
		}
		act->onUpdate();
	}
	*/
}

void SWAction::setAct( const tstring& name, SWAct* act )
{
	if ( act == NULL ) return;

	act->setAction( this );
	m_actTable[ name ] = act;
}

SWAct* SWAction::getAct( const tstring& name )
{
	ActTable::iterator itor = m_actTable.find( name );
	if ( itor != m_actTable.end() ) return itor->second();
	return NULL;
}

void SWAction::play( const tstring& name )
{
	m_next = getAct( name );
}

void SWAction::stop()
{
	m_act = NULL;
}

bool SWAction::isPlaying() const
{
	return ((m_act() != NULL) && ( !m_act()->isDone() ));
}

bool SWAction::isPlaying( const tstring& name ) const
{
	ActTable::const_iterator itor = m_actTable.begin();
	for ( ; itor != m_actTable.end() ; ++itor )
	{
		if ( itor->second() == m_act() )
		{
			return (itor->first == name);
		}
	}
	return false;
}

void SWAction::serialize( SWObjectWriter* writer )
{
	__super::serialize( writer );
	
	writer->writeUInt( m_actTable.size() );
	ActTable::iterator itor = m_actTable.begin();
	for ( ; itor != m_actTable.end() ; ++itor )
	{
		writer->writeString( itor->first.str() );
		writer->writeObject( itor->second() );
	}
}

void SWAction::deserialize( SWObjectReader* reader )
{
	__super::deserialize( reader );
	tuint count = reader->readUInt();
	for ( tuint i = 0 ; i < count ; ++i )
	{
		tstring key;
		reader->readString( key );
		m_actTable[ key ] = swrtti_cast<SWAct>( reader->readObject() );
	}
}

