
#include "SWGameContext.h"

#ifdef WIN32
#include "glew.h"
#else
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#endif

#include <memory>
#include <map>
#include "SWGameScene.h"
#include "SWVector2f.h"
#include "SWCriticalSection.h"
#include "SWMatrix4x4.h"
#include "SWDefines.h"
#include "SWLog.h"
#include "SWProfiler.h"
#include "SWUtil.h"
#include "SWTime.h"
#include "SWInput.h"
#include "SWString.h"

#include "stb_image.h"

struct TextureInfo
{
	unsigned int id;
	int width;
	int height;
};

class SWGameContext::Pimpl : public SWRefCountable
{
public:

	SWHardRef<SWGameScene> currentScene;
	SWHardRef<SWGameScene> nextScene;
	
	std::string resFolder;

	float screenWidth;
	float screenHeight;
	
	SWCriticalSection idleSection;

	SWMatrix4x4 modelMatrix;
	SWMatrix4x4 viewMatrix;
	SWMatrix4x4 projMatrix;

	GLuint programID;
	GLuint aPosLoc;
	GLuint aTexLoc;
	GLuint uMVPLoc;
	GLuint sTexLoc;
	
	std::map<std::string,unsigned int> textureCache;
	std::map<unsigned int,TextureInfo> textureTable;
	std::map<std::string, SWHardRef<SWObject>> storage;

	int lastBindedTexID;

};

SWGameContext::SWGameContext()
	: m_pimpl( NULL )
{
}

GLuint loadShader( GLenum type, const char* shaderSource )
{
	if ( shaderSource == NULL )
	{
		SWLog( "shader source is NULL" );
		return 0;
	}

	GLuint shaderID = glCreateShader( type );
	
	if ( shaderID == 0 )
	{
		SWLog( "create shader failed" );
		return 0;
	}

	glShaderSource( shaderID, 1, &shaderSource, NULL );
	glCompileShader( shaderID );
	
	GLint compiled = false;
	glGetShaderiv( shaderID, GL_COMPILE_STATUS, &compiled );
	
	if ( !compiled )
	{
		int infoLen = 0;
		glGetShaderiv( shaderID, GL_INFO_LOG_LENGTH, &infoLen );
		if ( infoLen > 0 )
		{
			SWString::Value msg;
			msg.resize( infoLen );
			glGetProgramInfoLog( shaderID, infoLen, NULL, &msg[0] );

			SWLog( msg.c_str() );
		}
		glDeleteShader( shaderID );
		return 0;
	}
	
	return shaderID;
}

GLuint loadProgram( const char* vertSource, const char* fragSource )
{
	GLuint vsID = 0;
	GLuint fsID = 0;
	GLuint programID = 0;

	if ( (vsID = loadShader( GL_VERTEX_SHADER, vertSource )) == 0 ) return 0;
	if ( (fsID = loadShader( GL_FRAGMENT_SHADER, fragSource )) == 0 )
	{
		glDeleteShader( vsID );
		return 0;
	}

	if ( (programID = glCreateProgram()) == 0 ) return 0;

	glAttachShader( programID, vsID );
	glAttachShader( programID, fsID );
	glLinkProgram( programID );

	GLint linked = false;
	glGetProgramiv( programID, GL_LINK_STATUS, &linked );
	if ( linked == 0 )
	{
		int infoLen = 0;
		glGetProgramiv( programID, GL_INFO_LOG_LENGTH, &infoLen );
		if ( infoLen > 0 )
		{
			SWString::Value msg;
			msg.resize( infoLen );
			glGetProgramInfoLog( programID, infoLen, NULL, &msg[0] );

			SWLog( msg.c_str() );
		}
		glDeleteProgram( programID );
		programID = 0;
	}

	glDeleteShader( vsID );
	glDeleteShader( fsID );

	if ( programID == 0 ) SWLog( "failed to loading program" );
	return programID;
}

void SWGameContext::onStart( SWGameScene* firstScene, const std::string& resFolder, float width, float height )
{

	Pimpl* pimpl = new Pimpl;
	m_pimpl = pimpl;
	pimpl->nextScene = firstScene;
	pimpl->resFolder = resFolder;
	pimpl->screenWidth  = width;
	pimpl->screenHeight = height;
	pimpl->lastBindedTexID = 0;

	SWTime.m_lastFrameTime = SWTime.getTime();

	//! opengl initializing
	{
		const char vertSrc[] = 
			"uniform   mat4 u_mvpMat;"           "\n"
			"attribute vec4 a_pos;"              "\n"
			"attribute vec2 a_tex;"              "\n"
			"varying   vec2 v_tex;"              "\n"
			"void main()"                        "\n"
			"{"                                  "\n"
			"   gl_Position = u_mvpMat * a_pos;" "\n"
			"   v_tex = a_tex;"                  "\n"
			"}";
		const char fragSrc[] =
			"precision mediump float;"                         "\n"
			"uniform sampler2D s_texture;"                     "\n"
			"varying vec2 v_tex;"                              "\n"
			"void main()"                                      "\n"
			"{"                                                "\n"
			"   gl_FragColor = texture2D( s_texture, v_tex );" "\n"
			"}";

		pimpl->programID = loadProgram( &vertSrc[0], &fragSrc[0] );
		pimpl->aPosLoc = glGetAttribLocation( pimpl->programID, "a_pos" );
		pimpl->aTexLoc = glGetAttribLocation( pimpl->programID, "a_tex" );
		pimpl->uMVPLoc = glGetUniformLocation( pimpl->programID, "u_mvpMat" );
		pimpl->sTexLoc = glGetUniformLocation( pimpl->programID, "s_texture" );
		
		glUseProgram( pimpl->programID );
		glEnableVertexAttribArray( pimpl->aPosLoc );
		glEnableVertexAttribArray( pimpl->aTexLoc );

		// 버퍼 클리어 색상 지정.
		glClearColor(0,0,1,1);
		
		// 버텍스 버퍼 사용
		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glEnable(GL_TEXTURE_2D);

		glEnable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glAlphaFunc(GL_GREATER, 0);

		//! 희망 우선순위

		// 뷰포트 지정.
		glViewport(0,0,width,height);

		// 프로젝션 매트릭스를 직교 행렬로 지정.
		SWMatrix4x4 proj;
		setProjectionMatrix( proj.ortho( 0, width, 0, height,1000,-1000) );
	}

}

void SWGameContext::onFrameMove()
{
	Pimpl* pimpl = m_pimpl();

	float nowTime = SWTime.getTime();
	SWTime.m_deltaFrameTime = ( nowTime - SWTime.m_lastFrameTime );
	SWTime.m_lastFrameTime = nowTime;

	SWTime.m_accumFrame += 1;
	SWTime.m_accumTime  += SWTime.getDeltaTime();

	SWTime.m_DPS = ( SWTime.m_accumDraw / SWTime.m_accumTime );
	SWTime.m_FPS = ( SWTime.m_accumFrame / SWTime.m_accumTime );
	SWLog( "FPS : %.1f", SWTime.getFPS() );
	SWLog( "DPS : %.1f", SWTime.getDPS() );
	if ( SWTime.m_accumTime > 10 )
	{
		SWTime.m_accumDraw  /= SWTime.m_accumTime;
		SWTime.m_accumFrame /= SWTime.m_accumTime;
		SWTime.m_accumTime  /= SWTime.m_accumTime;
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
		scene->update();
	}

	SWLogCenter::getInstance().present();
}

void SWGameContext::onRender()
{
	if ( SWGameScene* scene = m_pimpl()->currentScene() )
	{
		glClear( GL_COLOR_BUFFER_BIT );
		scene->draw();
	}
	SWTime.m_accumDraw += 1;
}

SWGameContext& SWGameContext::getInstance()
{
	static SWGameContext s_instance;
	return s_instance;
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
	matrix.inverse( m_pimpl()->viewMatrix );
}

void SWGameContext::setModelMatrix( const SWMatrix4x4& matrix )
{
	m_pimpl()->modelMatrix = matrix;
}

void SWGameContext::setProjectionMatrix( const SWMatrix4x4& matrix )
{
	m_pimpl()->projMatrix = matrix;
}

void SWGameContext::setVertexBuffer( const float* buffer )
{
	glVertexAttribPointer
		( m_pimpl()->aPosLoc
		, 3
		, GL_FLOAT
		, GL_FALSE
		, 0
		, buffer );
}

void SWGameContext::setTexCoordBuffer( const float* buffer )
{
	glVertexAttribPointer
		( m_pimpl()->aTexLoc
		, 2
		, GL_FLOAT
		, GL_FALSE
		, 0
		, buffer );
}

void SWGameContext::indexedDraw( size_t count, unsigned short* indeces)
{
	SWMatrix4x4 mvpMat;
	mvpMat = m_pimpl()->modelMatrix;
	mvpMat = mvpMat * m_pimpl()->viewMatrix;
	mvpMat = mvpMat * m_pimpl()->projMatrix;
	glUniformMatrix4fv( m_pimpl()->uMVPLoc, 1, GL_FALSE, (float*)&mvpMat );
	glColor4f( 1, 1, 1, 1 );
	glDrawElements( GL_TRIANGLES, count, GL_UNSIGNED_SHORT, indeces );
}

unsigned int glLoadTexture( const char* fileName, int& width, int& height );

unsigned int SWGameContext::loadTexture( const std::string& path )
{
	std::string solvedPath = m_pimpl()->resFolder + path;
	std::map<std::string,unsigned int>::iterator itor = m_pimpl()->textureCache.find( solvedPath );
	
	if ( m_pimpl()->textureCache.end() != itor ) return itor->second;

	TextureInfo info;
	info.id = glLoadTexture(  solvedPath.c_str(), info.width, info.height );
	if ( info.id != 0 )
	{
		m_pimpl()->textureTable.insert( std::make_pair( info.id, info ) );
		m_pimpl()->textureCache.insert( std::make_pair( solvedPath, info.id ) );
	}

	return info.id;
}

bool SWGameContext::getTextureSize( int texID, int& width, int& height )
{
	std::map<unsigned int,TextureInfo>::iterator itor = m_pimpl()->textureTable.find( texID );
	if ( itor == m_pimpl()->textureTable.end() ) return false;
	width  = itor->second.width;
	height = itor->second.height;
	return true;
}

void SWGameContext::bindTexture( unsigned int texID )
{
	if ( m_pimpl()->lastBindedTexID == texID ) return;
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, texID );
	glUniform1i( m_pimpl()->sTexLoc, 0 );
	m_pimpl()->lastBindedTexID = texID;
}

bool SWGameContext::storeItem( const std::string& key, SWObject* item )
{
	SWHardRef< SWObject > hold( item );
	std::map<std::string, SWHardRef<SWObject>>::iterator itor = m_pimpl()->storage.find( key );
	if ( itor != m_pimpl()->storage.end() ) return false;
	
	m_pimpl()->storage.insert( std::make_pair(key, item) );
	return true;
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

void SWGameContext::onResize( int width, int height )
{
	// 뷰포트 지정.
	glViewport(0,0,width,height);

	// 프로젝션 매트릭스를 직교 행렬로 지정.
	SWMatrix4x4 proj;
	setProjectionMatrix( proj.ortho( 0, width, 0, height,1000,-1000) );
}

unsigned int glLoadTexture( const char* fileName, int& width, int& height )
{
	if ( !fileName ) return 0;

	int comp;

	unsigned char* data = stbi_load( fileName, &width, &height, &comp, 0 );

	if ( !data ) return 0;

	unsigned int texID[1];

	glGenTextures(1,&texID[0]);
	glBindTexture(GL_TEXTURE_2D,texID[0]);

	glTexImage2D(GL_TEXTURE_2D, 0
	, (comp>2&&comp<5)? comp : GL_INVALID_ENUM
	, width, height, 0
	, (comp==4)? GL_RGBA : (comp==3)? GL_RGB : GL_INVALID_ENUM
	, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	stbi_image_free(data);

	GLenum err = glGetError();
	if ( err ) return 0;

	return texID[0];
}
