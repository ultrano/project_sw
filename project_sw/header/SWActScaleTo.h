//
//  SWActScaleTo.h
//  protopang
//
//  Created by hahasasa on 13. 6. 9..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWActScaleTo_h
#define protopang_SWActScaleTo_h

#include "SWAction.h"
#include "SWVector2f.h"

class SWTransform;

class SWActScaleTo : public SWAction
{
    SW_RTTI( SWActScaleTo, SWAction );
    
    SWWeakRef<SWTransform> m_trans;
    SWVector2f m_destination;
    SWVector2f m_gap;
    float m_duration;
    float m_accumulation;
    
public:
    
    SWActScaleTo( const SWVector2f& dest, float duration );
    SWActScaleTo( float destX, float destY, float duration );
    bool isDone();
    bool onStart();
    void onUpdate( float elapsed );
    
};


#endif
