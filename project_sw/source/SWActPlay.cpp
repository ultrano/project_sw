//
//  SWActPlay.cpp
//  protopang
//
//  Created by 한상운 on 13. 6. 12..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWActPlay.h"
#include "SWSpriteDrawer.h"
#include "SWActor.h"
#include "SWGameObject.h"


SWActPlay::SWActPlay( const std::string& sequence, float duration )
: m_seqName( sequence )
, m_duration( duration )
, m_accumulation( 0 )
, m_injury( 0 )
, m_seq( NULL )
{
    
}

bool SWActPlay::isDone()
{
    return ( !m_drawer() || ( m_accumulation >= m_duration ) );
}

bool SWActPlay::onStart()
{
    if ( !getActor() ) return false;
    if ( !getActor()->owner() ) return false;
    m_drawer = swrtti_cast<SWSpriteDrawer>( getActor()->owner()->getDrawer() );
    if ( !m_drawer() ) return false;
    m_data = m_drawer()->getSpriteData();
    if ( !m_data() ) return false;
    m_seq = m_data()->getSequence( m_seqName.c_str() );
    m_accumulation = m_injury;
    return ( !!m_seq );
}

void SWActPlay::onUpdate( float elapsed )
{
    if ( !m_drawer() ) return;
    m_accumulation += elapsed;
    m_injury        = m_accumulation - m_duration;
    if ( m_accumulation < m_duration )
    {
        int index = ( (m_accumulation / m_duration) * m_seq->size() );
        m_drawer()->setFrameAt( m_seq->at(index) );
    }
}