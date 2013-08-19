//
//  SWSpriteDrawer.h
//  protopang
//
//  Created by 한상운 on 13. 6. 12..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWSpriteDrawer_h
#define protopang_SWSpriteDrawer_h

#include "SWImageDrawer.h"

#include <vector>

class SWSpriteData;

class SWSpriteDrawer : public SWImageDrawer
{
    SW_RTTI( SWSpriteDrawer, SWImageDrawer );
    
    SWHardRef<SWSpriteData> m_spriteData;
    float m_sizeRate;
    
public:
    
    SWSpriteDrawer( SWSpriteData* data = NULL );
    ~SWSpriteDrawer();
    
    void setSpriteData( SWSpriteData* data );
    SWSpriteData* getSpriteData() { return m_spriteData(); };
    void setFrameAt( int frameIndex );
    
};

#endif
