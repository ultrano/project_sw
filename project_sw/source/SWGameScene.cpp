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
#include "SWMesh.h"
#include "SWGameContext.h"

SWGameScene::SWGameScene()
{
    
}

SWGameScene::~SWGameScene()
{
	SW_OutputLog( "game scene", "deleted" );
}

SWGameObject* SWGameScene::find( const char *query )
{
	return m_root()->find( query );
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
}

void SWGameScene::handleEvent( int type, int x, int y )
{
    //! 객체들에 대한 처리후 씬에게도 터치 처리를 호출
    onHandleTouch( type, x, y );
}