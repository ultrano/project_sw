// project_sw.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "glew.h"
#include "glut.h"
#include "SWTime.h"
#include "SWInput.h"
#include "SWType.h"
#include "SWMath.h"

//#include "CatAss.h"
#include "TestScene.h"

void callbackDisplay()
{
	SW_GC.onRender();
	glutSwapBuffers();
}

void callbackIdle()
{
	SW_GC.onFrameMove();
}

void callbackTimer( int value )
{
	glutPostRedisplay();
	glutTimerFunc( (1.0f/60.0f)*1000 ,callbackTimer,0);
}

void callbackMouse( int button, int state, int x, int y )
{
	switch ( state )
	{
	case GLUT_DOWN: state = SW_TouchPress;   break;
	case GLUT_UP:   state = SW_TouchRelease; break;
	}
	SW_GC.onTouch( state, x, y );
}

void callbackMouseMove( int x, int y )
{
	SW_GC.onTouch( SW_TouchMove, x, y );
}

void callbackReshape( int width, int height )
{
	SW_GC.onResize( width, height );
}

void callbackKeyboard( unsigned char key, int x, int y )
{
	SW_GC.onKeyChange( key, true );
}

void callbackKeyboardUp( unsigned char key, int x, int y )
{
	SW_GC.onKeyChange( key, false );
}

int _tmain(int argc, _TCHAR* argv[])
{
	float width = 960;
	float height = 640;
	// 디스플레이 버퍼를 RGB색상과 더블버퍼로 사용.
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );

	// 윈도우 크기 및 생성.
	glutInitWindowSize( (int)width, (int)height);
	glutCreateWindow("TSP");

	float nearPlane = 1;
	float farPlane = 1000;
	float w = 2*SWMath.tan( SWMath.pi/4 )*nearPlane;
	float h = 2*SWMath.tan( SWMath.pi/4 )*nearPlane;

	glutMouseFunc(callbackMouse);
	glutMotionFunc( callbackMouseMove );
	glutDisplayFunc(callbackDisplay);
	glutIdleFunc(callbackIdle);
	glutReshapeFunc( callbackReshape );
	glutTimerFunc( 1 ,callbackTimer,0);
	glutKeyboardFunc( callbackKeyboard );
	glutKeyboardUpFunc( callbackKeyboardUp );
	glewInit();

	
	SW_GC.onStart( new TestScene, new SWWIN32AssetsAccessor( "../resource/" ), width, height );

	glutMainLoop();
	return 0;
}