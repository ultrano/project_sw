//
//  SWActAlpha.cpp
//  protopang
//
//  Created by 한상운 on 13. 6. 8..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWActAlpha.h"
#include "SWActor.h"
#include "SWGameObject.h"
#include "SWDrawer.h"

SWActAlpha::SWActAlpha( float begin, float end, float duration )
: m_begin( begin )
, m_gap( end - begin )
, m_duration( duration )
, m_accumulation( 0 )
{
    
}

bool SWActAlpha::isDone()
{
    return ( m_accumulation >= m_duration );
}

bool SWActAlpha::onStart()
{
    if ( !getActor() ) return false;
    if ( !getActor()->owner() ) return false;
    m_drawer = swrtti_cast<SWDrawer>( getActor()->owner()->getDrawer() );
    if ( !m_drawer() ) return false;
    m_accumulation = 0;
    return true;
}

void SWActAlpha::onUpdate( float elapsed )
{
    if ( !m_drawer() ) return;
    
    elapsed = ( ( m_accumulation + elapsed ) > m_duration )? (m_duration - m_accumulation) : elapsed;
    m_accumulation += elapsed;
    m_drawer()->setAlpha( m_begin + ( m_gap * ( m_accumulation / m_duration ) ) );
}