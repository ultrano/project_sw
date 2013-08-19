//
//  SWActAlpha.h
//  protopang
//
//  Created by 한상운 on 13. 6. 8..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWActAlpha_h
#define protopang_SWActAlpha_h

#include "SWAction.h"

class SWDrawer;
class SWActAlpha : public SWAction
{
    SW_RTTI( SWActAlpha, SWAction );
    
    SWWeakRef<SWDrawer> m_drawer;
    
    float m_begin;
    float m_gap;
    
    float m_duration;
    float m_accumulation;
    
public:
    
    SWActAlpha( float begin, float end, float duration );
    
    bool isDone();
    bool onStart();
    void onUpdate( float elapsed );

};

#endif
