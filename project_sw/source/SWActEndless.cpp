//
//  SWActEndless.cpp
//  protopang
//
//  Created by 한상운 on 13. 6. 10..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWActEndless.h"
#include "SWActor.h"

SWActEndless::SWActEndless( SWAction* action )
: m_action( action )
{
    
}

bool SWActEndless::isDone()
{
    return !m_action.isValid();
}

bool SWActEndless::onStart()
{
    if ( !m_action() ) return false;
    if ( !getActor() ) return false;
    getActor()->runAction( m_action() );
    return true;
}

void SWActEndless::onUpdate( float elapsed )
{
    if ( m_action()->isDone() )
    {
        getActor()->runAction( m_action() );
    }
}