#ifndef TestScene_h__
#define TestScene_h__

#include "SWGameContext.h"
#include "TMatrix4x4.h"
#include "TQuaternion.h"
#include "SWGameScene.h"
#include "SWGameObject.h"
#include "SWTransform.h"
#include "SWMeshFilter.h"
#include "SWMesh.h"
#include "TVector2f.h"
#include "SWMeshRenderer.h"
#include "SWSpriteRenderer.h"
#include "SWLog.h"
#include "SWDefines.h"
#include "SWMath.h"
#include "SWInput.h"
#include "SWParam.h"
#include "SWValue.h"
#include "SWSpriteData.h"
#include "SWActContinue.h"
#include "SWFileStream.h"
#include "SWUtil.h"
#include "SWCamera.h"
#include "SWShader.h"
#include "SWSocket.h"
#include "SWAnimation.h"
#include "SWAnimationClip.h"

#include "SWAction.h"
#include "SWActMove.h"
#include "SWActRotate.h"
#include "SWActScale.h"
#include "SWActRepeat.h"
#include "SWActSequence.h"
#include "SWActDelay.h"
#include "SWActDelegate.h"
#include "SWActAnimate.h"

#include "SWPhysics2D.h"

#include "SWSprite.h"
#include "SWSpriteSequence.h"
#include "SWSpriteAnimation.h"

#include "SWRigidBody2D.h"
#include "SWCollider2D.h"

#include "WIDefines.h"
#include "WIImage.h"
#include "WIFontChar.h"
#include "WIFontData.h"
#include "WIText.h"
#include "SWAssets.h"

#include "SWActAlphaTo.h"

#include "SWObjectStream.h"
#include "SWByteBufferStream.h"

#include <stdio.h>
#include <set>
#include <map>
#include <fstream>
#include <locale>
#include <codecvt>


class TestScene : public SWGameScene
{
	SW_RTTI( TestScene, SWGameScene );

	TMatrix4x4 mat;
public:
	TestScene(){}
	TestScene( factory_constructor )
	{

	}
	void onAwake()
	{
		SW_GC.registerFactory<TestScene>();

		//! set primary camera
		{
			tvec3 screenSize( SW_GC.getScreenWidth(), SW_GC.getScreenHeight(), 0 );
			SWGameObject* go = new SWGameObject;
			go->setName( "mainCam" );
			SWCamera* cam = go->addComponent<SWCamera>();
			cam->orthoMode( screenSize.x, screenSize.y, 1, 1000 );
			cam->getComponent<SWTransform>()->setLocalPosition( tvec3( 0, 0, -100 ) );
			cam->setClearFlags( SW_Clear_Color );
			//cam->perspectiveMode( SWMath.angleToRadian(120), 1, 1, 1000 );
		}

		{
			SWGameObject* go = new SWGameObject();
			go->setName( "origin" );
			SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
			SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "boom.png" );
			renderer->setSprite( atlas()->find( "boom_0" ) );

			SWCircleCollider2D* collider = go->addComponent<SWCircleCollider2D>();
			collider->setRadius( 25 );

			SWGameObject* go2 = swrtti_cast<SWGameObject>( go->clone()() );
			go2->getComponent<SWTransform>()->setPosition( tvec3( 0, 100, 0 ) );
			
			SWRigidBody2D* body = go2->addComponent<SWRigidBody2D>();
			body->setGravityScale( tvec2::zero );
		}
	}

	void onUpdate()
	{
		if ( SWInput.getTouchState() != SW_TouchMove ) return;
		SWCamera* cam = findGO( "mainCam" )->getComponent<SWCamera>();
		tvec3 pos = cam->screenToWorld( tvec3( SWInput.getTouchX(), SWInput.getTouchY(), 500 ) );
		
		SWGameObject* go = findGO( "origin" );
		SWTransform* trans = go->getComponent<SWTransform>();
		trans->setPosition( pos );
	}

	void onPostDraw()
	{
	}
};

#endif // TestScene_h__