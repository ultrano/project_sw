//
//  SWActRemove.h
//  protopang
//
//  Created by hahasasa on 13. 6. 9..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWActRemove_h
#define protopang_SWActRemove_h

#include "SWAction.h"

class SWGameObject;

class SWActRemove : public SWAction
{
    SW_RTTI( SWActRemove, SWAction );
    
    SWWeakRef<SWGameObject> m_owner;
    float m_delay;
    float m_accumulation;
    
public:
    
    SWActRemove( float delay = 0 );
    
    bool isDone();
    bool onStart();
    void onUpdate( float elapsed );
};

#endif
