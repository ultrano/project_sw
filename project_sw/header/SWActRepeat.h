//
//  SWActRepeat.h
//  protopang
//
//  Created by 한상운 on 13. 6. 8..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWActRepeat_h
#define protopang_SWActRepeat_h

#include "SWAction.h"

class SWActRepeat : public SWAction
{
    SW_RTTI( SWActRepeat, SWAction );
    
    SWHardRef<SWAction> m_action;
    unsigned int m_count;
    unsigned int m_maxCount;
    
    
public:
    
    SWActRepeat( unsigned int maxCount, SWAction* action );
    
    bool isDone();
    bool onStart();
    void onUpdate( float elapsed );
    
};

#endif
