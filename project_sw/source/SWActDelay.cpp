//
//  SWActDelay.cpp
//  protopang
//
//  Created by 한상운 on 13. 6. 8..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWActDelay.h"
#include "SWGameObject.h"
#include "SWActor.h"

SWActDelay::SWActDelay( float duration )
: m_duration( duration )
{
    
}

bool SWActDelay::isDone()
{
    return ( m_accumulation >= m_duration );
}

bool SWActDelay::onStart()
{
    m_accumulation = 0;
    return true;
}

void SWActDelay::onUpdate( float elapsed )
{
    m_accumulation += elapsed;
}