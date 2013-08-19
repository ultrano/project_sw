//
//  SWActSequence.cpp
//  protopang
//
//  Created by 한상운 on 13. 6. 8..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWActSequence.h"
#include "SWAction.h"
#include "SWActor.h"

SWActSequence::SWActSequence( SWAction* one, SWAction* two, SWAction* tree )
: m_cursor( 0 )
, m_current( NULL )
{
    addAction( one );
    addAction( two );
    addAction( tree );
}

void SWActSequence::addAction( SWAction *action )
{
    if ( !action ) return;
    m_actions.push_back( action );
}

size_t SWActSequence::size() const
{
    return m_actions.size();
}

void SWActSequence::clear()
{
    m_actions.clear();
}

bool SWActSequence::isDone()
{
    return ( m_cursor >= m_actions.size() );
}

bool SWActSequence::onStart()
{
    if ( !getActor() ) return false;
    if ( m_actions.size() == 0 ) return false;
    m_cursor = 0;
    m_current = m_actions[m_cursor]();
    getActor()->runAction( m_current() );
    return true;
}

void SWActSequence::onUpdate( float elapsed )
{
    if ( !m_current() || ( m_current() && m_current()->isDone() ) )
    {
        ++m_cursor;
        if ( !isDone() && m_cursor < m_actions.size() )
        {
            m_current = m_actions[m_cursor]();
            getActor()->runAction( m_current() );
        }
    }
}