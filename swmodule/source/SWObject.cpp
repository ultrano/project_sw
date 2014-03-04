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

int& getObjectCount()
{
    static int count = 0;
    return count;
}

unsigned& uniqueObjectID()
{
	static unsigned unique_id = 0;
	return unique_id;
}

SWObject::SWObject()
	: m_id( ++uniqueObjectID() )
{
    //++getObjectCount();
}

SWObject::~SWObject()
{
    //--getObjectCount();
    //SW_OutputLog( "object", "deleted, remains: %d", getObjectCount() );
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
		SWDelegator* dg = swrtti_cast<SWDelegator>( (*itor)() );
		dg->destroy();
	}
	_make_zero_ref();
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