//
//  SWActor.cpp
//  protopang
//
//  Created by 한상운 on 13. 6. 8..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWActor.h"
#include "SWAction.h"

SWActor::SWActor()
: m_isAwake( false )
{
    
}

SWActor::~SWActor()
{
    
}

void SWActor::onAwake()
{
    m_isAwake = true;
    
    for ( ActionList::iterator itor = m_updates.begin()
         ; itor != m_updates.end()
         ; ++itor )
    {
        runAction( (*itor)() );
    }
}

void SWActor::onAsleep()
{
    stopAllActions();
}

bool SWActor::runAction( SWAction *action )
{
    if ( !action ) return false;
    SWHardRef<SWAction> holder = action;
    //! 아직 깨어나지 않았다면 임시로 update용 컨테이너에 담아두었다가
    //! 아중에 깨어나면서 다시 action 리스트에 넣어줌.
    if ( m_isAwake )
    {
        action->setActor( this );
        if ( !action->onStart() ) return false;
        m_actions.push_back( action );
        return true;
    }
    else m_updates.push_back( action );
    return false;
}

void SWActor::reRunAllActions()
{
    m_updates = m_actions;
    m_actions.clear();
    for ( ActionList::iterator itor = m_updates.begin()
         ; itor != m_updates.end()
         ; ++itor )
    {
        runAction( (*itor)() );
    }
}

void SWActor::resumeAllActions()
{
    if ( !m_isAwake ) return;
    for ( ActionList::iterator itor = m_actions.begin()
         ; itor != m_actions.end()
         ; ++itor )
    {
        (*itor)()->resume();
    }
}

void SWActor::pauseAllActions()
{
    if ( !m_isAwake ) return;
    for ( ActionList::iterator itor = m_actions.begin()
         ; itor != m_actions.end()
         ; ++itor )
    {
        (*itor)()->pause();
    }
}

void SWActor::stopAllActions()
{
    if ( m_isAwake ) m_actions.clear();
    else m_updates.clear();
}

void SWActor::stopAction( SWAction *action )
{
    if ( !action ) return;
    SWHardRef<SWAction> holder = action;
    if ( m_isAwake )
    {
        action->setActor( NULL );
        m_actions.remove( action );
    }
    else m_updates.remove( action );
}

bool SWActor::isAllStopped()
{
    return ( m_actions.size() == 0 );
}

void SWActor::onUpdate( float elapsed )
{
    if ( !m_isAwake ) return;
    
    m_updates = m_actions;
    for ( ActionList::iterator itor = m_updates.begin()
         ; itor != m_updates.end()
         ; ++itor )
    {
        SWAction* action = (*itor)();
        if ( action->isPaused() ) continue;
        action->onUpdate( elapsed );
        if (action->isDone()) stopAction( action );
    }
}