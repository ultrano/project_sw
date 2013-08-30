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
#include <set>
#include <map>
using namespace std;

class TestScene : public SWGameScene
{
	SW_RTTI( TestScene, SWGameScene );
	SWHardRef<SWGameObject> target;

	void onAwake()
	{

		SWVector3f s_verties[] = { SWVector3f( -0.5f, -0.5f, 0 )
			                     , SWVector3f(  0.5f, -0.5f, 0 )
			                     , SWVector3f(  0.5f,  0.5f, 0 )
			                     , SWVector3f( -0.5f,  0.5f, 0 ) };
		SWVector2f s_coords[]  = { SWVector2f( 0.0f, 1.0f )
			                     , SWVector2f( 1.0f, 1.0f )
			                     , SWVector2f( 1.0f, 0.0f )
			                     , SWVector2f( 0.0f, 0.0f ) };

		unsigned short s_indices[] = { 0, 1, 2, 2, 3, 0 };

		SWMesh* meshData = new SWMesh;
		meshData->setVertexStream( 12, (float*)&s_verties[0] );
		meshData->setTexCoordStream( 8, (float*)&s_coords[0] );
		meshData->setIndexStream( 6, &s_indices[0] );

		SWGameObject* obj1 = new SWGameObject;
		obj1->addComponent<SWMeshFilter>()->setData(meshData);
		obj1->getComponent<SWMeshRenderer>()->setTexture( SW_GC.loadTexture("./unit1.png") );
		SWTransform* trans1 = obj1->getComponent<SWTransform>();
		trans1->setLocalScale( SWVector3f::one * 50 );

		SWGameObject* obj2 = new SWGameObject;
		obj2->addComponent<SWMeshFilter>()->setData(meshData);
		SWTransform* trans2 = obj2->getComponent<SWTransform>();
		trans2->setLocalPosition( SWVector3f::one * 3 );
		trans2->setParent( trans1 );
		
		target = obj1;
	}

	void onHandleTouch( int type, int x, int y )
	{
		if ( !target() ) return;
		SWTransform* trans = target()->getComponent<SWTransform>();
		trans->setLocalPosition( SWVector3f( x, y, 0 ) );
		SW_OutputLog("s", target()->toString().c_str() );
		target()->destroy();
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	SW_GC.onStart( new TestScene, "", 320, 480 );
	return 0;
}