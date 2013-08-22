// project_sw.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "SWGameContext.h"
#include "SWMatrix4x4.h"
#include "SWQuaternion.h"
#include "SWGameScene.h"
#include <set>
#include <map>
using namespace std;

class TestScene : public SWGameScene
{
	SW_RTTI( TestScene, SWGameScene );

	void onDraw()
	{
		SWMatrix4x4 mat;
		mat.transform( SWVector3f::one*100, SWQuaternion().rotate(SWVector3f::axisZ, 3.14/8), SWVector3f::zero );
		SW_GC.setModelViewMatrix( mat );
		SW_GC.drawFillRect( 0, 0, 100, 100 );
	}

};

int _tmain(int argc, _TCHAR* argv[])
{
	SW_GC.onStart( new TestScene, "", 320, 480 );
	return 0;
}