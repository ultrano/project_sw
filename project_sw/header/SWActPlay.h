//
//  SWActPlay.h
//  protopang
//
//  Created by 한상운 on 13. 6. 12..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWActPlay_h
#define protopang_SWActPlay_h

#include "SWAction.h"
#include "SWSpriteData.h"

class SWSpriteDrawer;

class SWActPlay : public SWAction
{
    SW_RTTI( SWActPlay, SWAction );
    
    float                 m_duration;
    float                 m_accumulation;
    float                 m_injury;
    
    std::string           m_seqName;
    
    SWWeakRef<SWSpriteDrawer> m_drawer;
    SWHardRef<SWSpriteData>   m_data;
    SWSpriteData::Sequence*   m_seq;
    
public:
    
    SWActPlay( const std::string& sequence, float duration );
    
    bool isDone();
    bool onStart();
    void onUpdate( float elapsed );
    
};

#endif
