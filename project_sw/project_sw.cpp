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
	SWVector2f force;
	void onAwake()
	{

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
			ball->velocity = SWVector3f( force.x + rand()%100, force.y + rand()%100, 0 );
		}
	}
	void onHandleTouch( int type, int x, int y )
	{
		force.x = x;
		force.y = y;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	SW_GC.onStart( new TestScene, "../resource/", 1920, 1280 );
	return 0;
}