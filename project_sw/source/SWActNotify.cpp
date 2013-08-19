//
//  SWActNotify.cpp
//  protopang
//
//  Created by hahasasa on 13. 6. 9..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWActNotify.h"

SWActNotify::SWActNotify()
: m_listener( NULL )
, m_func( NULL )
, m_isNotified( false )
{
    
}

SWActNotify::SWActNotify( const std::string& tag, SWObject* listener, SWHandler func )
: m_tag( tag )
, m_listener( listener )
, m_func( func )
, m_isNotified( false )
{
    
}

void SWActNotify::setListener( SWObject *listener, SWHandler func )
{
    m_listener = listener;
    m_func = func;
}

bool SWActNotify::isDone()
{
    return m_isNotified || ( !m_listener() || !m_func );
}

bool SWActNotify::onStart()
{
    if ( !m_listener() || !m_func ) return false;
    m_isNotified = false;
    return true;
}

void SWActNotify::onUpdate( float elapsed )
{
    if ( m_isNotified ) return;
    if ( !m_listener() || !m_func ) return;
    
    (m_listener()->*m_func)( this );
    
    m_isNotified = true;

}