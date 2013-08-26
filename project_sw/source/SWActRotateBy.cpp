//
//  SWActRotateBy.cpp
//  protopang
//
//  Created by 한상운 on 13. 6. 10..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWActRotateBy.h"
#include "SWActor.h"
#include "SWGameObject.h"
#include "SWTransform.h"

SWActRotateBy::SWActRotateBy( float disp, float duration )
: m_displasement( disp )
, m_duration( duration )
, m_accumulation( 0 )
{
    
}

bool SWActRotateBy::isDone()
{
    return ( m_accumulation >= m_duration );
}

bool SWActRotateBy::onStart()
{
    if ( !getActor() ) return false;
    if ( !getActor()->getGameObject() ) return false;
    m_trans = getActor()->getGameObject()->getTransform();
    if ( !m_trans() ) return false;
    m_accumulation = 0;
    return true;
}

void SWActRotateBy::onUpdate( float elapsed )
{
    if ( !m_trans.isValid() ) return;
    if ( isDone() ) return;
    
    elapsed = ( ( m_accumulation + elapsed ) > m_duration )? (m_duration - m_accumulation) : elapsed;
    m_accumulation += elapsed;
    m_trans()->rot += ( m_displasement * ( elapsed / m_duration ) );
}