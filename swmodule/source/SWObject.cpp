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

SWDelegate* SWObject::getDelegate( const SWHandler& handler )
{
	SWDelegate* dg = NULL;
	SWObjectList::iterator itor = m_delegates.begin();
	for ( ; itor != m_delegates.end() ; ++itor )
	{
		dg = swrtti_cast<SWDelegate>( (*itor)() );
		if ( dg->getHandler().m_method == handler.m_method ) return dg;
	}
	
	dg = new SWDelegate( this, handler );
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
	SWObjectList::iterator itor = m_delegates.begin();
	for ( ; itor != m_delegates.end() ; ++itor )
	{
		SWDelegate* dg = swrtti_cast<SWDelegate>( (*itor)() );
		dg->destroy();
	}
	_make_zero_ref();
}

SWDelegate::SWDelegate( SWObject* object, const SWHandler& handler )
	: m_object(object), m_handler(handler)
{

}

SWDelegate::SWDelegate( const SWDelegate& copy )
	: m_object( copy.m_object), m_handler( copy.m_handler )
{

}

void SWDelegate::call() const
{
	if ( !isValid() ) return;
	if ( m_handler.m_needParam ) (m_object()->*m_handler.m_method)(NULL);
	else
	{
		SWHandler::MethodVoid method = (SWHandler::MethodVoid)m_handler.m_method;
		(m_object()->*method)();
	}
}

void SWDelegate::call( SWObject* object ) const
{
	if ( !isValid() ) return;
	if ( m_handler.m_needParam ) (m_object()->*m_handler.m_method)(object);
	else
	{
		SWHandler::MethodVoid method = (SWHandler::MethodVoid)m_handler.m_method;
		(m_object()->*method)();
	}
}

bool SWDelegate::isValid() const
{
	return m_object() && m_handler.m_method;
}
bool SWDelegate::isEqual( const SWDelegate* dg ) const
{
	bool check1 = ( dg->getHandler().m_method == m_handler.m_method );
	bool check2 = ( dg->getObject() == m_object() );
	return ( check1 && check2 );
}

bool SWDelegate::isEqual( SWObject* object, const SWHandler& handler ) const
{
	bool check1 = ( handler.m_method == m_handler.m_method );
	bool check2 = ( object == m_object() );
	return ( check1 && check2 );
}