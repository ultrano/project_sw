//
//  SWActSequence.h
//  protopang
//
//  Created by 한상운 on 13. 6. 8..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWActSequence_h
#define protopang_SWActSequence_h

#include "SWAction.h"

#include <vector>

class SWActSequence : public SWAction
{
    SW_RTTI( SWActSequence, SWAction );
    
    typedef std::vector< SWHardRef<SWAction> > ActionArray;
    
    ActionArray m_actions;
    int m_cursor;
    SWWeakRef<SWAction> m_current;
    
public:
    
    SWActSequence( SWAction* one = NULL, SWAction* two = NULL, SWAction* tree = NULL );
    
    void addAction( SWAction* action );
    size_t size() const;
    void   clear();
    
    bool isDone();
    bool onStart();
    void onUpdate( float elapsed );
};

#endif
