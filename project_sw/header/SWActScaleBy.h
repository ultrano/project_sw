//
//  SWActScaleBy.h
//  protopang
//
//  Created by hahasasa on 13. 6. 9..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWActScaleBy_h
#define protopang_SWActScaleBy_h


#include "SWAction.h"
#include "SWVector2f.h"

class SWTransform;

class SWActScaleBy : public SWAction
{
    SW_RTTI( SWActScaleBy, SWAction );
    
    SWWeakRef<SWTransform> m_trans;
    SWVector2f m_displasement;

    float m_duration;
    float m_accumulation;
    
public:
    
    SWActScaleBy( const SWVector2f& disp, float duration );
    SWActScaleBy( float dispX, float dispY, float duration );
    
    bool isDone();
    bool onStart();
    void onUpdate( float elapsed );
    
};


#endif
