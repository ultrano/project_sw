//
//  SWActMoveTo.cpp
//  protopang
//
//  Created by hahasasa on 13. 6. 9..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWActMoveTo.h"
#include "SWActor.h"
#include "SWGameObject.h"
#include "SWTransform.h"

SWActMoveTo::SWActMoveTo( const SWVector2f& dest, float duration )
: m_destination( dest )
, m_gap( 0, 0 )
, m_duration( duration )
, m_accumulation( 0 )
{
    
}

SWActMoveTo::SWActMoveTo( float destX, float destY, float duration )
: m_destination( destX, destY )
, m_gap( 0, 0 )
, m_duration( duration )
, m_accumulation( 0 )
{
    
}

bool SWActMoveTo::isDone()
{
    return ( m_accumulation >= m_duration );
}

bool SWActMoveTo::onStart()
{
    if ( !getActor() ) return false;
    if ( !getActor()->owner() ) return false;
    m_trans = getActor()->owner()->getTransform();
    if ( !m_trans() ) return false;
    m_accumulation = 0;
    m_gap = m_destination - m_trans()->pos;
    return true;
}

void SWActMoveTo::onUpdate( float elapsed )
{
    if ( !m_trans.isValid() ) return;
    if ( isDone() ) return;
    
    elapsed = ( ( m_accumulation + elapsed ) > m_duration )? (m_duration - m_accumulation) : elapsed;
    m_accumulation += elapsed;
    m_trans()->pos += ( m_gap * ( elapsed / m_duration ) );
}