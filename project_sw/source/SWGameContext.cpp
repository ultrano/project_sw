
#include "SWGameContext.h"
#include <memory>
#include <Windows.h>
#include <gl/glut.h>
#include "SWGameScene.h"
#include "SWVector2f.h"
#include "SWCriticalSection.h"
#include "SWMatrix4x4.h"

void callbackDisplay()
{
}

void callbackIdle()
{
	SW_GC.onFrameMove();
}

void callbackMouse( int button, int state, int x, int y )
{


}

class SWGameContext::Pimpl : public SWRefCountable
{
public:

	SWHardRef<SWGameScene> currentScene;
	SWHardRef<SWGameScene> nextScene;
	std::string resFolder;
	float screenWidth;
	float screenHeight;
	int   lastFrameTime;
	SWCriticalSection idleSection;
};

SWGameContext::SWGameContext()
	: m_pimpl( NULL )
{
}

void SWGameContext::onStart( SWGameScene* firstScene, const std::string& resFolder, float width, float height )
{
	Pimpl* pimpl = new Pimpl;
	m_pimpl = pimpl;
	pimpl->nextScene = firstScene;
	pimpl->resFolder = resFolder;
	pimpl->screenWidth  = width;
	pimpl->screenHeight = height;

	// 디스플레이 버퍼를 RGB색상과 더블버퍼로 사용.
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutCreateWindow("TSP");

	// 윈도우 크기 및 생성.
	glutInitWindowSize(width,height);

	// 버퍼 클리어 색상 지정.
	glClearColor(0,0,0,1);

	// 버텍스 버퍼 사용
	glEnableClientState( GL_VERTEX_ARRAY );
	/*
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER, 0);
	*/

	// 뷰포트 지정.
	glViewport(0,0,width,height);

	// 프로젝션 매트릭스를 직교 행렬로 지정.
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho(-width/2,width/2,-height/2,height/2,1,-1);

	glutMouseFunc(callbackMouse);
	glutDisplayFunc(callbackDisplay);
	glutIdleFunc(callbackIdle);
	//glutKeyboardFunc(callbackKeyboard);
	//glutPassiveMotionFunc(motionCallBack);
	//glutMotionFunc(motionCallBack);
	//glutReshapeFunc(windowReshape);

	glutMainLoop();
}

void SWGameContext::onFrameMove()
{
	Pimpl* pimpl = m_pimpl();

	int newFrameTime = GetTickCount();
	int elapsedTime  = newFrameTime - pimpl->lastFrameTime;
	pimpl->lastFrameTime = newFrameTime;

	if ( pimpl->nextScene.isValid() )
	{
		if ( pimpl->currentScene.isValid() ) pimpl->currentScene()->destroy();
		pimpl->currentScene = pimpl->nextScene();
		pimpl->nextScene = NULL;
		if ( pimpl->currentScene.isValid() ) pimpl->currentScene()->awake();
	}

	if ( SWGameScene* scene = pimpl->currentScene() )
	{
		scene->update( ( (float)elapsedTime ) / 1000.0f );

		glClearColor( 0, 0, 1, 1 );
		glClear( GL_COLOR_BUFFER_BIT );
		scene->draw();
		glutSwapBuffers();
	}
}

SWGameContext& SWGameContext::getInstance()
{
	static SWGameContext s_instance;
	return s_instance;
}

void* SWGameContext::alloc( size_t size )
{
	return malloc( size );
}

void  SWGameContext::free( void* memory )
{
	free( memory );
}

SWGameScene* SWGameContext::getScene()
{
	return m_pimpl()->currentScene();
}

void SWGameContext::setModelViewMatrix( const SWMatrix4x4& matrix )
{
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf( &matrix.m[0][0] );
}

void SWGameContext::setProjectionMatrix( const SWMatrix4x4& matrix )
{
	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf( &matrix.m[0][0] );
}

/*
void SWGameContext::drawImageRegion( const int& img, float dstX, float dstY, float dstWidth, float dstHeight, float srcX, float srcY, float srcWidth, float srcHeight )
{
	static SWVector2f s_verties[] = { SWVector2f( -0.5f, -0.5f )
		, SWVector2f(  0.5f, -0.5f )
		, SWVector2f(  0.5f,  0.5f )
		, SWVector2f( -0.5f,  0.5f ) };

	static short s_indices[] = { 0, 1, 2, 2, 3, 0 };

}
*/
void SWGameContext::drawFillRect( float x, float y, float width, float height )
{
	static SWVector2f s_verties[] = { SWVector2f( -0.5f, -0.5f )
		, SWVector2f(  0.5f, -0.5f )
		, SWVector2f(  0.5f,  0.5f )
		, SWVector2f( -0.5f,  0.5f ) };
	static SWVector2f s_coords[]  = { SWVector2f( 0.0f, 1.0f )
		, SWVector2f( 1.0f, 1.0f )
		, SWVector2f( 1.0f, 0.0f )
		, SWVector2f( 0.0f, 0.0f ) };

	static short s_indices[] = { 0, 1, 2, 2, 3, 0 };

	glColor3f( 1, 1, 1 );

	glVertexPointer( 2, GL_FLOAT, 0, &s_verties[0] );

	glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, &s_indices[0] );

}

void SWGameContext::setVertexBuffer( const float* buffer )
{
	glVertexPointer( 3, GL_FLOAT, 0, buffer );
}