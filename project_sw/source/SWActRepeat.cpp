//
//  SWActRepeat.cpp
//  protopang
//
//  Created by 한상운 on 13. 6. 8..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWActRepeat.h"
#include "SWActor.h"
#include "SWAction.h"

SWActRepeat::SWActRepeat( unsigned int maxCount, SWAction* action )
: m_action( action )
, m_maxCount( maxCount )
, m_count( 0 )
{
    
}

bool SWActRepeat::isDone()
{
    return ( m_count >= m_maxCount );
}

bool SWActRepeat::onStart()
{
    if ( !getActor() ) return false;
    if ( !m_action() ) return false;
    m_count = 0;
    getActor()->runAction( m_action() );
    return true;
}

void SWActRepeat::onUpdate( float elapsed )
{
    if ( m_action()->isDone() )
    {
        ++m_count;
        if ( !isDone() ) getActor()->runAction( m_action() );
    }
}