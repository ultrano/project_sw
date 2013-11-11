//
//  SWObject.cpp
//  protopang
//
//  Created by 한상운 on 13. 6. 12..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWObject.h"
#include "SWLog.h"


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
		if ( dg->getHandler() == handler ) return dg;
	}
	
	dg = new SWDelegate( this, handler );
	m_delegates.push_back( dg );
	return dg;
}

std::string SWObject::toString()
{
	static char buf[64];
	return itoa( m_id, &buf[0], 10 );
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

void SWDelegate::call( SWObject* object )
{
	if ( !isValid() ) return;
	(m_object()->*m_handler)(object);
}

bool SWDelegate::isValid()
{
	return m_object() && m_handler;
}
bool SWDelegate::isEqual( const SWDelegate* dg )
{
	bool check1 = ( dg->getHandler() == m_handler );
	bool check2 = ( dg->getObject() == m_object() );
	return ( check1 && check2 );
}

bool SWDelegate::isEqual( SWObject* object, const SWHandler& handler )
{
	bool check1 = ( handler == m_handler );
	bool check2 = ( object == m_object() );
	return ( check1 && check2 );
}