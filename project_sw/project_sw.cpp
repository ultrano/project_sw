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

#include "Cat.h"
#include "Ball.h"

#include <set>
#include <map>
using namespace std;

class TestScene : public SWGameScene
{
	SW_RTTI( TestScene, SWGameScene );
	SWHardRef<SWGameObject> target;
	float accum;
	void onAwake()
	{
		SWGameObject* go = new SWGameObject;
		SWMeshRenderer* renderer = go->addComponent<SWMeshRenderer>();
		renderer->setTexture( SW_GC.loadTexture( "background_01.png" ) );
		
		SWVector3f vertices[] = { SWVector3f(0,0,0), SWVector3f(1920,0,0), SWVector3f(0,1280,0), SWVector3f(1920,1280,0) };
		SWVector2f texCoords[] = { SWVector2f(0,0), SWVector2f(1,0), SWVector2f(0,1), SWVector2f(1,1) };
		unsigned short indices[] = {0,1,2,3,2,1};
		SWMesh* mesh = new SWMesh();
		mesh->setVertexStream( 12, (float*)&vertices[0] );
		mesh->setTexCoordStream( 8, (float*)&texCoords[0]);
		mesh->setIndexStream( 6, &indices[0] );

		SWMeshFilter* meshFilter = go->addComponent<SWMeshFilter>();
		meshFilter->setMesh( mesh );

		SWGameObject* catGO = new SWGameObject;
		catGO->addComponent<Cat>();
		catGO->setName( "cat" );

		accum = 0;
	}

	void onUpdate( float elapsed )
	{
		accum -= SW_GC.deltaTime();
		if ( accum <= 0 )
		{
			accum = 0.1f;

			SWGameObject* ballGO = new SWGameObject;
			Ball* ball = ballGO->addComponent<Ball>();
			ballGO->setName( "ball" );
			ball->velocity = SWVector3f( 10 + rand()%100, 10 + rand()%100, 0 );
		}
	}
	void onHandleTouch( int type, int x, int y )
	{
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	SW_GC.onStart( new TestScene, "../resource/", 1920, 1280 );
	return 0;
}