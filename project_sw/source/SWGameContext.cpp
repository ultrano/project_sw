
#include "SWGameContext.h"
#include <memory>
#include <map>
#include <Windows.h>
#include "glut.h"
#include "SWGameScene.h"
#include "SWVector2f.h"
#include "SWCriticalSection.h"
#include "SWMatrix4x4.h"
#include "SWDefines.h"
#include "SWLog.h"
#include "SWProfiler.h"

#include "stb_image.h"

void callbackDisplay()
{
	SW_GC.onRender();
}

void callbackIdle()
{
	SW_GC.onFrameMove();
}

void callbackMouse( int button, int state, int x, int y )
{
	switch ( state )
	{
	case GLUT_DOWN: state = SW_TouchPress;   break;
	case GLUT_UP:   state = SW_TouchRelease; break;
	}
	SW_GC.onHandleEvent( state, x, y );
}

void callbackMouseMove( int x, int y )
{
	SW_GC.onHandleEvent( SW_TouchMove, x, y );
}

void callbackReshape( int width, int height )
{
	SW_GC.onResize( width, height );
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
	int   lastDrawTime;
	float drawingTerm;
	SWCriticalSection idleSection;
	SWMatrix4x4 viewMatrix;
	bool  exitMainLoop;
	float deltaTime;
	int   startTime;
	std::map<std::string,int> textureCache;
	std::map<std::string, SWHardRef<SWObject>> storage;
	int touchState;
	int touchX;
	int touchY;
	int lastBindedTexID;
	float accumFrame;
	float accumSecond;

};

SWGameContext::SWGameContext()
	: m_pimpl( NULL )
{
}

void SWGameContext::onStart( SWGameScene* firstScene, const std::string& resFolder, float width, float height )
{
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
	glutMotionFunc( callbackMouseMove );
	glutDisplayFunc(callbackDisplay);
	glutIdleFunc(callbackIdle);
	glutReshapeFunc( callbackReshape );
	//glutKeyboardFunc(callbackKeyboard);
	//glutPassiveMotionFunc(motionCallBack);
	//glutMotionFunc(motionCallBack);
	//glutReshapeFunc(windowReshape);

	Pimpl* pimpl = new Pimpl;
	m_pimpl = pimpl;
	pimpl->nextScene = firstScene;
	pimpl->resFolder = resFolder;
	pimpl->screenWidth  = width;
	pimpl->screenHeight = height;
	pimpl->exitMainLoop = false;
	pimpl->lastFrameTime = GetTickCount();
	pimpl->lastDrawTime = pimpl->lastFrameTime;
	pimpl->drawingTerm  = 1.0f/10.0f;
	pimpl->deltaTime = 0;
	pimpl->startTime = GetTickCount();
	pimpl->lastBindedTexID = 0;
	pimpl->accumFrame = 0;
	pimpl->accumSecond = 0;
	glutMainLoop();
}

void SWGameContext::onFrameMove()
{
	{
		Pimpl* pimpl = m_pimpl();

		int nowFrameTime = GetTickCount();
		pimpl->deltaTime = ( (float)(nowFrameTime - pimpl->lastFrameTime) ) / 1000.0f;
		pimpl->lastFrameTime = nowFrameTime;

		pimpl->accumFrame += 1;
		pimpl->accumSecond += pimpl->deltaTime;

		SWLog( "Frame Count : %d", (int)pimpl->accumFrame );
		SWLog( "FPS : %.1f", ( pimpl->accumFrame / pimpl->accumSecond ) );
		if ( pimpl->accumSecond > 10 )
		{
			pimpl->accumFrame /= pimpl->accumSecond;
			pimpl->accumSecond /= pimpl->accumSecond;
		}

		if ( pimpl->nextScene.isValid() )
		{
			if ( pimpl->currentScene.isValid() ) pimpl->currentScene()->destroy();
			pimpl->currentScene = pimpl->nextScene();
			pimpl->nextScene = NULL;
			if ( pimpl->currentScene.isValid() ) pimpl->currentScene()->awake();
		}

		if ( SWGameScene* scene = pimpl->currentScene() )
		{
			scene->update( pimpl->deltaTime );
		}

		float term = ((float)(nowFrameTime - pimpl->lastDrawTime))/1000.0f;
		if ( term >= pimpl->drawingTerm )
		{
			pimpl->lastDrawTime = nowFrameTime;
			glutPostRedisplay();
		}
	}

	SWLogCenter::getInstance().present();
}

void SWGameContext::onRender()
{
	if ( SWGameScene* scene = m_pimpl()->currentScene() )
	{
		glClearColor( 0, 0, 1, 1 );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
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

void SWGameContext::setNextScene( SWGameScene* scene )
{
	m_pimpl()->nextScene = scene;
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
	std::string solvedPath = m_pimpl()->resFolder + path;
	std::map<std::string,int>::iterator itor = m_pimpl()->textureCache.find( solvedPath );
	
	if ( m_pimpl()->textureCache.end() != itor ) return itor->second;

	int texID = glLoadTexture(  solvedPath.c_str() );
	if ( texID != 0 ) m_pimpl()->textureCache.insert( std::make_pair( solvedPath, texID ) );

	return texID;
}

void SWGameContext::bindTexture( unsigned int texID )
{
	if ( m_pimpl()->lastBindedTexID == texID ) return;
	glBindTexture( GL_TEXTURE_2D, texID );
	m_pimpl()->lastBindedTexID = texID;
}

float SWGameContext::deltaTime() const
{
	return m_pimpl()->deltaTime;
}

float SWGameContext::awakeTime() const
{
	int delta = (GetTickCount() - m_pimpl()->startTime);
	return ( (float)delta )/1000.0f;
}

bool SWGameContext::storeItem( const std::string& key, SWObject* item )
{
	std::map<std::string, SWHardRef<SWObject>>::iterator itor = m_pimpl()->storage.find( key );
	if ( itor != m_pimpl()->storage.end() ) return false;
	
	m_pimpl()->storage.insert( std::make_pair(key, item) );
}

void SWGameContext::removeItem( const std::string& key )
{
	m_pimpl()->storage.erase( key );
}

SWObject* SWGameContext::findItem( const std::string& key )
{
	std::map<std::string, SWHardRef<SWObject>>::iterator itor = m_pimpl()->storage.find( key );
	if ( itor == m_pimpl()->storage.end() ) return NULL;
	return itor->second();
}

void SWGameContext::onHandleEvent( int type, int param1, int param2 )
{
	m_pimpl()->touchState = type;
	m_pimpl()->touchX = param1;
	m_pimpl()->touchY = param2;

	SWGameScene* scene = getScene();
	if ( scene ) scene->handleEvent( type, param1, param2 );
}

void SWGameContext::onResize( int width, int height )
{
	// 뷰포트 지정.
	glViewport(0,0,width,height);

	// 프로젝션 매트릭스를 직교 행렬로 지정.
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, width, height, 0,1000,-1000);
}

int SWGameContext::getTouchState() const
{
	return m_pimpl()->touchState;
}

int SWGameContext::getTouchX() const
{
	return m_pimpl()->touchX;
}

int SWGameContext::getTouchY() const
{
	return m_pimpl()->touchY;
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
