//
//  SWActEndless.h
//  protopang
//
//  Created by 한상운 on 13. 6. 10..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWActEndless_h
#define protopang_SWActEndless_h

#include "SWAction.h"

class SWActEndless : public SWAction
{
    SW_RTTI( SWActEndless, SWAction );
    
    SWHardRef<SWAction> m_action;
    
public:
    
    SWActEndless( SWAction* action );
    
    bool isDone();
    bool onStart();
    void onUpdate( float elapsed );
    
};

#endif
