//
//  SWActRotate.h
//  protopang
//
//  Created by 한상운 on 13. 6. 8..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWActRotate_h
#define protopang_SWActRotate_h

#include "SWAction.h"

class SWTransform;

class SWActRotate : public SWAction
{
    SW_RTTI( SWActRotate, SWAction );

    SWWeakRef<SWTransform> m_trans;
    float m_begin;
    float m_gap;
    float m_duration;
    float m_accumulation;
    
public:
    
    SWActRotate( float begin, float end, float duration );
    bool isDone();
    bool onStart();
    void onUpdate( float elapsed );
    
};

#endif
