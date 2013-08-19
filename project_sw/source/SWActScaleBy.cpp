//
//  SWActScaleBy.cpp
//  protopang
//
//  Created by hahasasa on 13. 6. 9..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWActScaleBy.h"
#include "SWActor.h"
#include "SWGameObject.h"
#include "SWTransform.h"

SWActScaleBy::SWActScaleBy( const SWVector2f& disp, float duration )
: m_displasement( disp )
, m_duration( duration )
, m_accumulation( 0 )
{
    
}

SWActScaleBy::SWActScaleBy( float dispX, float dispY, float duration )
: m_displasement( dispX, dispY )
, m_duration( duration )
, m_accumulation( 0 )
{
    
}

bool SWActScaleBy::isDone()
{
    return ( m_accumulation >= m_duration );
}

bool SWActScaleBy::onStart()
{
    if ( !getActor() ) return false;
    if ( !getActor()->owner() ) return false;
    m_trans = getActor()->owner()->getTransform();
    if ( !m_trans() ) return false;
    m_accumulation = 0;
    return true;
}

void SWActScaleBy::onUpdate( float elapsed )
{
    if ( !m_trans.isValid() ) return;
    if ( isDone() ) return;

    elapsed = ( ( m_accumulation + elapsed ) > m_duration )? (m_duration - m_accumulation) : elapsed;
    m_accumulation += elapsed;
    m_trans()->scale += ( m_displasement * ( elapsed / m_duration ) );
}
