//
//  SWActRemove.cpp
//  protopang
//
//  Created by hahasasa on 13. 6. 9..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWActRemove.h"
#include "SWActor.h"
#include "SWGameObject.h"
#include "SWGameScene.h"


SWActRemove::SWActRemove( float delay )
: m_delay( delay )
, m_accumulation( 0 )
{
    
}

bool SWActRemove::isDone()
{
    return ( !m_owner() && ( m_accumulation >= m_delay ) );
}

bool SWActRemove::onStart()
{
    if ( !getActor() ) return false;
    m_owner = getActor()->owner();
    if ( !m_owner() ) return false;
    m_accumulation = 0;
    return true;
}

void SWActRemove::onUpdate( float elapsed )
{
    if ( !m_owner() ) return;
    m_accumulation += elapsed;
    if ( m_accumulation < m_delay ) return;
    SWGameScene* scene = m_owner()->getScene();
    if ( !scene ) return;
    m_owner()->remove();
    m_owner = NULL;
}