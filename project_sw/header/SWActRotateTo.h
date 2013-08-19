//
//  SWActRotateTo.h
//  protopang
//
//  Created by 한상운 on 13. 6. 10..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWActRotateTo_h
#define protopang_SWActRotateTo_h


#include "SWAction.h"
#include "SWVector2f.h"

class SWTransform;

class SWActRotateTo : public SWAction
{
    SW_RTTI( SWActRotateTo, SWAction );
    
    SWWeakRef<SWTransform> m_trans;
    float m_destination;
    float m_gap;
    
    float m_duration;
    float m_accumulation;
    
public:
    
    SWActRotateTo( float dest, float duration );
    
    bool isDone();
    bool onStart();
    void onUpdate( float elapsed );
};


#endif
