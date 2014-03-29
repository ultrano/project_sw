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

#include "SWRigidBody2D.h"

#include "WIDefines.h"
#include "WIImage.h"
#include "WIFontChar.h"
#include "WIFontData.h"
#include "WIText.h"

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
	void onAwake()
	{

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

		//! set default camera
		{
			tvec3 screenSize( SW_GC.getScreenWidth(), SW_GC.getScreenHeight(), 0 );
			SWGameObject* go = new SWGameObject;
			SWCamera* cam = go->addComponent<SWCamera>();
			cam->orthoMode( screenSize.x, screenSize.y, 1, 1000 );
			cam->getComponent<SWTransform>()->setLocalPosition( tvec3( 0, 0, -500 ) );
			//cam->perspectiveMode( SWMath.angleToRadian(120), 1, 1, 1000 );
			SWCamera::mainCamera = cam;
		}

		{
			WIImage* parent = makeCatImg();
			SWTransform* trans = parent->getComponent<SWTransform>();
			trans->setLocalPosition( tvec3( 0, 0, 0 ) );
			//trans->setLocalRotate( tquat().rotate( 0, 0, SWMath.angleToRadian(45) ) );
			//trans->setLocalScale( tvec3( 2,0.5,1 ) );
			SWAction* action = parent->gameObject()->addComponent<SWAction>();
			SWAct* act1 = new SWActRotateBy( 5, tvec3( 0, 0, SWMath.angleToRadian(720) ) );//new SWActMoveBy( 1, tvec3( 0,100,0 ) );
			//SWAct* act2 = new SWActMoveBy( 1, tvec3( 0,-100,0 ) );
			SWActSequence* act3 = new SWActSequence();
			act3->addAct( act1 );
			//act3->addAct( act2 );
			SWAct* act = new SWActRepeat( act3 ) ;
			action->setAct( "gg", act );
			//action->play( "gg" );

			WIImage* child = makeCatImg();
			child->gameObject()->setName( "child" );
			trans = child->getComponent<SWTransform>();
			trans->setParent( parent->getComponent<SWTransform>() );
			trans->setLocalPosition( tvec3( 100, 100, 0 ) );
		}
	}

	WIImage* makeCatImg()
	{
		SWGameObject* go = new SWGameObject;
		go->addUpdateDelegator( GetDelegator( onUpdateCat ) );
		WIImage* image = go->addComponent<WIImage>();
		image->setTexture( "cat3.png" );
		image->setSizeToTexture( 0.5f, 0.5f );
		SWRigidBody2D* body = go->addComponent<SWRigidBody2D>();
		body->setGravityScale( tvec2::zero );
		body->setInertia( 100 );
		return image;
	}

	void onUpdateCat( SWGameObject* go )
	{
		//if ( go->getName() != "child" ) return;
		if ( SWInput.getTouchState() != SW_TouchMove ) return;

		tvec3 pos( SWInput.getTouchX(), SWInput.getTouchY(), 500 );
		pos = SWCamera::mainCamera()->screenToWorld( pos );

		SWTransform* trans = go->getComponent<SWTransform>();
		trans->setPosition( pos );
		trans->setRotate( tquat().rotate( tvec3::axisZ, SWMath.angleToRadian(-45) ) );

		SWRigidBody2D* body = go->getComponent<SWRigidBody2D>();
		tvec3 delta(SWInput.getDeltaX(), -SWInput.getDeltaY(), 500);
		//delta = SWCamera::mainCamera()->screenToWorld( delta );
		body->addForce( delta.xy() / 10, pos.xy() );
	}

	void onUpdate()
	{
		if ( SWInput.getKey( 'p' ) )
		{
			SW_GC.setNextScene( new TestScene );
		}
	}

	void onPostDraw()
	{
	}
};

#endif // TestScene_h__