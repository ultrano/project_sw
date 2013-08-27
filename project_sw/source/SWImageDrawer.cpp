//
//  SWImageDrawer.cpp
//  prototype
//
//  Created by 한상운 on 13. 5. 29..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWImageDrawer.h"
#include "SWGameContext.h"
#include "SWTransform.h"
#include "SWGameObject.h"
#include "SWLog.h"
#include "SWDefines.h"

SWImageDrawer::SWImageDrawer()
: m_offsetX( 0 )
, m_offsetY( 0 )
, m_width( 0 )
, m_height( 0 )
, m_stretchSide( SW_None )
{
    
}

SWImageDrawer::SWImageDrawer( const SWImageRegion& region, float width, float height, const SWVector2f& offset )
: m_region( region )
, m_offsetX( offset.x )
, m_offsetY( offset.y )
, m_width( width )
, m_height( height )
, m_stretchSide( SW_None )
{
}

void SWImageDrawer::onDraw()
{
    if ( m_alpha <= 0 ) return;
    if ( !getGameObject.isValid() )
    {
        SW_OutputLog( "on draw", "invalid owner" );
        return;
    }

    SWTransform* trans = getGameObject()->getTransform();
    if ( trans == NULL ) return;
    
    float width  = m_width;
    float height = m_height;
    float rate   = (float)(m_region.width*height)/(float )(m_region.height*width);
    
    if ( m_stretchSide == SW_Stretch_Height )
    {
        height /= rate;
    }
    else if ( m_stretchSide == SW_Stretch_Width )
    {
        width *= rate;
    }
    else if ( m_stretchSide == SW_Stretch_Origin )
    {
        width  = m_region.width;
        height = m_region.height;
    }
    
	/*
    SW_GC.setColor( 1, 1, 1, m_alpha );
    SW_GC.setScale( trans->scale.x, trans->scale.y );
    SW_GC.setRotate( trans->rot );
    SW_GC.setTranslate( trans->pos.x, trans->pos.y );
    
    SW_GC.drawImageRegion( m_region.img
                          , m_offsetX - ( (float)width / 2.0f ), m_offsetY - ( (float)height / 2.0f )
                          , width, height
                          , m_region.x, m_region.y
                          , m_region.width, m_region.height );
						  */

}