//
//  SWGameObject.cpp
//  prototype
//
//  Created by 한상운 on 13. 5. 23..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWGameObject.h"
#include "SWGameScene.h"
#include "SWTransform.h"
#include "SWDrawer.h"
#include "SWBehavior.h"
#include "SWCollider.h"
#include "SWTouchSensor.h"
#include "SWLog.h"

SWGameObject::SWGameObject( unsigned allocId )
: m_id( allocId )
, m_scene( NULL )
, m_transform( NULL )
, m_drawer( NULL )
, m_behavior( NULL )
, m_collider( NULL )
{
    
}

SWGameObject::~SWGameObject()
{
    if ( m_scene.isValid() )
    {
        m_scene()->unregisterComponent( m_transform() );
        m_scene()->unregisterComponent( m_drawer() );
        m_scene()->unregisterComponent( m_collider() );
        m_scene()->unregisterComponent( m_behavior() );
        m_scene()->unregisterComponent( m_touchSensor() );
    }
    m_behavior = NULL;
    m_touchSensor = NULL;
    m_collider = NULL;
    m_drawer = NULL;
    m_transform = NULL;
    m_scene = NULL;
    //SW_OutputLog("game object", "deleted" );
}

void SWGameObject::awake()
{
    
    if ( m_transform() )
    {
        m_transform()->owner = this;
        m_scene()->registerComponent( m_transform() );
    }
    if ( m_drawer() )
    {
        m_drawer()->owner = this;
        m_scene()->registerComponent( m_drawer() );
    }
    if ( m_collider() )
    {
        m_collider()->owner = this;
        m_scene()->registerComponent( m_collider() );
    }
    if ( m_touchSensor() )
    {
        m_touchSensor()->owner = this;
        m_scene()->registerComponent( m_touchSensor() );
    }
    if ( m_behavior() )
    {
        m_behavior()->owner = this;
        m_scene()->registerComponent( m_behavior() );
        m_behavior()->onAwake();
    }
}

void SWGameObject::remove()
{
    if ( m_scene.isValid() )
    {
        if ( m_behavior() ) m_behavior()->onAsleep();
        m_scene()->unregisterComponent( m_behavior() );
        m_scene()->unregisterComponent( m_touchSensor() );
        m_scene()->unregisterComponent( m_collider() );
        m_scene()->unregisterComponent( m_drawer() );
        m_scene()->unregisterComponent( m_transform() );
    }
    
    if ( m_scene.isValid() ) m_scene()->removeObject( this );
}

void SWGameObject::setScene( SWGameScene* scene )
{
    if ( m_scene.isValid() == false )
    {
        m_scene = scene;
    }
    else
    {
        SW_OutputLog( "game object", "already has scene" );
    }
}

void SWGameObject::setTransform( SWTransform* component )
{
    m_transform = swrtti_cast<SWTransform>( component );
}

void SWGameObject::setDrawer( SWDrawer* component )
{
    m_drawer = swrtti_cast<SWDrawer>( component );
}

void SWGameObject::setBehavior( SWBehavior *component )
{
    m_behavior = swrtti_cast<SWBehavior>( component );
}

void SWGameObject::setCollider( SWCollider *component )
{
    m_collider = swrtti_cast<SWCollider>( component );
}

void SWGameObject::setTouchSensor( SWTouchSensor *component )
{
    m_touchSensor = swrtti_cast<SWTouchSensor>( component );
}
