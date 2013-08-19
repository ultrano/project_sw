//
//  SWActDelay.h
//  protopang
//
//  Created by 한상운 on 13. 6. 8..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWActDelay_h
#define protopang_SWActDelay_h

#include "SWAction.h"

class SWActDelay : public SWAction
{
    SW_RTTI( SWActDelay, SWAction );

    float m_duration;
    float m_accumulation;
    
public:
    
    SWActDelay( float duration );
    
    bool isDone();
    bool onStart();
    void onUpdate( float elapsed );
    
};

#endif
