//
//  SWTimer.cpp
//  protopang
//
//  Created by 한상운 on 13. 6. 12..
//  Copyright (c) 2013년 한상운. All rights reserved.
//
/*
#include "SWTimer.h"

SWTimer::SWTimer( SWObject* target, SWHandler handler)
: m_interval( 0 )
, m_target( target )
, m_handler( handler )
, m_isRunning( false )
, m_accumulation( 0 )
{
    
}

void SWTimer::reStart()
{
    stop();
    start();
}

void SWTimer::start()
{
    m_isRunning = true;
}

void SWTimer::stop()
{
    pause();
    m_accumulation = 0;
}

void SWTimer::pause()
{
    m_isRunning = false;
}

void SWTimer::update( float elapsed )
{
    if ( !m_isRunning ) return;
    if ( !m_handler ) return;
    if ( !m_target() ) return;
    
    m_accumulation += elapsed;

    while ( ( m_accumulation - m_interval ) >= 0 )
    {
        m_accumulation -= m_interval;
        (m_target()->*m_handler)( this );
    }
}
*/