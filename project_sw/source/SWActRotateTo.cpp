//
//  SWActRotateTo.cpp
//  protopang
//
//  Created by 한상운 on 13. 6. 10..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWActRotateTo.h"
#include "SWActor.h"
#include "SWGameObject.h"
#include "SWTransform.h"

SWActRotateTo::SWActRotateTo( float dest, float duration )
: m_destination( dest )
, m_gap( 0 )
, m_duration( duration )
, m_accumulation( 0 )
{
    
}

bool SWActRotateTo::isDone()
{
    return ( m_accumulation >= m_duration );
}

bool SWActRotateTo::onStart()
{
    if ( !getActor() ) return false;
    if ( !getActor()->owner() ) return false;
    m_trans = getActor()->owner()->getTransform();
    if ( !m_trans() ) return false;
    m_accumulation = 0;
    m_gap = m_destination - m_trans()->rot;
    return true;
}

void SWActRotateTo::onUpdate( float elapsed )
{
    if ( !m_trans.isValid() ) return;
    if ( isDone() ) return;
    
    elapsed = ( ( m_accumulation + elapsed ) > m_duration )? (m_duration - m_accumulation) : elapsed;
    m_accumulation += elapsed;
    m_trans()->rot += ( m_gap * ( elapsed / m_duration ) );
}


