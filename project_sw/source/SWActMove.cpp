//
//  SWActMove.cpp
//  protopang
//
//  Created by 한상운 on 13. 6. 8..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWActMove.h"
#include "SWActor.h"
#include "SWGameObject.h"
#include "SWTransform.h"

SWActMove::SWActMove( const SWVector2f& begin, const SWVector2f& end, float duration )
: m_begin( begin )
, m_gap( end - begin )
, m_duration( duration )
, m_accumulation( 0 )
{
    
}

bool SWActMove::isDone()
{
    return ( m_accumulation >= m_duration );
}

bool SWActMove::onStart()
{
    if ( !getActor() ) return false;
    if ( !getActor()->owner() ) return false;
    m_trans = getActor()->owner()->getTransform();
    if ( !m_trans() ) return false;
    m_accumulation = 0;
    return true;
}

void SWActMove::onUpdate( float elapsed )
{
    if ( !m_trans.isValid() ) return;
    if ( isDone() ) return;
    m_accumulation += elapsed;
    m_trans()->pos = m_begin + ( m_gap * ( m_accumulation / m_duration ) );
}