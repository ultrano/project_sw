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

SWGameScene::SWGameScene()
{
    
}

SWGameScene::~SWGameScene()
{
	SWLog( "game scene", "deleted" );
}

SWGameObject* SWGameScene::findGO( const char *name )
{
	SWObject::List::iterator itor = m_roots.begin();
	for ( ; itor != m_roots.end() ; ++itor )
	{
		SWGameObject* object = swrtti_cast<SWGameObject>( (*itor)() );
		if ( object->getName() == name ) return object;
	}
	return NULL;
}

void SWGameScene::reserveDestroy( const SWGameObject* go )
{
	m_destroyGOs.push_back( go );
}

void SWGameScene::awake()
{
	SWInput.addInputDelegate( GetDelegator(handleEvent) );
	onAwake();
}

void SWGameScene::destroy()
{
	onDestroy();

	m_updates = m_roots;
	SWObject::List::iterator itor = m_updates.begin();
	for ( ; itor != m_updates.end() ; ++itor )
	{
		SWGameObject* go = swrtti_cast<SWGameObject>( (*itor)() );
		go->destroyNow();
	}

	m_updates.clear();
	m_renderers.clear();
	m_destroyGOs.clear();

	SWInput.removeInputDelegate( GetDelegator(handleEvent) );
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

	//! regular update
	{
		m_updates = m_roots;
		SWObject::List::iterator itor = m_updates.begin();
		for ( ; itor != m_updates.end() ; ++itor )
		{
			SWGameObject* go = swrtti_cast<SWGameObject>( (*itor)() );
			go->udpate();
		}
	}

	//! post destroy game objects
	do
	{
		m_updates = m_destroyGOs;
		m_destroyGOs.clear();
		SWObject::List::iterator itor = m_updates.begin();
		for ( ; itor != m_updates.end() ; ++itor )
		{
			SWGameObject* go = swrtti_cast<SWGameObject>( (*itor)() );
			if ( !go ) continue;
			go->destroyNow();
		}
	} while ( m_destroyGOs.size() != 0 );
}

void SWGameScene::draw()
{
	SWObject::List::iterator itor = m_renderers.begin();
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