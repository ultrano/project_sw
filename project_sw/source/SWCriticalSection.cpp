//
//  SWCriticalSection.cpp
//  protopang
//
//  Created by 한상운 on 13. 6. 4..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWCriticalSection.h"
#include "SWLog.h"

SWCriticalSection::SWCriticalSection()
: m_signal( true )
{
}

void SWCriticalSection::enter()
{
    m_signal = false;
}

void SWCriticalSection::leave()
{
    m_signal = true;
}

int  SWCriticalSection::wait()
{
    while ( !m_signal )
    {
        SW_OutputLog( "criticla section", "wait for signal" );
    }
    return 0;
}

////////////////////////////////////////////////////

SWSectionGuard::SWSectionGuard( SWCriticalSection& cs )
: m_cs( cs )
{
    m_cs.enter();
}

SWSectionGuard::~SWSectionGuard()
{
    m_cs.leave();
}