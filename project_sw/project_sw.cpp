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
	void onAwake()
	{

		SWVector3f vertices[] = { SWVector3f(-0.5f,-0.5f,0), SWVector3f(0.5f,-0.5f,0), SWVector3f(-0.5f,0.5f,0), SWVector3f(0.5f,0.5f,0) };
		SWVector2f texCoords[] = { SWVector2f(0,0), SWVector2f(1,0), SWVector2f(0,1), SWVector2f(1,1) };
		unsigned short indices[] = {0,1,2,3,2,1};
		SWMesh* mesh = new SWMesh();
		mesh->setVertexStream( 4, &vertices[0] );
		mesh->setTexCoordStream( 4, &texCoords[0]);
		mesh->setIndexStream( 6, &indices[0] );

		SW_GC.storeItem("unitRectMesh", mesh);

		{
			SWGameObject* go = new SWGameObject;
			go->addComponent<SWMeshFilter>()->setMesh( mesh );
			go->addComponent<SWMeshRenderer>()->setTexture( SW_GC.loadTexture("background_02.png") );
			SWTransform* transform = go->getComponent<SWTransform>();
			transform->setLocalScale( SWVector3f(800,600,0) );
			transform->setLocalPosition( SWVector3f( 400,300,0 ) );
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
				transform->setLocalPosition( SWVector3f( 170,500,0 ) );
				transform->setLocalScale( SWVector3f( 50,50,0 ) );
			}

			{
				SWGameObject* go = new SWGameObject;
				SWTransform* transform = go->getComponent<SWTransform>();
				go->addComponent<SWMeshRenderer>()->setTexture( SW_GC.loadTexture( "rat.png" ) );
				go->addComponent<SWMeshFilter>()->setMesh(mesh);
				transform->setParent( ratGO->getComponent<SWTransform>() );
				transform->setLocalPosition( SWVector3f( 0.1, -0.35, 0 ) );
			}

			{
				SWGameObject* go = new SWGameObject;
				go->addComponent<BallGenerator>();
				go->setName( "generator" );
				SWTransform* transform = go->getComponent<SWTransform>();
				transform->setParent( ratGO->getComponent<SWTransform>() );
				transform->setLocalPosition( SWVector3f( 0.7,-0.25,0 ) );
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
	SW_GC.onStart( new TestScene, "../resource/", 800, 600 );
	return 0;
}