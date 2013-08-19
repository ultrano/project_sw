//
//  SWPrimitiveDrawer.h
//  prototype
//
//  Created by 한상운 on 13. 5. 23..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef prototype_SWPrimitiveDrawer_h
#define prototype_SWPrimitiveDrawer_h

#include "SWPoint2.h"
#include "SWDrawer.h"

class SWGameObject;

/**
 @brief 기본 도형을 그림. (원, 사각)
 */
class SWPrimitiveDrawer : public SWDrawer
{
    SW_RTTI( SWPrimitiveDrawer, SWDrawer )
public:
    
    
    int m_primitiveType;  //< SW_Circle, SW_Rectangle, SW_Line, SW_FillRectanble
    
    SWPoint2 m_offset;    //< 그릴시에 위치 오프셋
    
    float m_radius, m_width, m_height; //< 원, 사각 정보.
        
    SWPoint2 m_begin, m_end; //< 선 정보.
    
    unsigned char m_red, m_green, m_blue; //< 색정보
   
public:
    
    SWPrimitiveDrawer();
    SWPrimitiveDrawer( int primType );
    
    void setPrimitiveType( int type ) { m_primitiveType = type; };
    void setOffset( float x, float y ) { m_offset = SWPoint2( x, y ); };
    void setRadius( float radius ) { m_radius = radius; };
    void setWidth( float width ) { m_width = width; };
    void setHeight( float height ) { m_height = height; };
    void setBeginPoint( const SWPoint2& begin ) { m_begin = begin; };
    void setEndPoint( const SWPoint2& end ) { m_end = end; };
    void setRGB( unsigned char r, unsigned char g, unsigned char b ) { m_red = r; m_green = g; m_blue = b; };
    
    void onDraw();
};

#endif
