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
#include "SWOpenGL.h"
#include "SWDefines.h"
#include "SWTime.h"
#include "SWPhysics2D.h"

#include "SWTransform.h"
#include "SWBehavior.h"
#include "SWCamera.h"
#include "SWRenderer.h"

#include <algorithm>

SWGameScene::SWGameScene()
{
    
}

SWGameScene::~SWGameScene()
{
	SWLog( "game scene", "deleted" );
}

SWGameObject* SWGameScene::findGO( const tstring& name )
{
	tuint offset = name.find( '/' );
	tstring subName = ( offset != tstring::npos )? name.substr( 0, offset ) : name;
	
	SWGameObject* object = NULL;
	SWObject::List::iterator itor = m_roots.begin();
	for ( ; itor != m_roots.end() ; ++itor )
	{
		object = swrtti_cast<SWGameObject>( (*itor)() );
		if ( object->getName() == subName ) break;
		object = NULL;
	}

	if ( object != NULL && ( offset != tstring::npos ) )
	{
		offset += 1;
		SWTransform* trans = object->getComponent<SWTransform>();
		trans = trans->find( name.substr( offset, name.size() - offset ) );
		object = ( trans != NULL )? trans->gameObject() : NULL;
	}
	return object;
}

void SWGameScene::reserveDestroy( const SWGameObject* go )
{
	SWObject::List::iterator itor = m_destroyGOs.begin();
	for ( ; itor != m_destroyGOs.end() ; ++itor )
	{
		if ( go == (*itor)() ) return;
	}
	m_destroyGOs.push_back( go );
}

void SWGameScene::awake()
{
	m_fixedFrameRate = 1.0f/30.0f;
	m_accumFrameRate = 0;
	m_physicsFrameRate = 0;
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
	//! calculate count of fixed rate update
	tuint fixedCount = 0;
	m_physicsFrameRate += SWTime.getDeltaTime();
	while ( m_physicsFrameRate >= SWPhysics2D.getFixedInterval() )
	{
		m_physicsFrameRate -= SWPhysics2D.getFixedInterval();		
		fixedCount += 1;
	}

	//! copy objects to update
	m_updates = m_roots;
	
	/*//! pre-updates
	{	
		onUpdate();

		SWObject::List::iterator itor = m_updates.begin();
		for ( ; itor != m_updates.end() ; ++itor )
		{
			SWGameObject* go = swrtti_cast<SWGameObject>( (*itor)() );
			if ( go == NULL ) continue;
			if ( go->isActiveSelf() ) go->preUpdate();
		}
	}
	/*/
	
	//! fixed rate updates
	while ( fixedCount-- )
	{
		onFixedRateUpdate();

		m_updates = m_roots;
		SWObject::List::iterator itor = m_updates.begin();
		for ( ; itor != m_updates.end() ; ++itor )
		{
			SWGameObject* go = swrtti_cast<SWGameObject>( (*itor)() );
			if ( go == NULL ) continue;
			if ( go->isActiveSelf() ) go->fixedRateUpdate();
		}

		SWPhysics2D.simulate();
	}

	//! regular updates
	{	
		onUpdate();

		SWObject::List::iterator itor = m_updates.begin();
		for ( ; itor != m_updates.end() ; ++itor )
		{
			SWGameObject* go = swrtti_cast<SWGameObject>( (*itor)() );
			if ( go == NULL ) continue;
			if ( go->isActiveSelf() ) go->udpate();
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

	//* 

	//*/

    onPostDraw();
}

void SWGameScene::handleEvent()
{
    //! 객체들에 대한 처리후 씬에게도 터치 처리를 호출
    onHandleTouch();
}

tuint SWGameScene::addRenderer( SWRenderer* renderer )
{
	return 0;
}

void SWGameScene::removeRenderer( SWRenderer* renderer )
{
}

tuint SWGameScene::addCamera( SWCamera* camera )
{
	if ( camera == NULL ) return m_cameraTree.nullID;

	taabb3d aabb;
	camera->computeFrustrumAABB( aabb );
	tuint proxyID = m_cameraTree.createProxy( aabb, camera );

	if ( proxyID != m_cameraTree.nullID ) m_cameras.push_back( camera );
	else SWLog( "failed to create camera proxy aabb into dynamic tree" );

	return proxyID;
}

void SWGameScene::removeCamera( SWCamera* camera )
{
	if ( camera == NULL ) return;

	tuint proxyID = camera->getProxyID();
	if ( proxyID == m_cameraTree.nullID ) return;

	m_cameraTree.destroyProxy( proxyID );
	m_cameras.remove( camera );
}

SWGameLayer* SWGameScene::getLayer( const tstring& name )
{
	LayerTable::iterator itor = m_layerTable.find( name );
	if ( itor == m_layerTable.end() )
	{
	}
}
