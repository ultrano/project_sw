
#include "SWGameContext.h"

#ifdef WIN32
#include "glew.h"
#else
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

#include <memory>
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
#include "SWFileStream.h"
#include "SWBuffer.h"
#include "SWMath.h"

#include "stb_image.h"

#include "SWTable.h"
#include "SWString.h"
#include "SWNumber.h"
#include "SWArray.h"
#include "SWValue.h"
#include "json.h"

#include <stdio.h>
#include <fstream>

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
	
	tstring resFolder;

	int screenWidth;
	int screenHeight;
	
	SWCriticalSection idleSection;

	SWMatrix4x4 modelMatrix;
	SWMatrix4x4 viewMatrix;
	SWMatrix4x4 projMatrix;
	SWMatrix4x4 textureMatrix;

	GLuint programID;
	GLuint aPosLoc;
	GLuint aTexLoc;
	GLuint sTexLoc;
	GLuint uMVPMatLoc;
	GLuint uTexMatLoc;
	
	ttable<tstring,unsigned int> textureCache;
	ttable<unsigned int,TextureInfo> textureTable;
	ttable<tstring, SWHardRef<SWObject>> storage;

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
			tstring msg;
			msg.resize( infoLen );
			glGetProgramInfoLog( shaderID, infoLen, &infoLen, &msg[0] );

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

	if ( (vsID = loadShader( GL_VERTEX_SHADER, vertSource )) == 0 )
	{
		SWLog( "failed compile vertex shader" );
		return 0;
	}
	if ( (fsID = loadShader( GL_FRAGMENT_SHADER, fragSource )) == 0 )
	{
		glDeleteShader( vsID );
		SWLog( "failed compile fragment shader" );
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
			tstring msg;
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

void SWGameContext::onStart( SWGameScene* firstScene, const tstring& resFolder, int width, int height )
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
			"uniform   mat4 u_texMat;"           "\n"
			"attribute vec4 a_pos;"              "\n"
			"attribute vec2 a_tex;"              "\n"
			"varying   vec2 v_tex;"              "\n"
			"void main()"                        "\n"
			"{"                                  "\n"
			"   gl_Position = u_mvpMat * a_pos;" "\n"
			"   vec4 aTex = vec4(a_tex.xy,0,1);" "\n"
			"   v_tex = ( u_texMat * aTex ).xy;" "\n"
			"}";
		const char fragSrc[] =
#ifndef WIN32
			"precision mediump float;"                         "\n"
#endif
			"uniform sampler2D s_texture;"                     "\n"
			"varying vec2 v_tex;"                              "\n"
			"void main()"                                      "\n"
			"{"                                                "\n"
			"   gl_FragColor = texture2D( s_texture, v_tex );" "\n"
			"}";

		pimpl->programID = loadProgram( &vertSrc[0], &fragSrc[0] );
		pimpl->aPosLoc = glGetAttribLocation( pimpl->programID, "a_pos" );
		pimpl->aTexLoc = glGetAttribLocation( pimpl->programID, "a_tex" );
		pimpl->sTexLoc = glGetUniformLocation( pimpl->programID, "s_texture" );
		pimpl->uMVPMatLoc = glGetUniformLocation( pimpl->programID, "u_mvpMat" );
		pimpl->uTexMatLoc = glGetUniformLocation( pimpl->programID, "u_texMat" );
		
		glUseProgram( pimpl->programID );
		glEnableVertexAttribArray( pimpl->aPosLoc );
		glEnableVertexAttribArray( pimpl->aTexLoc );

		// 버퍼 클리어 색상 지정.
		glClearColor(0,0,1,1);
		
		// 버텍스 버퍼 사용
		glEnable(GL_TEXTURE_2D);

		glEnable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glAlphaFunc(GL_GREATER, 0);


		//! »Ò∏¡ øÏº±º¯¿ß

		// ∫‰∆˜∆Æ ¡ˆ¡§.
		glViewport(0,0,width,height);

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

void SWGameContext::onTouch( int type, int param1, int param2 )
{
	SWInput.m_touchState = type;
	if ( type == SW_TouchPress )
	{
		SWInput.m_deltaY = param1;
		SWInput.m_deltaY = param2;
	}
	else if ( type == SW_TouchMove )
	{
		SWInput.m_deltaX = param1 - SWInput.m_touchX;
		SWInput.m_deltaY = param2 - SWInput.m_touchY;
	}
	SWInput.m_touchX = param1;
	SWInput.m_touchY = param2;

	SWList::Value copy = SWInput.m_listeners;
	SWList::iterator itor = copy.begin();
	for ( ; itor != copy.end() ; ++itor )
	{
		SWDelegate* del = swrtti_cast<SWDelegate>( (*itor)() );
		if ( del ) del->call();
		else SWInput.m_listeners.remove( del );
	}
}

void SWGameContext::onKeyChange( tuint key, bool press )
{
	if ( key >= SWInput.eKeyCount ) return;
	SWInput.m_keyFlags[ key ] = press;

	SWList::Value copy = SWInput.m_listeners;
	SWList::iterator itor = copy.begin();
	for ( ; itor != copy.end() ; ++itor )
	{
		SWDelegate* del = swrtti_cast<SWDelegate>( (*itor)() );
		if ( del ) del->call();
		else SWInput.m_listeners.remove( del );
	}
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

int SWGameContext::getScreenWidth()
{
	return m_pimpl()->screenWidth;
}

int SWGameContext::getScreenHeight()
{
	return m_pimpl()->screenHeight;
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

void SWGameContext::setTextureMatrix( const SWMatrix4x4& matrix )
{
	m_pimpl()->textureMatrix = matrix;
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

void SWGameContext::drawIndexed( size_t count, unsigned short* indeces)
{
	SWMatrix4x4 mvpMat;
	mvpMat = m_pimpl()->modelMatrix;
	mvpMat = mvpMat * m_pimpl()->viewMatrix;
	mvpMat = mvpMat * m_pimpl()->projMatrix;
	glUniformMatrix4fv( m_pimpl()->uMVPMatLoc, 1, GL_FALSE, (float*)&mvpMat );
	glUniformMatrix4fv( m_pimpl()->uTexMatLoc, 1, GL_FALSE, (float*)&m_pimpl()->textureMatrix );
	glColor4f( 1, 1, 1, 1 );
	glDrawElements( GL_TRIANGLES, count, GL_UNSIGNED_SHORT, indeces );
}

void SWGameContext::drawRect( float left, float top, float right, float bottom )
{
	SWVector3f vertex[] = 
	{ SWVector3f( left, top, 0 )
	, SWVector3f( left, bottom, 0 )
	, SWVector3f( right, bottom, 0 )
	, SWVector3f( right, top, 0) };
	setVertexBuffer( (float*)&vertex[0] );
	bindTexture(0);
	glDrawArrays( GL_LINE_LOOP, 0, 4 );
}

unsigned int glLoadTexture( const char* fileName, int& width, int& height );
unsigned int glLoadTextureFromMemory( const unsigned char* buf, int len, int& width, int& height );

unsigned int SWGameContext::loadTexture( const tstring& path )
{
	tstring solvedPath = m_pimpl()->resFolder + path;
	ttable<tstring,unsigned int>::iterator itor = m_pimpl()->textureCache.find( solvedPath );

	if ( m_pimpl()->textureCache.end() != itor ) return itor->second;

	SWHardRef<SWFileInputStream> fis = new SWFileInputStream( solvedPath );
	SWHardRef<SWBuffer> buf = new SWBuffer( fis()->size() );
	fis()->read( (tbyte*)buf()->getPtr(), buf()->size() );

	TextureInfo info;
	info.id = glLoadTextureFromMemory( (const tbyte*)buf()->getPtr(), buf()->size(), info.width, info.height );
	buf()->freeMem();
	if ( info.id != 0 )
	{
		m_pimpl()->textureTable.insert( std::make_pair( info.id, info ) );
		m_pimpl()->textureCache.insert( std::make_pair( solvedPath, info.id ) );
	}
	else
	{
		SWLog( "failed to load texture : %s", path.c_str() );
	}

	return info.id;
}

unsigned int SWGameContext::loadTextureFromMemory( const tbyte* buf, size_t len )
{
	TextureInfo info;
	info.id = glLoadTextureFromMemory(  buf, len, info.width, info.height );

	return info.id;
}

bool SWGameContext::getTextureSize( int texID, int& width, int& height )
{
	ttable<unsigned int,TextureInfo>::iterator itor = m_pimpl()->textureTable.find( texID );
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

const tstring& SWGameContext::assetFolder() const
{
	return m_pimpl()->resFolder;
}

const tstring  SWGameContext::assetPath( const tstring& assetFile) const
{
	return assetFolder() + assetFile;
}

bool SWGameContext::storeItem( const tstring& key, SWObject* item )
{
	SWHardRef< SWObject > hold( item );
	ttable<tstring, SWHardRef<SWObject>>::iterator itor = m_pimpl()->storage.find( key );
	if ( itor != m_pimpl()->storage.end() ) return false;
	
	m_pimpl()->storage.insert( std::make_pair(key, item) );
	return true;
}

void SWGameContext::removeItem( const tstring& key )
{
	m_pimpl()->storage.erase( key );
}

SWObject* SWGameContext::findItem( const tstring& key )
{
	ttable<tstring, SWHardRef<SWObject>>::iterator itor = m_pimpl()->storage.find( key );
	if ( itor == m_pimpl()->storage.end() ) return NULL;
	return itor->second();
}

void SWGameContext::onResize( int width, int height )
{
	// ∫‰∆˜∆Æ ¡ˆ¡§.
	glViewport(0,0,width,height);
}

unsigned int glLoadTextureFromMemory( const unsigned char* buf, int len, int& width, int& height )
{
	if ( !buf || !len ) return 0;

	int comp;

	unsigned char* data = stbi_load_from_memory( buf, len, &width, &height, &comp, 0 );

	if ( !data ) return 0;

	unsigned int texID[1];

	glGenTextures(1,&texID[0]);
	glBindTexture(GL_TEXTURE_2D,texID[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexImage2D(GL_TEXTURE_2D, 0
	, (comp==4)? GL_RGBA : (comp==3)? GL_RGB : GL_INVALID_ENUM
	, width, height, 0
	, (comp==4)? GL_RGBA : (comp==3)? GL_RGB : GL_INVALID_ENUM
                 , GL_UNSIGNED_BYTE, data);
    
	GLenum err = glGetError();

	stbi_image_free(data);
	if ( err ) return 0;

	return texID[0];
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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexImage2D(GL_TEXTURE_2D, 0
	, (comp==4)? GL_RGBA : (comp==3)? GL_RGB : GL_INVALID_ENUM
	, width, height, 0
	, (comp==4)? GL_RGBA : (comp==3)? GL_RGB : GL_INVALID_ENUM
                 , GL_UNSIGNED_BYTE, data);
    
	GLenum err = glGetError();

	stbi_image_free(data);
	if ( err ) return 0;

	return texID[0];
}

SWHardRef<SWObject> convertJsonValue( const Json::Value& value )
{
	switch ( value.type() )
	{
	case Json::ValueType::nullValue : return NULL;
	case Json::ValueType::booleanValue : return new SWBoolean( value.asBool() );
	case Json::ValueType::realValue : return new SWNumber( value.asDouble() );
	case Json::ValueType::intValue  : return new SWNumber( value.asInt() );
	case Json::ValueType::uintValue : return new SWNumber( value.asUInt() );
	case Json::ValueType::stringValue : return new SWString( value.asString() );
	case Json::ValueType::arrayValue :
		{
			SWArray* arr = new SWArray;
			int count = value.size();
			for ( Json::Value::UInt i = 0 ; i < count ; ++i )
			{
				SWHardRef<SWObject> object = convertJsonValue( value.get(i,Json::Value::null) );
				arr->add( object() );
			}
			return arr;
		}
	case Json::ValueType::objectValue :
		{
			SWTable* tbl = new SWTable;
			Json::Value::Members members = value.getMemberNames();
			for ( int i = 0 ; i < members.size() ; ++i )
			{
				const tstring&  key    = members[i];
				SWHardRef<SWObject> object = convertJsonValue( value.get( key, Json::Value::null ) );
				tbl->insert( key, object() );
			}
			return tbl;
		}
	}
}

SWHardRef<SWObject> SWGameContext::loadJson( const tstring& path )
{
	tstring solvedPath = m_pimpl()->resFolder + path;
	std::ifstream ifs( solvedPath.c_str() );

	Json::Reader reader;
	Json::Value root;
	reader.parse( (std::istream&)ifs, root );

	return convertJsonValue( root );
}

SWHardRef<SWObject> SWGameContext::loadJsonFromString( const tstring& doc )
{
	Json::Reader reader;
	Json::Value root;
	reader.parse( doc, root );

	return convertJsonValue( root );
}