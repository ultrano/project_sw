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
			SWHardRef<SWSpriteAnimation> animation = SWAssets.loadSpriteAnimation( "animation2.txt" );
			SWHardRef<SWTexture> tex = SWAssets.loadTexture( "boom.png" );
			SWGameObject* go = new SWGameObject();
			go->setName( "boom" );

			SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
			SWHardRef<SWSpriteSheet> sheet = SWAssets.loadSpriteSheet( "balls.png" );
			renderer->setSprite( sheet()->find( "ball_1" ) );

			SWTransform* trans = renderer->getComponent<SWTransform>();
			trans->setLocalPosition( tvec3( 0, 0, -2 ) );

			SWAction* action = go->addComponent<SWAction>();
			{
				SWActAnimate* act1 = new SWActAnimate(1, animation()->getSequenceByName( "boom" ) );
				SWHardRef<SWActRepeat> act2 = new SWActRepeat(act1);
				SWHardRef<SWObject> clone = act2()->clone();

				act2 = swrtti_cast<SWActRepeat>( clone() );
				action->setAct( "test", act2() );
				//action->play( "test" );
			}

			{
				SWActSequence* seq = new SWActSequence;
				seq->addAct( new SWActRotateFrom( 5, tvec3( 0,0,3.14f ) ));
				SWHardRef<SWAct> act = new SWActRepeat( seq );
				SWHardRef<SWObject> clone = act()->clone();
				action->setAct( "move", swrtti_cast<SWAct>(clone()) );
				action->play( "move" );
			}

			SWRectCollider2D* collider = go->addComponent<SWRectCollider2D>();
			collider->setCenter( tvec2::zero );
			collider->setSize( tvec2::one*100 );
		}
	}

	WIImage* makeCatImg()
	{
		SWGameObject* go = new SWGameObject;
		go->addUpdateDelegator( GetDelegator( onUpdateCat ) );
		
		WIImage* image = go->addComponent<WIImage>();
		image->setTexture( SWAssets.loadTexture("Image4.png") );
		image->setUVRect( 0,0,32,32 );
		image->setSize( 64, 64 );

		//SWRigidBody2D* body = go->addComponent<SWRigidBody2D>();
		//body->setGravityScale( tvec2::zero );
		//body->setInertia( 100 );

		SWAction* action = go->addComponent<SWAction>();
		SWActSequence* seq = new SWActSequence();
		seq->addAct( new SWActScaleTo( 1, tvec3( 1.2f, 1.2f, 1 ) ) );
		seq->addAct( new SWActScaleTo( 1, tvec3( 0.8f, 0.8f, 1 ) ) );
		action->setAct( "pumping", new SWActRepeat( seq ) );
		action->play( "pumping" );
		
		return image;
	}

	void onUpdateCat( SWGameObject* go )
	{
	}

	void onUpdate()
	{
		if ( SWInput.getKey( 'p' ) )
		{
			SWHardRef<SWObject> scene = SW_GC.newInstance( "IntroScene" );
			SW_GC.setNextScene( swrtti_cast<SWGameScene>( scene() ) );
		}

		if ( SWInput.getTouchState() == SW_TouchPress )
		{
			SWGameObject* go = findGO( "boom" );
			SWCollider2D* collider = go->getComponent<SWRectCollider2D>();
			go = findGO( "mainCam" );
			SWCamera* camera = go->getComponent<SWCamera>();
			tvec3 pos( SWInput.getTouchX(), SWInput.getTouchY(), 100 );
			pos = camera->screenToWorld( pos );
			if ( collider->containPoint( pos.xy() ) )
			{
				SWLog( "hit collider" );
			}
			//SWHardRef<SWObject> object = findGO( "img" )->clone();
			//SWGameObject* go = swrtti_cast<SWGameObject>( object() );
			//go->setActive( true );
			//tvec3 pos = SWCamera::mainCamera()->screenToWorld( tvec3( SWInput.getTouchX(), SWInput.getTouchY(), 500 ) );
			//go->getComponent<SWTransform>()->setPosition( pos );
		}
	}

	void onPostDraw()
	{
	}
};

#endif // TestScene_h__