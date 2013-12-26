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
#include "SWInput.h"

#include "SWTransform.h"
#include "SWBehavior.h"
#include "SWRenderer.h"
#include "SWGameContext.h"
#include "SWCamera.h"

SWGameScene::SWGameScene()
{
    
}

SWGameScene::~SWGameScene()
{
	SWLog( "game scene", "deleted" );
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
	SWInput.addInputDelegate( GetDelegate(handleEvent) );
    onAwake();
	SWGameObject* go = new SWGameObject;
	SWCamera* cam = go->addComponent<SWCamera>();
	cam->cameraMatrix.ortho( 0, SW_GC.getScreenWidth(), 0, SW_GC.getScreenHeight(),1000,-1000);
	SWCamera::mainCamera = cam;
}

void SWGameScene::destroy()
{
	onDestroy();

	m_updates = m_roots;
	GameObjectList::iterator itor = m_updates.begin();
	for ( ; itor != m_updates.end() ; ++itor )
	{
		SWGameObject* object = (*itor)();
		object->destroy();
	}

	m_updates.clear();
	m_renderers.clear();

	SWInput.removeInputDelegate( GetDelegate(handleEvent) );
	__super::destroy();
}

void SWGameScene::resume()
{
    onResume();
}

void SWGameScene::pause()
{
    onPause();
}

void SWGameScene::update()
{
	onUpdate();

	m_updates = m_roots;
	GameObjectList::iterator itor = m_updates.begin();
	for ( ; itor != m_updates.end() ; ++itor )
	{
		SWGameObject* go = (*itor)();
		go->udpate();
	}
}

void SWGameScene::draw()
{
	if ( SWCamera::mainCamera.isValid() == false ) return;
	
	SWCamera* cam = SWCamera::mainCamera();
	SWTransform* trans = cam->getComponent<SWTransform>();
	SW_GC.setViewMatrix( trans->getFinalMatrix() );
	SW_GC.setProjectionMatrix( cam->cameraMatrix );

	ComponentList::iterator itor = m_renderers.begin();
	for ( ; itor != m_renderers.end() ; ++itor )
	{
		SWRenderer* renderer = swrtti_cast<SWRenderer>((*itor)());
		renderer->preRender();
		renderer->render();
	}
	
    onPostDraw();
}

void SWGameScene::handleEvent()
{
    //! 객체들에 대한 처리후 씬에게도 터치 처리를 호출
    onHandleTouch();
}