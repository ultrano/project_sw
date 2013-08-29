// project_sw.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "SWGameContext.h"
#include "SWMatrix4x4.h"
#include "SWQuaternion.h"
#include "SWGameScene.h"
#include "SWGameObject.h"
#include "SWTransform.h"
#include "SWMesh.h"
#include <set>
#include <map>
using namespace std;

class TestScene : public SWGameScene
{
	SW_RTTI( TestScene, SWGameScene );
	SWGameObject* obj1;

	void onAwake()
	{
		obj1 = new SWGameObject;
		SWMesh* mesh = obj1->addComponent<SWMesh>();

		static SWVector3f s_verties[] = { SWVector3f( -0.5f, -0.5f, 0 )
			                            , SWVector3f(  0.5f, -0.5f, 0 )
			                            , SWVector3f(  0.5f,  0.5f, 0 )
			                            , SWVector3f( -0.5f,  0.5f, 0 ) };

		static unsigned short s_indices[] = { 0, 1, 2, 2, 3, 0 };

		mesh->setVertexStream( 12, (float*)&s_verties[0] );
		mesh->setIndexStream( 6, &s_indices[0] );
		
		SWTransform* trans = obj1->getComponent<SWTransform>();
		trans->setLocalScale( SWVector3f::one * 100 );
	}

	void onDraw()
	{
	}

	void onHandleTouch( int type, int x, int y )
	{
		SWTransform* trans = obj1->getComponent<SWTransform>();
		trans->setLocalPosition( SWVector3f( x, y, 0 ) );
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	SW_GC.onStart( new TestScene, "", 320, 480 );
	return 0;
}