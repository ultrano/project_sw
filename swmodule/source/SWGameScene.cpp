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
	m_fixedFrameRate = 1.0f/30.0f;
	m_accumFrameRate = 0;
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
	//! regular update
	onUpdate();
	{
		m_updates = m_roots;
		SWObject::List::iterator itor = m_updates.begin();
		for ( ; itor != m_updates.end() ; ++itor )
		{
			SWGameObject* go = swrtti_cast<SWGameObject>( (*itor)() );
			if ( go->isActiveSelf() ) go->udpate();
		}
	}

	//! calculate count of fixed frame update
	tuint countOfFixedRateUpdate = 0;
	m_accumFrameRate += SWTime.getDeltaTime();
	while ( m_accumFrameRate >= m_fixedFrameRate )
	{
		countOfFixedRateUpdate += 1;
		m_accumFrameRate -= m_fixedFrameRate;
	}

	//! fixed frame update
	onFixedRateUpdate();
	while ( countOfFixedRateUpdate-- )
	{
		m_updates = m_roots;
		SWObject::List::iterator itor = m_updates.begin();
		for ( ; itor != m_updates.end() ; ++itor )
		{
			SWGameObject* go = swrtti_cast<SWGameObject>( (*itor)() );
			if ( go->isActiveSelf() ) go->fixedRateUpdate();
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
	struct CameraSorter
	{
		bool operator()( SWObject::Ref left, SWObject::Ref right )
		{
			return ((SWCamera*)left())->getDepth() < ((SWCamera*)right())->getDepth();
		}
	} cameraSorter;

	struct RendererSorter
	{
		tvec3 cameraPos;
		tvec3 lookDir;
		RendererSorter( const tvec3& pos, const tvec3& look ) : cameraPos( pos ), lookDir( look ) {}
		bool operator()( SWObject::Ref left, SWObject::Ref right )
		{
			SWTransform* lTrans = ((SWComponent*)left())->getComponent<SWTransform>();
			SWTransform* rTrans = ((SWComponent*)right())->getComponent<SWTransform>();
			tvec3 lPos = lTrans->getPosition();
			tvec3 rPos = rTrans->getPosition();
			float lDepth = lookDir.dot(lPos - cameraPos);
			float rDepth = lookDir.dot(rPos - cameraPos);
			return lDepth < rDepth;
		}
	};

	//! sort cameras
	m_cameras.sort( cameraSorter );

	ttable<thashstr,SWObject::List> layers;
	
	//! extract game objects in layer
	{
		SWObject::List::iterator itor = m_renderers.begin();
		for ( ; itor != m_renderers.end() ; ++itor )
		{
			SWRenderer* renderer = swrtti_cast<SWRenderer>((*itor)());
			SWGameObject* go = renderer->gameObject();
			if ( go->isActiveInScene() )
			{
				layers[ go->getLayerName() ].push_back( renderer );
			}
		}
	}
	
	//! sort game objects and render
	{
		SWObject::List::iterator itor = m_cameras.begin();
		for ( ; itor != m_cameras.end() ; ++itor )
		{
			SWCamera* camera = swrtti_cast<SWCamera>((*itor)());
			SWTransform* transform = camera->getComponent<SWTransform>();

			//! get clear mask
			int clearMask = GL_NONE;
			if ( camera->getClearFlags() & SW_Clear_Color ) clearMask |= GL_COLOR_BUFFER_BIT;
			if ( camera->getClearFlags() & SW_Clear_Depth ) clearMask |= GL_DEPTH_BUFFER_BIT;

			//! sort objects from camera
			SWObject::List& objectList = layers[ camera->getTargetLayerName() ];
			objectList.sort( RendererSorter( transform->getPosition(), camera->getLookDir() ) );

			//! clear buffer
			if ( clearMask != GL_NONE )
			{
				tcolor color = camera->getClearColor();
				glClearColor( color.r, color.g, color.b, color.a );
				glClearDepth( camera->getClearDepth() );
				glClear( clearMask );
			}

			SWObject::List::iterator itor2 = objectList.begin();
			for ( ; itor2 != objectList.end() ; ++itor2 )
			{
				SWRenderer* renderer = swrtti_cast<SWRenderer>((*itor2)());
				renderer->preRender();
				renderer->render( camera );
			}
		}
	}

	/**/
	
    onPostDraw();
}

void SWGameScene::handleEvent()
{
    //! 객체들에 대한 처리후 씬에게도 터치 처리를 호출
    onHandleTouch();
}