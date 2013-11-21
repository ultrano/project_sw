// project_sw.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "SWGameContext.h"
#include "SWMatrix4x4.h"
#include "SWQuaternion.h"
#include "SWGameScene.h"
#include "SWGameObject.h"
#include "SWTransform.h"
#include "SWMeshFilter.h"
#include "SWMesh.h"
#include "SWVector2f.h"
#include "SWMeshRenderer.h"
#include "SWLog.h"
#include "SWDefines.h"
#include "SWMath.h"
#include "SWInput.h"

#include "UIImage.h"

#include "Cat.h"
#include "Ball.h"
#include "BallGenerator.h"
#include "CatGenerator.h"

#include <set>
#include <map>
using namespace std;

class TestScene : public SWGameScene
{
	SW_RTTI( TestScene, SWGameScene );
	SWHardRef<SWGameObject> target;

	void noParam(){};
	void hasParam( SWGameObject* ){};
	void onAwake()
	{
		SWHandler2 h1 = SWHandler2::delimit(&SWTransform::onUpdate);
		SWHandler2 h2 = SWHandler2::delimit(&__this::hasParam);
		//! background
		{
			SWGameObject* go = new SWGameObject;
			SWTransform* transform = go->getComponent<SWTransform>();
			UIImage* image = go->addComponent<UIImage>();

			image->setSize( 960, 640 );
			image->setTexture( "background_02.png" );

			transform->setLocalPosition( SWVector3f( 480,320,0 ) );
		}

		{
			SWGameObject* go = new SWGameObject;
			go->addComponent<CatGenerator>();
		}

		{
			SWGameObject* ratGO = new SWGameObject;
			{
				ratGO->setName( "rat" );
				SWTransform* transform = ratGO->getComponent<SWTransform>();
				transform->setLocalPosition( SWVector3f( 170,550,0 ) );
			}

			{
				SWGameObject* go = new SWGameObject;
				SWTransform* transform = go->getComponent<SWTransform>();
				UIImage* image = go->addComponent<UIImage>();
				image->setTexture( "rat.png" );
				image->setSizeToTexture();
				transform->setParent( ratGO->getComponent<SWTransform>() );
				transform->setLocalPosition( SWVector3f( 0, -image->getHeight()/2, 0 ) );
			}

			{
				SWGameObject* go = new SWGameObject;
				go->addComponent<BallGenerator>();
				go->setName( "generator" );
				SWTransform* transform = go->getComponent<SWTransform>();
				transform->setParent( ratGO->getComponent<SWTransform>() );
				transform->setLocalPosition( SWVector3f( 60,-60,0 ) );
			}
		}

	}

	void OnTransformSet( SWTransform* transform )
	{
	}

	void onUpdate()
	{
		SWGameObject* ratGO = find( "rat" );
		SWTransform* ratTrans = ratGO->getComponent<SWTransform>();

		SWGameObject* go = ratTrans->find("generator")->gameObject();
		BallGenerator* gen = go->getComponent<BallGenerator>();

		switch ( SWInput.getTouchState() )
		{
		case SW_TouchRelease : gen->turnOn = false; break;
		case SW_TouchPress :   gen->turnOn = true;
		case SW_TouchMove :
			{
				SWVector3f worldPos = ratTrans->getLocalPosition() * ratTrans->getWorldMatrix();
				SWVector3f touchPos( SWInput.getTouchX(), SWInput.getTouchY(), 0 );
				SWVector2f delta = (worldPos - touchPos).xy;
				float power = delta.length();
				gen->force = delta;
				delta = delta.normalize();
				float maxRadian = SWMath.angleToRadian( 20 );
				float minRadian = SWMath.angleToRadian( -20 );
				float radian = SWMath.atan( delta.y, delta.x );
				SWLog( "angle : %f", SWMath.radianToAngle( radian ) );
				
				radian = ( radian > maxRadian )? maxRadian : ( radian < minRadian )? minRadian : radian;
				gen->force = SWVector2f( SWMath.cos(radian), SWMath.sin(radian) ) * power;
				ratTrans->setLocalRotate( SWQuaternion().rotate( SWVector3f::axisZ, radian ) );
			}
			break;
		}
	}

	void onSetParent( SWTransform* transform )
	{
		SWGameObject* go = transform->gameObject();
		SWLog( go->getName().c_str() );
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	SW_GC.onStart( new TestScene, "../resource/", 960, 640 );
	return 0;
}