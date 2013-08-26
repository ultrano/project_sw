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
#include "SWMesh.h"
#include "SWCollider.h"
#include "SWTouchSensor.h"
#include "SWCollisionInfo.h"
#include "SWGameContext.h"

SWGameScene::SWGameScene()
{
    
}

SWGameScene::~SWGameScene()
{
	SW_OutputLog( "game scene", "deleted" );
}

SWGameObject* SWGameScene::find( const char *name )
{
	GameObjectList::iterator itor = m_objects.begin();
	for ( ; itor != m_objects.end() ; ++itor )
	{
		SWGameObject* object = (*itor)();
		if ( name == object->getName() ) return object;
    }
    return NULL;
}

void SWGameScene::awake()
{
    onAwake();
}

void SWGameScene::destroy()
{
    onDestroy();
    
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
    //! 파생 씬의 업데이트
    onUpdate( elapsed );
}

void SWGameScene::draw()
{
    onDraw();
	GameObjectList::iterator itor = m_rootObjects.begin();
	for ( ; itor != m_rootObjects.end() ; ++itor )
	{
		SWMesh* mesh = (*itor)()->getComponent<SWMesh>();
		if ( mesh ) mesh->render();
	}
	
    
}

void SWGameScene::handleEvent( int type, int x, int y )
{
    //! 객체들에 대한 처리후 씬에게도 터치 처리를 호출
    onHandleTouch( type, x, y );
}