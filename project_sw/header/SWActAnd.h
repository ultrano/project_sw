//
//  SWActAnd.h
//  protopang
//
//  Created by 한상운 on 13. 6. 12..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWActAnd_h
#define protopang_SWActAnd_h

#include "SWAction.h"
#include <list>

class SWActAnd : public SWAction
{
    SW_RTTI( SWActAnd, SWAction );
    typedef std::list< SWHardRef<SWAction> > ActionList;
    
    ActionList m_actions;
    bool       m_isDone;
    
public:
    
    SWActAnd( SWAction* one = NULL, SWAction* two = NULL, SWAction* tree = NULL );
    
    void addAction( SWAction* action );

    bool isDone();
    bool onStart();
    void onUpdate( float elapsed );
    
};

#endif
