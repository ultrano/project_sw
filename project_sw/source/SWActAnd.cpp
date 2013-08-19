//
//  SWActAnd.cpp
//  protopang
//
//  Created by 한상운 on 13. 6. 12..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWActAnd.h"
#include "SWActor.h"


SWActAnd::SWActAnd( SWAction* one, SWAction* two, SWAction* tree)
{
    addAction( one );
    addAction( two );
    addAction( tree );
}

void SWActAnd::addAction( SWAction* action )
{
    if ( !action ) return;
    m_actions.push_back( action );
}

bool SWActAnd::isDone()
{
    return ( m_isDone );
}

bool SWActAnd::onStart()
{
    if ( !getActor() ) return false;
    if ( !m_actions.size() ) return false;
    m_isDone = false;
    
    bool allClean = true;
    SWActor* actor = getActor();
    ActionList::iterator itor = m_actions.begin();
    for ( ; itor != m_actions.end() ; ++itor )
    {
        allClean = allClean && actor->runAction( (*itor)() );
    }
    if ( !allClean )
    {
        itor = m_actions.begin();
        for ( ; itor != m_actions.end() ; ++itor )
        {
            actor->stopAction( (*itor)() );
        }
    }
    return allClean;
}

void SWActAnd::onUpdate( float elapsed )
{
    int count = m_actions.size();
    ActionList::iterator itor = m_actions.begin();
    for ( ; itor != m_actions.end() ; ++itor )
    {
        if ( (*itor)()->isDone() ) --count;
    }
    m_isDone = !count;
}