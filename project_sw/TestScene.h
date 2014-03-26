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
			//cam->getComponent<SWTransform>()->setLocalPosition( tvec3( -100, -100, 0 ) );
			//cam->perspectiveMode( SWMath.angleToRadian(120), 1, 1, 1000 );
			SWCamera::mainCamera = cam;
		}

		{
			SWGameObject* go = new SWGameObject;
			go->addUpdateDelegator( GetDelegator( onUpdateGO ) );
			WIImage* image = go->addComponent<WIImage>();
			image->setTexture( "cat3.png" );
			image->setSizeToTexture( 0.5f, 0.5f );
			SWTransform* transform = go->getComponent<SWTransform>();
			transform->setLocalPosition( TVector3f( 100,100,500 ) );

			SWAct* act = new SWActRepeat( new SWActRotateBy( 3, tvec3( 0, 0, SWMath.angleToRadian(360) ) ) );
			SWAction* action = go->addComponent<SWAction>();
			action->setAct( "rotation", act );
			//action->play( "rotation" );

			SWRigidBody2D* rigid = go->addComponent<SWRigidBody2D>();
			rigid->setGravityScale( tvec2::zero );
			rigid->setInertia( 1000 );
		}

	}

	void onUpdateGO( SWGameObject* go )
	{
		SWRigidBody2D* rigid = go->getComponent<SWRigidBody2D>();

		SWAction* action = go->getComponent<SWAction>();
		if ( SWInput.getTouchState() == SW_TouchMove )
		{
			SWTransform* transform = rigid->getComponent<SWTransform>();
			tvec3 pos( SWInput.getTouchX(), SWInput.getTouchY(), 500 );
			pos = SWCamera::mainCamera()->screenToWorld( pos );
			tvec2 force = tvec2( SWInput.getDeltaX(), -SWInput.getDeltaY() );
			force = force.normal()/10;
			
			rigid->addForce( force, pos.xy() );
		}
		else if ( action->isPlaying() == false )
		{
		}
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