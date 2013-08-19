//
//  SWGameScene.cpp
//  prototype
//
//  Created by 한상운 on 13. 5. 24..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWGameScene.h"
#include "SWGameObject.h"
#include "SWCollisionManager.h"
#include "SWLog.h"

#include "SWTransform.h"
#include "SWBehavior.h"
#include "SWDrawer.h"
#include "SWCollider.h"
#include "SWTouchSensor.h"
#include "SWCollisionInfo.h"
#include "SWGameContext.h"

SWGameScene::SWGameScene()
: m_maxID( 0 )
{
    
}

SWGameScene::~SWGameScene()
{
    SW_OutputLog( "game scene", "deleted" );
}

SWGameObject* SWGameScene::createEmptyObject()
{
    SWGameObject* object = new SWGameObject( ++m_maxID );
    object->setScene( this );
    m_objects.push_back( object );
    return object;
}
SWGameObject* SWGameScene::findAsName( const char *name )
{
    GameObjectList::iterator itor = m_objects.begin();
    for ( ; itor != m_objects.end() ; ++itor )
    {
        SWGameObject* object = (*itor)();
        if ( strcmp( name, object->getName() ) == 0 ) return object;
    }
    return NULL;
}

void SWGameScene::removeObject( SWGameObject *object )
{
    if ( !object ) return;
    m_objects.remove( object );
}

void SWGameScene::awake()
{
    onAwake();
}

void SWGameScene::destroy()
{
    onDestroy();
    
    m_updates.clear();
    
    m_objects.clear();
    m_freeObjects.clear();
    
    m_transforms.clear();
    m_drawers.clear();
    m_colliders.clear();
    m_behaviors.clear();
    m_touchSensors.clear();
    
}

void SWGameScene::resume()
{
    onResume();
}

void SWGameScene::pause()
{
    onPause();
}

void SWGameScene::update( float elapsed )
{
    //! 이터레이터 붕괴를 대비해 사본을 뜸.
    //! 충돌 컴포넌트 업데이트
    m_updates = m_colliders;
    for ( ComponentList::iterator itor1 = m_updates.begin()
         ; itor1 != m_updates.end()
         ; ++itor1 )
    {
        SWCollider* collider = swrtti_cast<SWCollider>( (*itor1)() );
        collider->update( elapsed );
    }
    
    //! 충돌 컴포넌트 충돌 검사 및 반응처리
    for ( ComponentList::iterator itor1 = m_updates.begin()
         ; itor1 != m_updates.end()
         ; ++itor1 )
    {
        SWCollider* suspect1 = swrtti_cast<SWCollider>( (*itor1)() );
        
        if ( suspect1->doCollide() == false ) continue;
        if ( suspect1->owner.isValid() == false ) continue;
        
        ComponentList::iterator itor2 = itor1;
        for ( ++itor2; itor2 != m_updates.end() ; ++itor2 )
        {
            SWCollider* suspect2 = swrtti_cast<SWCollider>( (*itor2)() );
            if ( suspect2->owner.isValid() == false ) continue;
            SWCollider::collisionTest( suspect1, suspect2 );
        }
    }
    
    //! 이터레이터 붕괴를 대비해 사본을 뜸.
    //! 행동 컴포넌트 업데이트
    m_updates = m_behaviors;
    for ( ComponentList::iterator itor = m_updates.begin()
         ; itor != m_updates.end()
         ; ++itor )
    {
        SWBehavior* behavior = swrtti_cast<SWBehavior>((*itor)());
        if ( behavior ) behavior->onUpdate( elapsed );
    }
    
    //! 파생 씬의 업데이트
    onUpdate( elapsed );
}

void SWGameScene::draw()
{
    onDraw();
    //! draw 컴포넌트는 사본을 뜨지 않음.
    //! draw는 그리는 역할만을 하지.
    //! 객체의 추가 삭제같은 컨트롤을 하는 담당이 아니기 때문이다.
    for ( ComponentList::iterator itor = m_drawers.begin()
         ; itor != m_drawers.end()
         ; ++itor )
    {
        SWDrawer* drawer = swrtti_cast<SWDrawer>((*itor)());
        drawer->onDraw();
    }
    
    //! 객체들 위로 그려져야할 것들에 대한 처리를 위해
    //! 후처리 드로우 함수를 호출
    onPostDraw();
}

void SWGameScene::handleEvent( int type, int x, int y )
{
    //! 이터레이터 붕괴를 대비해 사본을 뜸.
    //! 터치 센서를 업데이트
    m_updates = m_touchSensors;
    for ( ComponentList::iterator itor = m_updates.begin()
         ; itor != m_updates.end()
         ; ++itor )
    {
        SWTouchSensor* sensor = swrtti_cast<SWTouchSensor>( (*itor)() );
        sensor->handleTouch( type, x, y );
    }
    
    //! 객체들에 대한 처리후 씬에게도 터치 처리를 호출
    onHandleTouch( type, x, y );
}
void SWGameScene::registerComponent( SWComponent *component )
{
    if ( component == NULL ) return ;
    
    //! 편의를 위해 안에서 알아서 갈라 먹도록 해놨음.
         if ( swrtti_cast<SWTransform>( component ) ) m_transforms.push_back( component );
    else if ( swrtti_cast<SWCollider>( component ) )  m_colliders.push_back( component );
    else if ( swrtti_cast<SWDrawer>( component ) )    m_drawers.push_back( component );
    else if ( swrtti_cast<SWBehavior>( component ) )  m_behaviors.push_back( component );
    else if ( swrtti_cast<SWTouchSensor>( component ) )  m_touchSensors.push_back( component );
    else SW_OutputLog( "register component", "%s is unknown component", component->queryRtti()->name );
    
}

void SWGameScene::unregisterComponent( SWComponent *component )
{
    if ( component == NULL ) return ;
    
         if ( swrtti_cast<SWTransform>( component ) ) m_transforms.remove( component );
    else if ( swrtti_cast<SWCollider>( component ) )  m_colliders.remove( component );
    else if ( swrtti_cast<SWDrawer>( component ) )    m_drawers.remove( component );
    else if ( swrtti_cast<SWBehavior>( component ) )  m_behaviors.remove( component );
    else if ( swrtti_cast<SWTouchSensor>( component ) )  m_touchSensors.remove( component );
    else SW_OutputLog( "unregister component", "%s is unknown component", component->queryRtti()->name );
    
}