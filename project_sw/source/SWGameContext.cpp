
#include "SWGameContext.h"
#include <memory>
#include <Windows.h>
#include <gl/glut.h>
#include "SWGameScene.h"
#include "SWVector2f.h"
#include "SWCriticalSection.h"
#include "SWMatrix4x4.h"
#include "stb_image.h"

void callbackDisplay()
{
}

void callbackIdle()
{
	SW_GC.onFrameMove();
}

void callbackMouse( int button, int state, int x, int y )
{
	SWGameScene* scene = SW_GC.getScene();
	
	if ( scene ) scene->handleEvent( state, x, y );
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
	SWMatrix4x4 viewMatrix;
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
	// 윈도우 크기 및 생성.
	glutInitWindowSize( (int)width, (int)height);
	glutCreateWindow("TSP");

	// 버퍼 클리어 색상 지정.
	glClearColor(0,0,0,1);

	// 버텍스 버퍼 사용
	glEnableClientState( GL_VERTEX_ARRAY );
	
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER, 0);
	

	// 뷰포트 지정.
	glViewport(0,0,width,height);

	// 프로젝션 매트릭스를 직교 행렬로 지정.
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, width, height, 0,1000,-1000);

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
	::free( memory );
}

SWGameScene* SWGameContext::getScene()
{
	return m_pimpl()->currentScene();
}

void SWGameContext::setViewMatrix( const SWMatrix4x4& matrix )
{
	m_pimpl()->viewMatrix = matrix;
}

void SWGameContext::setModelMatrix( const SWMatrix4x4& matrix )
{
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf( (float*)&(matrix * m_pimpl()->viewMatrix) );
}

void SWGameContext::setProjectionMatrix( const SWMatrix4x4& matrix )
{
	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf( &matrix.m[0][0] );
}

void SWGameContext::setVertexBuffer( const float* buffer )
{
	glVertexPointer( 3, GL_FLOAT, 0, buffer );
}

void SWGameContext::setTexCoordBuffer( const float* buffer )
{
	glTexCoordPointer( 2, GL_FLOAT, 0, buffer );
}

void SWGameContext::indexedDraw( size_t count, unsigned short* indeces)
{
	glColor3f( 1, 1, 1 );
	glDrawElements( GL_TRIANGLES, count, GL_UNSIGNED_SHORT, indeces );
}

unsigned int glLoadTexture( const char* fileName );

unsigned int SWGameContext::loadTexture( const std::string& path )
{
	return glLoadTexture( path.c_str() );
}

void SWGameContext::bindTexture( unsigned int texID )
{
	glBindTexture( GL_TEXTURE_2D, texID );
}

unsigned int glLoadTexture( const char* fileName )
{
	if ( !fileName ) return 0;

	int x,y,comp;

	unsigned char* data = stbi_load( fileName, &x, &y, &comp, 0 );

	if ( !data ) return 0;

	unsigned int texID[1];

	glGenTextures(1,&texID[0]);
	glBindTexture(GL_TEXTURE_2D,texID[0]);
	gluBuild2DMipmaps( GL_TEXTURE_2D
		             , (comp==4)? GL_RGBA8 : (comp==3)? GL_RGB8 : GL_INVALID_ENUM
					 , x, y
					 , (comp==4)? GL_RGBA : (comp==3)? GL_RGB : GLU_INVALID_VALUE
					 , GL_UNSIGNED_BYTE, data );
	/*
	glTexImage2D(GL_TEXTURE_2D, 0
	, (comp==4)? GL_RGBA8 : (comp==3)? GL_RGB8 : GL_INVALID_ENUM
	, x, y, 0
	, (comp==4)? GL_RGBA : (comp==3)? GL_RGB : GLU_INVALID_VALUE
	, GL_UNSIGNED_BYTE, data);
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

	stbi_image_free(data);

	GLenum err = glGetError();
	if ( err ) return 0;

	return texID[0];
}
