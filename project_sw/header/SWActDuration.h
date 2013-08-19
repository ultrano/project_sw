//
//  SWActDuration.h
//  shakeThatAss
//
//  Created by 한상운 on 13. 6. 19..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef shakeThatAss_SWActDuration_h
#define shakeThatAss_SWActDuration_h

#include "SWAction.h"

class SWActDuration : public SWAction
{
    SW_RTTI( SWActDuration, SWAction );
    
    float m_duration;
    float m_accumulation;
    SWHardRef<SWAction> m_action;
public:
    
    SWActDuration( float duration, SWAction* action );
    bool isDone();
    bool onStart();
    void onUpdate( float elapsed );
};

#endif
