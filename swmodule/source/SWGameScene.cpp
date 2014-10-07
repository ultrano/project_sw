//
//  SWGameScene.cpp
//  prototype
//
//  Created by 한상운 on 13. 5. 24..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWGameScene.h"
#include "SWOpenGL.h"
#include "SWDefines.h"
#include "SWGameObject.h"
#include "SWRefNode.h"
#include "SWLog.h"
#include "SWInput.h"
#include "SWTime.h"
#include "SWPhysics2D.h"

#include "SWTransform.h"
#include "SWBehavior.h"
#include "SWCamera.h"
#include "SWRenderer.h"
#include "SWGameLayer.h"

#include <algorithm>

struct CameraSorter
{
	bool operator()( SWObject::Ref left, SWObject::Ref right )
	{
		return ((SWCamera*)left())->getDepth() < ((SWCamera*)right())->getDepth();
	}
};

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
	for ( SWRefNode* node = m_rootNode() ; node ; node = node->next() )
	{
		object = (SWGameObject*)node->ref();
		if ( object && object->getName() == subName ) break;
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
	SWObject::Array::iterator itor = m_destroyGOs.begin();
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
	for ( SWRefNode* node = m_rootNode() ; node ; node = node->next() )
	{
		SWGameObject* go = (SWGameObject*)node->ref();
		if ( go ) go->destroyNow();
	}
	m_rootNode = NULL;

	m_iterateCopy.clear();
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
	
	//! fixed rate updates
	while ( fixedCount-- )
	{
		onFixedRateUpdate();
		for ( SWRefNode* node = m_rootNode() ; node ; )
		{
			SWGameObject* go = (SWGameObject*)node->ref();
			if ( go && go->isActiveSelf() ) go->fixedRateUpdate();

			//! make the iteration safe from the removing during iterate (lazy delete node)
			if ( node->ref.isValid() ) node = node->next();
			else while ( node && !node->ref.isValid() )
			{
				SWRefNode* next = node->next();
				SWRefNode* prev = node->prev();
				if ( next ) next->prev = prev;
				if ( prev ) prev->next = next;
				if ( m_rootNode() == node ) m_rootNode = next;
				node = next;
			}
		}

		SWPhysics2D.simulate();
	}

	//! regular updates
	{
		onUpdate();
		for ( SWRefNode* node = m_rootNode() ; node ; node = node? node->next():NULL )
		{
			SWGameObject* go = (SWGameObject*)node->ref();
			if ( go && go->isActiveSelf() ) go->udpate();

			//! make the iteration safe from the removing during iterate (lazy delete node)
			if ( node->ref.isValid() ) node = node->next();
			else while ( node && !node->ref.isValid() )
			{
				SWRefNode* next = node->next();
				SWRefNode* prev = node->prev();
				if ( next ) next->prev = prev;
				if ( prev ) prev->next = next;
				if ( m_rootNode() == node ) m_rootNode = next;
				node = next;
			}
		}
	}
	
	//! post destroy game objects
	do
	{
		m_iterateCopy = m_destroyGOs;
		m_destroyGOs.clear();
		SWObject::Array::iterator itor = m_iterateCopy.begin();
		for ( ; itor != m_iterateCopy.end() ; ++itor )
		{
			SWGameObject* go = swrtti_cast<SWGameObject>( (*itor)() );
			if ( !go ) continue;
			go->destroyNow();
		}
	} while ( m_destroyGOs.size() != 0 );
}

void SWGameScene::draw()
{
	m_cameras.sort( CameraSorter() );

	for ( tuint i = 0 ; i < MaxLayerCount ; ++i)
	{
		SWGameLayer* layer = m_layerTable[i]();
		if ( layer ) layer->update();
	}

	SWObject::List::iterator itor = m_cameras.begin();
	for ( ; itor != m_cameras.end() ; ++itor )
	{
		SWCamera* camera = swrtti_cast<SWCamera>( (*itor)() );
		tuint32 cullingMask = camera->getCullingMask();

		//! get clear mask
		int clearMask = GL_NONE;
		int clearFlags = camera->getClearFlags();
		if ( clearFlags & SW_Clear_Color ) clearMask |= GL_COLOR_BUFFER_BIT;
		if ( clearFlags & SW_Clear_Depth ) clearMask |= GL_DEPTH_BUFFER_BIT;

		//! clear buffer
		if ( clearMask != GL_NONE )
		{
			tcolor color = camera->getClearColor();
			glClearColor( color.r, color.g, color.b, color.a );
			glClearDepth( camera->getClearDepth() );
			glClear( clearMask );
		}

		for ( tuint i = 0 ; i < MaxLayerCount ; ++i)
		{
			bool doDraw = (cullingMask & 0x1);
			cullingMask = cullingMask >> 1;
			if ( !doDraw ) continue;
			SWGameLayer* layer = m_layerTable[i]();
			if ( layer ) layer->draw( camera );
		}
	}

    onPostDraw();
}

void SWGameScene::handleEvent()
{
    //! 객체들에 대한 처리후 씬에게도 터치 처리를 호출
    onHandleTouch();
}

tuint SWGameScene::addRenderer( tuint layer, SWRenderer* renderer )
{
	if ( layer >= MaxLayerCount ) return SWDynamicTree3D::nullID;

	SWGameLayer* gameLayer = getLayer(layer);
	return gameLayer->addRenderer( renderer );
}

void SWGameScene::removeRenderer( tuint layer, SWRenderer* renderer )
{
	if ( layer >= MaxLayerCount ) return;

	SWGameLayer* gameLayer = getLayer(layer);
	gameLayer->removeRenderer( renderer );
}

void SWGameScene::addCamera( tuint32 layerMask, SWCamera* camera )
{
	if ( camera == NULL ) return ;
	if ( layerMask == 0 ) return ;

	for ( tuint i = 0 ; i < MaxLayerCount ; ++i )
	{
		bool ret = (layerMask & 0x1);
		layerMask = layerMask >> 1;
		if ( !ret ) continue;
		SWGameLayer* layer =  getLayer(i);
		layer->addCamera( camera );
	}
	m_cameras.push_back( camera );
}

void SWGameScene::removeCamera( tuint32 layerMask, SWCamera* camera )
{
	if ( camera == NULL ) return ;
	if ( layerMask == 0 ) return ;

	for ( tuint i = 0 ; i < MaxLayerCount ; ++i )
	{
		bool ret = (layerMask & 0x1);
		layerMask = layerMask >> 1;
		if ( !ret ) continue;
		SWGameLayer* layer =  getLayer(i);
		layer->removeCamera( camera );
	}
	m_cameras.remove( camera );
}

void SWGameScene::updateCamera( tuint32 oldMask, tuint32 newMask, SWCamera* camera )
{
	tuint32 common  = oldMask & newMask;
	tuint32 removedMask = newMask ^ common;
	tuint32 addedMask   = oldMask ^ common;

	for ( tuint i = 0 ; i < MaxLayerCount ; ++i )
	{
		bool added   = (addedMask & 0x1);
		bool removed = (removedMask & 0x1);
		addedMask   = addedMask >> 1;
		removedMask = removedMask >> 1;

		SWGameLayer* layer =  getLayer(i);
		if ( added ) layer->addCamera( camera );
		if ( removed ) layer->removeCamera( camera );
	}
}

SWGameLayer* SWGameScene::getLayer( tuint layer )
{
	SWGameLayer* gameLayer = m_layerTable[layer]();
	if ( gameLayer == NULL )
	{
		gameLayer = new SWGameLayer;
		m_layerTable[layer] = gameLayer;
	}
	return gameLayer;
}