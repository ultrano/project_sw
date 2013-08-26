//
//  SWTouchSensor.cpp
//  prototype
//
//  Created by 한상운 on 13. 5. 29..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWTouchSensor.h"
#include "SWGameObject.h"
#include "SWTransform.h"
#include "SWBehavior.h"
#include "SWGameScene.h"
#include "SWDefines.h"
#include "SWLog.h"


SWTouchSensor::SWTouchSensor()
: m_offset( 0, 0 )
, m_width( 0 )
, m_height( 0 )
, m_handler( NULL )
, m_isDisable( false)
{
    
}

SWTouchSensor::SWTouchSensor( SWObject* target, const SWHandler& handler
                             , const std::string& tagName
                             , float w, float h
                             , const SWVector2f& offset )
: m_target( target )
, m_handler( handler )
, m_tag( tagName )
, m_offset( 0, 0 )
, m_width( w )
, m_height( h )
, m_isDisable( false)
{
    
}

SWTouchSensor::~SWTouchSensor()
{
    //SW_OutputLog( "touch sensor", "deleted" );
}

void SWTouchSensor::setHandler( SWObject *target, const SWHandler &handler )
{
    m_target  = target;
    m_handler = handler;
}

void SWTouchSensor::handleTouch( int type, int x, int y )
{
    if ( isDisable() ) return;
    if ( !getGameObject() ) return;
    SWTransform* trans = getGameObject()->getTransform();
    if ( trans == NULL ) return;
    
    m_type = type;
    m_touchX = x;
    m_touchY = y;
    
    x -= trans->pos.x - (m_width/2)  + m_offset.x;
    y -= trans->pos.y - (m_height/2) + m_offset.y;
    
    if ( SWRect( 0, 0, m_width, m_height ).hitTest( x, y ) == SW_RectIn )
    {
        if ( m_target() && m_handler ) (m_target()->*m_handler)(this);
    }
}

void SWTouchSensor::setTag( const std::string &tag )
{
    m_tag = tag;
}

const std::string& SWTouchSensor::getTag() const
{
    return m_tag;
}

void SWTouchSensor::setWidth( float width )
{
    m_width = width;
}

void SWTouchSensor::setHeight( float height )
{
    m_height = height;
}

void SWTouchSensor::setOffset( const SWVector2f &offset )
{
    m_offset = offset;
}

float SWTouchSensor::getWidth() const
{
    return m_width;
}

float SWTouchSensor::getHeight() const
{
    return m_height;
}

const SWVector2f& SWTouchSensor::getOffset() const
{
    return m_offset;
}