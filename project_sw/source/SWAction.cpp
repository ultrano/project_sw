//
//  SWAction.cpp
//  protopang
//
//  Created by 한상운 on 13. 6. 8..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWAction.h"
#include "SWActor.h"
#include "SWLog.h"


SWAction::SWAction()
: m_isPaused( false )
{
    
}

SWAction::~SWAction()
{
    //SW_OutputLog( "action", "deleted" );
}

void SWAction::setActor( SWActor* actor )
{
    m_actor = actor;
}

SWActor* SWAction::getActor()
{
    return m_actor();
}

void SWAction::pause()
{
    m_isPaused = true;
}

void SWAction::resume()
{
    m_isPaused = false;
}

bool SWAction::isPaused() const
{
    return m_isPaused;
}