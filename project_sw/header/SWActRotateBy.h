//
//  SWActRotateBy.h
//  protopang
//
//  Created by 한상운 on 13. 6. 10..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWActRotateBy_h
#define protopang_SWActRotateBy_h

#include "SWAction.h"

class SWTransform;

class SWActRotateBy : public SWAction
{
    SW_RTTI( SWActRotateBy, SWAction );
    
    
    SWWeakRef<SWTransform> m_trans;
    float m_displasement;
    
    float m_duration;
    float m_accumulation;
    
public:
    
    SWActRotateBy( float disp, float duration );
    
    bool isDone();
    bool onStart();
    void onUpdate( float elapsed );
};


#endif
