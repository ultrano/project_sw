// project_sw.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "glut.h"
#include "SWTime.h"
#include "SWInput.h"

#include "CatAss.h"

void callbackDisplay()
{
	SW_GC.onRender();
}

void callbackIdle()
{
	SW_GC.onFrameMove();
}

void callbackTimer( int value )
{
	SW_GC.onRender();
	glutSwapBuffers();
	glutTimerFunc( (1.0f/60.0f)*1000 ,callbackTimer,0);
}

void callbackMouse( int button, int state, int x, int y )
{
	switch ( state )
	{
	case GLUT_DOWN: state = SW_TouchPress;   break;
	case GLUT_UP:   state = SW_TouchRelease; break;
	}
	SWInput.onHandleEvent( state, x, y );
}

void callbackMouseMove( int x, int y )
{
	SWInput.onHandleEvent( SW_TouchMove, x, y );
}

void callbackReshape( int width, int height )
{
	SW_GC.onResize( width, height );
}

int _tmain(int argc, _TCHAR* argv[])
{
	float width = 960;
	float height = 640;
	// ���÷��� ���۸� RGB����� ������۷� ���.
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );

	// ������ ũ�� �� ����.
	glutInitWindowSize( (int)width, (int)height);
	glutCreateWindow("TSP");

	glutMouseFunc(callbackMouse);
	glutMotionFunc( callbackMouseMove );
	glutDisplayFunc(callbackDisplay);
	glutIdleFunc(callbackIdle);
	glutReshapeFunc( callbackReshape );
	glutTimerFunc( (1.0f/60.0f)*1000 ,callbackTimer,0);

	SW_GC.onStart( new InitScene, "../resource/", width, height );
	glutMainLoop();
	return 0;
}