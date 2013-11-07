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
	float accum;
	void onAwake()
	{
		SWGameObject* go = new SWGameObject;
		SWMeshRenderer* renderer = go->addComponent<SWMeshRenderer>();
		renderer->setTexture( SW_GC.loadTexture( "background_01.png" ) );
		
		SWVector3f vertices[] = 
		{ SWVector3f()
		, SWVector3f()
		, SWVector3f()
		, SWVector3f()};
		SWMesh* mesh = new SWMesh();
		//mesh->setVertexStream(4, 

		SWMeshFilter* meshFilter = go->getComponent<SWMeshFilter>();
		meshFilter->setMesh(  );
	}

	void onUpdate( float elapsed )
	{
	}
	void onHandleTouch( int type, int x, int y )
	{
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	SW_GC.onStart( new TestScene, "C:/Users/hahasasa/Desktop/project_sw/resource/", 320, 480 );
	return 0;
}