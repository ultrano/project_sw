//
//  SWGameScene.cpp
//  prototype
//
//  Created by 한상운 on 13. 5. 24..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWGameScene.h"
#include "SWGameObject.h"
#include "SWLog.h"

#include "SWTransform.h"
#include "SWBehavior.h"
#include "SWMeshRenderer.h"
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

	GameObjectList::iterator itor = m_roots.begin();
	for ( ; itor != m_roots.end() ; ++itor )
	{
		SWGameObject* object = (*itor)();
		if ( object->getName() == name ) return object;
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
	onUpdate( elapsed );

	m_updates = m_roots;
	GameObjectList::iterator itor = m_updates.begin();
	for ( ; itor != m_updates.end() ; ++itor )
	{
		(*itor)()->udpate();
	}
}

void SWGameScene::draw()
{
    onDraw();

	GameObjectList::iterator itor = m_roots.begin();
	for ( ; itor != m_roots.end() ; ++itor )
	{
		SWMeshRenderer* renderer = (*itor)()->getComponent<SWMeshRenderer>();
		if ( renderer ) renderer->render();
	}

}

void SWGameScene::handleEvent( int type, int x, int y )
{
    //! 객체들에 대한 처리후 씬에게도 터치 처리를 호출
    onHandleTouch( type, x, y );
}