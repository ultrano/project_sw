
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
#include "SWShader.h"
#include "SWMaterial.h"

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

	SWHardRef<SWMaterial> material;
	
	ttable< tstring,unsigned int > textureCache;
	ttable< unsigned int,TextureInfo > textureTable;
	ttable< tstring, SWHardRef<SWObject> > storage;

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

	tarray<const char*> sources;
	switch ( type )
	{
	case GL_VERTEX_SHADER :
		sources.push_back( "#define VERTEX_SHADER 1" );
		break;
	case GL_FRAGMENT_SHADER :
		sources.push_back( "#define FRAGMENT_SHADER 1" );
		break;
	}
	sources.push_back( shaderSource );

	glShaderSource( shaderID, sources.size(), &sources[0], NULL );
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
			glGetShaderInfoLog( shaderID, infoLen, &infoLen, &msg[0] );

			SWLog( msg.c_str() );
		}
		glDeleteShader( shaderID );
		return 0;
	}
	
	return shaderID;
}

GLuint loadProgram( const char* source )
{
	GLuint vsID = 0;
	GLuint fsID = 0;
	GLuint programID = 0;

	if ( (vsID = loadShader( GL_VERTEX_SHADER, source )) == 0 )
	{
		SWLog( "failed compile vertex shader" );
		return 0;
	}
	if ( (fsID = loadShader( GL_FRAGMENT_SHADER, source )) == 0 )
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

#define SHADER_SOURCE( code ) (#code)

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
		SWHardRef<SWFileInputStream> fis = new SWFileInputStream( assetPath( "system/default.shader" ) );
		tuint bufSize = fis()->available();
		tstring source;
		source.resize( bufSize );
		fis()->read( (tbyte*)&source[0], bufSize );

		SWHardRef<SWShader> shader = compileShader( source );
		pimpl->material = new SWMaterial( shader() );

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

	//////////////////////////////////////////////////////////////////////////
	{
		SWList::Value copy = SWInput.m_listeners;
		SWList::iterator itor = copy.begin();
		for ( ; itor != copy.end() ; ++itor )
		{
			SWDelegate* del = swrtti_cast<SWDelegate>( (*itor)() );
			if ( del ) del->call();
			else SWInput.m_listeners.remove( del );
		}
	}
}

void SWGameContext::onKeyChange( tuint key, bool press )
{
	if ( key >= SWInput.eKeyCount ) return;
	SWInput.m_keyFlags[ key ] = press;
	SWInput.m_lastKey = key;
	SWInput.m_keyDownCount += press? +1:-1;

	//////////////////////////////////////////////////////////////////////////
	{
		SWList::Value copy = SWInput.m_listeners;
		SWList::iterator itor = copy.begin();
		for ( ; itor != copy.end() ; ++itor )
		{
			SWDelegate* del = swrtti_cast<SWDelegate>( (*itor)() );
			if ( del ) del->call();
			else SWInput.m_listeners.remove( del );
		}
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

void SWGameContext::setVertexBuffer( const float* buffer )
{
	glVertexAttribPointer
		( SW_Attribute_Position
		, 3
		, GL_FLOAT
		, GL_FALSE
		, 0
		, buffer );
}

void SWGameContext::setTexCoordBuffer( const float* buffer )
{
	glVertexAttribPointer
		( SW_Attribute_Texture
		, 2
		, GL_FLOAT
		, GL_FALSE
		, 0
		, buffer );
}

void SWGameContext::drawIndexed( size_t count, unsigned short* indeces)
{
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
	SWHardRef<SWBuffer> buf = new SWBuffer( fis()->available() );
	fis()->read( (tbyte*)buf()->getPtr(), buf()->size() );

	TextureInfo info;
	info.id = glLoadTextureFromMemory( (const tbyte*)buf()->getPtr(), buf()->size(), info.width, info.height );
	buf()->clear();
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

const tstring& SWGameContext::assetFolder() const
{
	return m_pimpl()->resFolder;
}

const tstring  SWGameContext::assetPath( const tstring& assetFile) const
{
	return assetFolder() + assetFile;
}

SWHardRef<SWInputStream> SWGameContext::assetInputStream( const tstring& assetFile )
{
	SWFileInputStream* fis = new SWFileInputStream( assetPath( assetFile ) );
	return fis;
}

bool SWGameContext::storeItem( const tstring& key, SWObject* item )
{
	SWHardRef< SWObject > hold( item );
	ttable< tstring, SWHardRef<SWObject> >::iterator itor = m_pimpl()->storage.find( key );
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
	ttable< tstring, SWHardRef<SWObject> >::iterator itor = m_pimpl()->storage.find( key );
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

SWHardRef<SWShader> SWGameContext::compileShader( const tstring& source )
{
	tuint shaderID = loadProgram( source.c_str() );
	int bufSize = 0;
	int count = 0;
	tstring name;

	SWHardRef<SWShader> shader = new SWShader();
	shader()->m_shaderID = shaderID;

	//! check uniform
	{
		glGetProgramiv( shaderID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &bufSize );
		glGetProgramiv( shaderID, GL_ACTIVE_UNIFORMS, &count );
		name.resize( bufSize );

		for ( tuint i = 0 ; i < count ; ++i )
		{
			GLint sz = 0;
			GLenum type = GL_NONE;
			GLint len = 0;
			glGetActiveUniform( shaderID, i, bufSize, &len, &sz, &type, &name[0] );
			int index = glGetUniformLocation( shaderID, name.c_str() );
			shader()->m_uniformTable.insert( std::make_pair( name.substr(0,len), index ) );
		}
	}

	//! check attribute
	{
		glGetProgramiv( shaderID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &bufSize );
		glGetProgramiv( shaderID, GL_ACTIVE_ATTRIBUTES, &count );
		name.resize( bufSize );
		
		tarray<tstring> attribs;
		for ( tuint i = 0 ; i < count ; ++i )
		{
			GLint sz = 0;
			GLenum type = GL_NONE;
			GLint len = 0;
			glGetActiveAttrib( shaderID, i, bufSize, &len, &sz, &type, &name[0] );
			attribs.push_back( name.substr(0,len) );
		}
		
		for ( tuint i = 0 ; i < count ; ++i )
		{
			int index = -1;
			const tstring attribName = attribs[i];
			if ( attribName == "a_pos" ) index = SW_Attribute_Position;
			else if ( attribName == "a_tex" ) index = SW_Attribute_Texture;
			else continue;

			if ( index < 0 ) continue;
			glBindAttribLocation( shaderID, index, attribName.c_str() );
			shader()->m_attributes.push_back( index );
		}
		glLinkProgram( shaderID );
	}

	return shader();
}

void SWGameContext::releaseShader( SWShader* shader )
{
	if ( shader == NULL ) return;
	glDeleteProgram( shader->m_shaderID );
}

void SWGameContext::useShader( SWShader* shader )
{
	if ( shader == NULL ) return;

	glUseProgram( shader->m_shaderID );

	glDisableVertexAttribArray( SW_Attribute_Position );
	glDisableVertexAttribArray( SW_Attribute_Texture );

	for ( tuint i = 0 ; i < shader->m_attributes.size() ; ++i )
	{
		glEnableVertexAttribArray( shader->m_attributes[i] );
	}
}

void SWGameContext::setShaderMatrix4x4( int location, const float* val )
{
	glUniformMatrix4fv( location, 1, GL_FALSE, val );
}

void SWGameContext::setShaderFloat( int location, float val )
{
	glUniform1f( location, val );
}

void SWGameContext::setShaderVector2( int location, float x, float y )
{
	glUniform2f(location, x, y );
}

void SWGameContext::setShaderVector3( int location, float x, float y, float z )
{
	glUniform3f(location, x, y, z );
}

void SWGameContext::setShaderVector4( int location, float x, float y, float z, float w )
{
	glUniform4f(location, x, y, z, w );
}

void SWGameContext::setShaderTexture( int location, const tuint val )
{
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, val );
	glUniform1i( location, 0 );
}

SWMaterial* SWGameContext::getDefaultMaterial()
{
	return m_pimpl()->material();
}
