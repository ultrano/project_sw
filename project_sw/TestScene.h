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
#include "SWLog.h"
#include "SWDefines.h"
#include "SWMath.h"
#include "SWInput.h"
#include "SWParam.h"
#include "SWValue.h"
#include "SWActPlay.h"
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
#include "SWActMoveBy.h"
#include "SWActRepeat.h"
#include "SWActSequence.h"
#include "SWActDelay.h"
#include "SWActDelegate.h"
#include "SWActRotateBy.h"
#include "SWActScaleTo.h"

#include "SWRigidBody2D.h"

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
		//! matrix inverse test
		{
			tmat44 mat1, mat2;
			mat1.perspective( SWMath.angleToRadian(90), 1, 1, 1000 );
			mat1.inverse( mat2 );
			tvec3 pos( SW_GC.getScreenWidth(), SW_GC.getScreenHeight(),100 );
			pos = pos * mat1;
			pos = pos * mat2;
			pos = pos * mat1;
		}

		//! set primary camera
		{
			tvec3 screenSize( SW_GC.getScreenWidth(), SW_GC.getScreenHeight(), 0 );
			SWGameObject* go = new SWGameObject;
			SWCamera* cam = go->addComponent<SWCamera>();
			cam->orthoMode( screenSize.x, screenSize.y, 1, 1000 );
			cam->getComponent<SWTransform>()->setLocalPosition( tvec3( 0, 0, -100 ) );
			//cam->perspectiveMode( SWMath.angleToRadian(120), 1, 1, 1000 );
		}

		//! set secondary camera
		{
			tvec3 screenSize( SW_GC.getScreenWidth(), SW_GC.getScreenHeight(), 0 );
			SWGameObject* go = new SWGameObject;
			SWCamera* cam = go->addComponent<SWCamera>();
			cam->setTargetLayerName( "second" );
			cam->orthoMode( screenSize.x, screenSize.y, 1, 1000 );
			cam->getComponent<SWTransform>()->setLocalPosition( tvec3( 0, 0, -100 ) );


			SWAction* action = go->addComponent<SWAction>();
			SWActSequence* seq = new SWActSequence();
			seq->addAct( new SWActMoveBy( 2, tvec3( 100, 0, 0 ) ) );
			seq->addAct( new SWActMoveBy( 2, tvec3( -100, 0, 0 ) ) );
			action->setAct( "test", new SWActRepeat( seq ) );
			action->play( "test" );

		}

		{
			WIImage* parent = makeCatImg();
			SWTransform* trans = parent->getComponent<SWTransform>();
			trans->setLocalPosition( tvec3( 0, 0, -2 ) );
		

			WIImage* child = makeCatImg();
			child->gameObject()->setLayerName( "second" );
			child->gameObject()->setName( "child" );
			trans = child->getComponent<SWTransform>();
			trans->setLocalPosition( tvec3( 10, 10, -3 ) );
		}

		{
			WIImage* img = makeCatImg();
			SWTransform* trans = img->getComponent<SWTransform>();
			trans->setLocalPosition( tvec3( 0, 0, 0 ) );
			img->gameObject()->setActive( false );
			img->gameObject()->setName( "img" );
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

		SWRigidBody2D* body = go->addComponent<SWRigidBody2D>();
		body->setGravityScale( tvec2::zero );
		body->setInertia( 100 );

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
		if ( SWInput.getTouchState() == SW_TouchPress )
		{
			SWAction* action = go->addComponent<SWAction>();
			action->setAct( "pop", new SWActAlphaTo( 3, 0 ) );
			//action->play( "pop" );

			{
				/*
				SWHardRef<SWInputStream> is = SWAssets.loadBuffer( "test.txt" );
				SWHardRef<SWObjectReader> or = new SWObjectReader( is() );
				go = swrtti_cast<SWGameObject>( or()->readObject() );

				tvec3 pos = SWCamera::mainCamera()->screenToWorld( tvec3( SWInput.getTouchX(), SWInput.getTouchY(), 500 ) );
				go->getComponent<SWTransform>()->setPosition( pos );
				*/
			}
		}
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