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
#include "SWAction.h"
#include "SWActContinue.h"
#include "SWFileStream.h"
#include "SWUtil.h"
#include "SWCamera.h"
#include "SWShader.h"
#include "SWSocket.h"
#include "SWAnimation.h"
#include "SWAnimationClip.h"

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
		{
			SWHardRef<SWSocket> sock = new SWSocket;
			
			sock()->connect( "127.0.0.1", 10000 );
			
			SWHardRef<SWOutputStream> os = sock()->getOutputStream();
			SWHardRef<SWInputStream>  is = sock()->getInputStream();

			char buf[] = "are you there?";
			os()->write( (tbyte*)buf, sizeof(buf) );

			memset( buf, 0, sizeof(buf) );

			is()->read( (tbyte*)buf, sizeof(buf) );

			SWLog( (char*)buf );
		}
		//! set default camera
		{
			SWGameObject* go = new SWGameObject;
			SWCamera* cam = go->addComponent<SWCamera>();
			cam->orthoMode( SW_GC.getScreenWidth(), SW_GC.getScreenHeight(), 1, 1000 );
			cam->perspectiveMode( SWMath.angleToRadian(90), 1, 1, 1000 );
			SWCamera::mainCamera = cam;
		}

		//! euler test
		
			tquat a;
			a.rotate( tvec3::axisY, SWMath.angleToRadian( 180 ) );
			tvec3 euler = a.toEulerAngle();
			tvec3 angle;
			angle.x = SWMath.radianToAngle( euler.x );
			angle.y = SWMath.radianToAngle( euler.y );
			angle.z = SWMath.radianToAngle( euler.z );
			tquat b;
			b.fromEulerAngle( euler );

		{
			SWGameObject* go = new SWGameObject;
			WIImage* image = go->addComponent<WIImage>();
			image->setTexture( "cat3.png" );
			image->setSizeToTexture();
			SWTransform* transform = go->getComponent<SWTransform>();
			transform->setLocalPosition( TVector3f( 100,100,500 ) );
		
			SWAnimationClip* clip = new SWAnimationClip;
			//clip->addLine( SWTransform::getRtti(), "position.x", SWAnimationLine::Linear( 0,5,0,100 ) );
			//clip->addLine( SWTransform::getRtti(), "position.y", SWAnimationLine::Linear( 0,3,0,100 ) );
			//clip->addLine( SWTransform::getRtti(), "rotation.x", SWAnimationLine::Linear( 0,1,0,45 ) );
			//clip->addLine( SWTransform::getRtti(), "rotation.z", SWAnimationLine::Linear( 0,6,0,360 ) );
			clip->addLine( SWTransform::getRtti(), "rotation.y", SWAnimationLine::Linear( 0,4,0,91 ) );
			SWAnimation* anim = go->addComponent<SWAnimation>();
			anim->addClip( "test", clip );
			anim->play( "test" );
		}
	}

	void onHandleTouch()
	{
	}

	void onUpdate()
	{
	}

	void onPostDraw()
	{
	}
};

#endif // TestScene_h__