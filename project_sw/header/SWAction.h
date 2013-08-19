//
//  SWAction.h
//  protopang
//
//  Created by 한상운 on 13. 6. 8..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWAction_h
#define protopang_SWAction_h

#include "SWObject.h"

class SWActor;

class SWAction : public SWObject
{
    SW_RTTI( SWAction, SWObject );
    
    SWWeakRef<SWActor> m_actor;
    bool m_isPaused;
    
public:
    
    SWAction();
    ~SWAction();
    
    void setActor( SWActor* actor );
    SWActor* getActor();

    void pause();
    void resume();
    bool isPaused() const;
    
    //! pure virtual methods
    virtual bool isDone() = 0;
    virtual bool onStart() = 0;
    virtual void onUpdate( float elapsed ) = 0;
    
};

#endif
