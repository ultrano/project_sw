//
//  SWObject.cpp
//  protopang
//
//  Created by 한상운 on 13. 6. 12..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWObject.h"
#include "SWLog.h"
#include "SWRtti.h"
#include "SWByteBufferStream.h"
#include "SWObjectStream.h"

int& getObjectCount()
{
    static int count = 0;
    return count;
}

SWObject::SWObject()
{
    ++getObjectCount();
}

SWObject::~SWObject()
{
	__this::destroy();
    --getObjectCount();
    SWLog( "SWObject deleted, remains: %d", getObjectCount() );
}

SWDelegator* SWObject::getDelegator( const SWHandler& handler )
{
	SWDelegator* dg = NULL;
	SWObject::List::iterator itor = m_delegates.begin();
	for ( ; itor != m_delegates.end() ; ++itor )
	{
		dg = swrtti_cast<SWDelegator>( (*itor)() );
		if ( dg->getHandler().m_method == handler.m_method ) return dg;
	}
	
	dg = new SWDelegator( this, handler );
	m_delegates.push_back( dg );
	return dg;
}

tstring SWObject::toString() const
{
	static char buf[64];
    sprintf( &buf[0], "%d", m_id );
	return &buf[0];
}

void SWObject::destroy()
{
	SWObject::List::iterator itor = m_delegates.begin();
	for ( ; itor != m_delegates.end() ; ++itor )
	{
		if ( (*itor).isValid() == false ) continue;
		SWDelegator* dg = swrtti_cast<SWDelegator>( (*itor)() );
		dg->destroy();
	}
	_make_zero_ref();
}

SWHardRef<SWObject> SWObject::clone()
{
	SWByteBufferOutputStream* bbos = new SWByteBufferOutputStream();
	SWHardRef<SWObjectWriter> ow = new SWObjectWriter( bbos );
	ow()->writeObject( this );

	SWByteBufferInputStream* bbis = new SWByteBufferInputStream( bbos->getBuffer() );
	SWHardRef<SWObjectReader> reader = new SWObjectReader( bbis );

	SWHardRef<SWObject> ret = reader()->readObject();
	return ret;
}

SWDelegator::SWDelegator( SWObject* object, const SWHandler& handler )
	: m_object(object), m_handler(handler)
{

}

SWDelegator::SWDelegator( const SWDelegator& copy )
	: m_object( copy.m_object), m_handler( copy.m_handler )
{

}

void SWDelegator::call() const
{
	if ( !isValid() ) return;
	if ( m_handler.m_needParam ) (m_object()->*m_handler.m_method)(NULL);
	else
	{
		SWHandler::MethodVoid method = (SWHandler::MethodVoid)m_handler.m_method;
		(m_object()->*method)();
	}
}

void SWDelegator::call( SWObject* object ) const
{
	if ( !isValid() ) return;
	if ( m_handler.m_needParam ) (m_object()->*m_handler.m_method)(object);
	else
	{
		SWHandler::MethodVoid method = (SWHandler::MethodVoid)m_handler.m_method;
		(m_object()->*method)();
	}
}

bool SWDelegator::isValid() const
{
	return m_object() && m_handler.m_method;
}
bool SWDelegator::isEqual( const SWDelegator* dg ) const
{
	bool check1 = ( dg->getHandler().m_method == m_handler.m_method );
	bool check2 = ( dg->getObject() == m_object() );
	return ( check1 && check2 );
}

bool SWDelegator::isEqual( SWObject* object, const SWHandler& handler ) const
{
	bool check1 = ( handler.m_method == m_handler.m_method );
	bool check2 = ( object == m_object() );
	return ( check1 && check2 );
}