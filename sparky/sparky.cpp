// sparky.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "glew.h"
#include "glut.h"
#include "SWLog.h"

void callbackDisplay()
{
	glutSwapBuffers();
}

void callbackIdle()
{
}

void callbackTimer( int value )
{
	glutPostRedisplay();
	glutTimerFunc( (1.0f/60.0f)*1000 ,callbackTimer,0);
}

void callbackMouse( int button, int state, int x, int y )
{
	SWLog( "dddd" );
}

void callbackMouseMove( int x, int y )
{
}

void callbackReshape( int width, int height )
{
}

void callBackMenu( int entryID )
{
	SWLog( "22222" );
}

int _tmain(int argc, _TCHAR* argv[])
{
	float width = 960;
	float height = 640;
	// ���÷��� ���۸� RGB����� ������۷� ���.
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );

	// ������ ũ�� �� ����.
	glutInitWindowSize( (int)width, (int)height);
	glutCreateWindow("Sparky 1.0");

	glutMouseFunc(callbackMouse);
	glutMotionFunc( callbackMouseMove );
	glutDisplayFunc(callbackDisplay);
	glutIdleFunc(callbackIdle);
	glutReshapeFunc( callbackReshape );
	glutTimerFunc( 1 ,callbackTimer,0);
	glewInit();

	glutCreateMenu( callBackMenu );
	glutAddMenuEntry("test",1);
	glutAttachMenu( GLUT_RIGHT_BUTTON );

	glutMainLoop();
	return 0;
}

