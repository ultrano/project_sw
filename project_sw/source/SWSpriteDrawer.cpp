//
//  SWSpriteDrawer.cpp
//  protopang
//
//  Created by 한상운 on 13. 6. 12..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWSpriteDrawer.h"
#include "SWGameContext.h"
#include "SWSpriteData.h"
#include "SWTransform.h"
#include "SWGameObject.h"
#include "SWLog.h"

SWSpriteDrawer::SWSpriteDrawer( SWSpriteData* data )
: m_spriteData( data )
, m_sizeRate( 1 )
{
    setStetch( SW_None );
}

SWSpriteDrawer::~SWSpriteDrawer()
{
    
}

void SWSpriteDrawer::setSpriteData( SWSpriteData *data )
{
    m_spriteData = data;
}

void SWSpriteDrawer::setFrameAt( int frameIndex )
{
    SWSpriteData* data = NULL;
    if ( !(data = m_spriteData()) ) return;

    data->getRegion( frameIndex, m_region );
}
