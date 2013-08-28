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
    //if ( getObjectCount() % 10 == 0 ) SW_OutputLog( "object", "deleted, remains: %d", getObjectCount() );
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
