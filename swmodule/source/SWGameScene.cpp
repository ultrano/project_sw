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
#include "SWMesh.h"
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
		for ( SWHardRef<SWRefNode> node = m_rootNode() ; node.isValid() ; )
		{
			SWGameObject* go = (SWGameObject*)node()->ref();
			node = node()->next();
			if ( !go ) continue;
			if ( go->isActiveSelf() ) go->fixedRateUpdate();
		}

		SWPhysics2D.simulate();
	}

	//! regular updates
	{
		onUpdate();
		for ( SWHardRef<SWRefNode> node = m_rootNode() ; node.isValid() ; )
		{
			SWGameObject* go = (SWGameObject*)node()->ref();
			node = node()->next();
			if ( !go ) continue;
			if ( go->isActiveSelf() ) go->udpate();
		}
	}
	
	//! post destroy game objects
	do
	{
		SWObject::Array iterateCopy;
		iterateCopy = m_destroyGOs;
		m_destroyGOs.clear();
		SWObject::Array::iterator itor = iterateCopy.begin();
		for ( ; itor != iterateCopy.end() ; ++itor )
		{
			SWGameObject* go = swrtti_cast<SWGameObject>( (*itor)() );
			if ( !go ) continue;
			go->destroyNow();
		}
	} while ( m_destroyGOs.size() != 0 );
}

void SWGameScene::draw()
{
	static tarray<tuint> proxyIDs;

	for ( SWObject::Array::iterator itor = m_renderers.begin()
		; itor != m_renderers.end()
		; ++itor)
	{
		taabb3d aabb;
		SWRenderer* renderer = (SWRenderer*)((*itor)());
		renderer->computeAABB(aabb);
		m_rendererTree.updateProxy(renderer->getProxyID(), aabb);
	}

	/*

	*/
	m_cameras.sort( CameraSorter() );

	SWObject::List::iterator itor = m_cameras.begin();
	for ( ; itor != m_cameras.end() ; ++itor )
	{
		SWCamera* camera = swrtti_cast<SWCamera>( (*itor)() );
		tflag32 cullingMask = camera->getCullingMask();

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

		taabb3d frustrumAABB;
		camera->computeFrustrumAABB(frustrumAABB);
		m_rendererTree.query(proxyIDs, frustrumAABB);

		for (int i = 0 ; i <proxyIDs.size() ; ++i)
		{
			tuint proxyID = proxyIDs[i];
			SWRenderer* renderer = (SWRenderer*)m_rendererTree.getUserData(proxyID);
			tuint layerMask = renderer->gameObject()->getLayer();
			if (cullingMask.get(layerMask)) renderer->render(camera);
		}
	}

    onPostDraw();
}

void SWGameScene::handleEvent()
{
    //! 객체들에 대한 처리후 씬에게도 터치 처리를 호출
    onHandleTouch();
}

tuint SWGameScene::addRenderer( SWRenderer* renderer )
{
	m_renderers.push_back(renderer);
	
	taabb3d aabb;
	SWMesh* mesh = renderer->getMesh();
	if (mesh != NULL) aabb = mesh->getAABB();

	return m_rendererTree.createProxy(aabb, renderer);
}

void SWGameScene::removeRenderer( SWRenderer* renderer )
{
	m_rendererTree.destroyProxy(renderer->getProxyID());
	SWObject::Array::iterator last = std::remove( m_renderers.begin(), m_renderers.end(), renderer );
	m_renderers.erase( last, m_renderers.end() );
}

void SWGameScene::addCamera( SWCamera* camera )
{
	if ( camera == NULL ) return ;
	m_cameras.push_back( camera );
}

void SWGameScene::removeCamera( SWCamera* camera )
{
	if ( camera == NULL ) return ;
	m_cameras.remove( camera );
}
