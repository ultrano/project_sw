//
//  SWActMoveBy.h
//  protopang
//
//  Created by hahasasa on 13. 6. 9..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWActMoveBy_h
#define protopang_SWActMoveBy_h


#include "SWAction.h"
#include "SWVector2f.h"

class SWTransform;

class SWActMoveBy : public SWAction
{
    SW_RTTI( SWActMoveBy, SWAction );
    
    SWWeakRef<SWTransform> m_trans;
    SWVector2f m_displasement;
    
    float m_duration;
    float m_accumulation;
    
public:
    
    SWActMoveBy( const SWVector2f& disp, float duration );
    SWActMoveBy( float dispX, float dispY, float duration );
    
    bool isDone();
    bool onStart();
    void onUpdate( float elapsed );
};


#endif
