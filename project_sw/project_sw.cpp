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
#include "SWMeshData.h"
#include <set>
#include <map>
using namespace std;

class TestScene : public SWGameScene
{
	SW_RTTI( TestScene, SWGameScene );
	SWGameObject* obj1;

	void onAwake()
	{

		SWVector3f s_verties[] = { SWVector3f( -0.5f, -0.5f, 0 )
			, SWVector3f(  0.5f, -0.5f, 0 )
			, SWVector3f(  0.5f,  0.5f, 0 )
			, SWVector3f( -0.5f,  0.5f, 0 ) };

		unsigned short s_indices[] = { 0, 1, 2, 2, 3, 0 };

		SWMeshData* meshData = new SWMeshData;
		meshData->setVertexStream( 12, (float*)&s_verties[0] );
		meshData->setIndexStream( 6, &s_indices[0] );
		
		obj1 = new SWGameObject;
		obj1->addComponent<SWMesh>()->setData(meshData);
		SWTransform* trans1 = obj1->getComponent<SWTransform>();
		trans1->setLocalScale( SWVector3f::one * 10 );

		SWGameObject* obj2 = new SWGameObject;
		obj2->addComponent<SWMesh>()->setData(meshData);
		SWTransform* trans2 = obj2->getComponent<SWTransform>();
		trans2->setLocalPosition( SWVector3f::one * 3 );
		trans2->setParent( trans1 );
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