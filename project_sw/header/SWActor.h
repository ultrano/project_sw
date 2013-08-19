//
//  SWActor.h
//  protopang
//
//  Created by 한상운 on 13. 6. 8..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWActor_h
#define protopang_SWActor_h

#include "SWBehavior.h"

#include <list>

class SWAction;

class SWActor : public SWBehavior
{
    SW_RTTI( SWActor, SWBehavior );
    
    typedef std::list< SWHardRef<SWAction> > ActionList;
    
    bool       m_isAwake;
    ActionList m_actions;
    ActionList m_updates;
    
public:
    
    SWActor();
    ~SWActor();
    
    bool runAction( SWAction* action );
    void reRunAllActions();
    void resumeAllActions();
    void pauseAllActions();
    void stopAllActions();
    void stopAction( SWAction* action );
    bool isAllStopped();
    
    void onAwake();
    void onAsleep();
    void onUpdate( float elapsed );
    
};

#endif
