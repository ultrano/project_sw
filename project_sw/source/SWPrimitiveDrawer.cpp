//
//  SWPrimitiveDrawer.cpp
//  prototype
//
//  Created by 한상운 on 13. 5. 23..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWPrimitiveDrawer.h"
#include "SWGameContext.h"
#include "SWGameObject.h"
#include "SWTransform.h"

SWPrimitiveDrawer::SWPrimitiveDrawer()
: m_offset( 0, 0 )
, m_radius( 0 )
, m_width( 0 )
, m_height( 0 )
, m_begin( 0, 0 )
, m_end( 0,0 )
, m_red(255), m_green(255), m_blue(255)
{
    
}

SWPrimitiveDrawer::SWPrimitiveDrawer( int primType )
: m_primitiveType( primType )
, m_offset( 0, 0 )
, m_radius( 0 )
, m_width( 0 )
, m_height( 0 )
, m_begin( 0, 0 )
, m_end( 0,0 )
, m_red( 255 ), m_green( 255 ), m_blue( 255 )
{
    
}

void SWPrimitiveDrawer::onDraw()
{
    SWTransform* trans = owner()->getTransform();
    if ( trans == NULL ) return;
    
    int rgb = SW_GC.getRGB();
    SW_GC.setRGB( m_red, m_green, m_blue );
    SW_GC.setAlpha( m_alpha );
    SW_GC.setScale( trans->scale.x, trans->scale.y );
    SW_GC.setRotate( trans->rot );
    SW_GC.setTranslate( trans->pos.x , trans->pos.y);
    
    switch ( m_primitiveType )
    {
        case SW_Circle :
            SW_GC.drawPrimitiveCircle( m_offset.x
                                      , m_offset.y
                                      , m_radius );
            break;
            
        case SW_Rectangle :
            SW_GC.drawPrimitiveRect( -( m_width / 2.0f ) + m_offset.x, -( m_height / 2.0f ) + m_offset.y, m_width, m_height );
            break;
            
        case SW_Line :
            SW_GC.drawLine( m_begin.x + trans->pos.x, m_begin.y + trans->pos.y, m_end.x + trans->pos.x, m_end.y + trans->pos.y );
            break;
            
        case SW_FillRectangle :
            SW_GC.drawFillRect( -( m_width / 2.0f ) + m_offset.x, -( m_height / 2.0f ) + m_offset.y, m_width, m_height );
            break;
    }
    SW_GC.setRGB( rgb );
}