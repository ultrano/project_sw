//
//  SWActDuration.cpp
//  shakeThatAss
//
//  Created by 한상운 on 13. 6. 19..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWActDuration.h"
#include "SWActor.h"
#include "SWAction.h"

SWActDuration::SWActDuration( float duration, SWAction* action )
: m_duration( duration )
, m_action( action )
{
    
}

bool SWActDuration::isDone()
{
    return ( m_accumulation >= m_duration );
}

bool SWActDuration::onStart()
{
    if ( !getActor() ) return false;
    if ( !m_action() ) return false;
    m_accumulation = 0;
    getActor()->runAction( m_action() );
    return true;
}

void SWActDuration::onUpdate( float elapsed )
{
    m_accumulation += elapsed;
    if ( m_action()->isDone() && !isDone() ) getActor()->runAction( m_action() );
}
