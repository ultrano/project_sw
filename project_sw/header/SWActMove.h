//
//  SWActMove.h
//  protopang
//
//  Created by 한상운 on 13. 6. 8..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWActMove_h
#define protopang_SWActMove_h

#include "SWAction.h"
#include "SWVector2f.h"

class SWTransform;

class SWActMove : public SWAction
{
    SW_RTTI( SWActMove, SWAction );
 
    SWWeakRef<SWTransform> m_trans;
    SWVector2f m_begin;
    SWVector2f m_gap;
    
    float m_duration;
    float m_accumulation;
    
public:
    
    SWActMove( const SWVector2f& begin, const SWVector2f& end, float duration );
    
    bool isDone();
    bool onStart();
    void onUpdate( float elapsed );
};

#endif
