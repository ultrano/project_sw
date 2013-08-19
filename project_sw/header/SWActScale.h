//
//  SWActScale.h
//  protopang
//
//  Created by 한상운 on 13. 6. 8..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWActScale_h
#define protopang_SWActScale_h

#include "SWAction.h"
#include "SWVector2f.h"

class SWTransform;

class SWActScale : public SWAction
{
    SW_RTTI( SWActScale, SWAction );
    
    SWWeakRef<SWTransform> m_trans;
    SWVector2f m_begin;
    SWVector2f m_gap;
    float m_duration;
    float m_accumulation;
    
public:
    
    SWActScale( const SWVector2f& begin, const SWVector2f& end, float duration );
    bool isDone();
    bool onStart();
    void onUpdate( float elapsed );
    
};

#endif
