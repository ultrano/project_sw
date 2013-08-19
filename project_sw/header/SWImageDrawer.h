//
//  SWImageDrawer.h
//  prototype
//
//  Created by 한상운 on 13. 5. 29..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef prototype_SWImageDrawer_h
#define prototype_SWImageDrawer_h

#include "SWDrawer.h"
#include "SWVector2f.h"
#include "SWImageRegion.h"

class SWImageDrawer : public SWDrawer
{
    SW_RTTI( SWImageDrawer, SWDrawer );
    
protected:
    
    SWImageRegion m_region;
    float m_offsetX, m_offsetY;    //< 그릴시에 위치 오프셋
    float m_width, m_height;
    int   m_stretchSide;
    
public:
    
    SWImageDrawer();
    SWImageDrawer( const SWImageRegion& region, float width, float height, const SWVector2f& offset = SWVector2f(0,0) );
    
    void setRegion( const SWImageRegion& region ) { m_region = region; };
    void setOffsetX( float x ) { m_offsetX = x; };
    void setOffsetY( float y ) { m_offsetY = y; };
    void setWidth( float width ) { m_width = width; };
    void setHeight( float height ) { m_height = height; };
    
    void setStetch( int stretch ) { m_stretchSide = stretch; };
    int  getStretch() const  { return m_stretchSide; };
    
    float getOffsetX() const { return m_offsetX; };
    float getOffsetY() const { return m_offsetY; };
    float getWidth() const { return m_width; };
    float getHeight() const { return m_height; };
    
    void onDraw();
};

#endif
