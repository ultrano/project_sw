//
//  SWActNotify.h
//  protopang
//
//  Created by hahasasa on 13. 6. 9..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWActNotify_h
#define protopang_SWActNotify_h

#include "SWAction.h"

#include <string>

class SWActNotify : public SWAction
{
    SW_RTTI( SWActNotify, SWAction );
    
private:
    
    std::string         m_tag;
    SWWeakRef<SWObject> m_listener;
    SWHandler           m_func;
    bool                m_isNotified;
    
public:
    
    SWActNotify();
    SWActNotify( const std::string& tag, SWObject* listener = NULL, SWHandler func = NULL );
    
    const std::string& getTag() const { return m_tag; }
    void setTag( const char* tag ) { m_tag = tag; };
    void setListener( SWObject* listener, SWHandler func );
    
    bool isDone();
    bool onStart();
    void onUpdate( float elapsed );
};

#endif
