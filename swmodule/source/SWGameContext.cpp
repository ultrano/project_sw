
#include "SWGameContext.h"

#include <memory>
#include "SWGameScene.h"
#include "TVector2f.h"
#include "SWCriticalSection.h"
#include "TMatrix4x4.h"
#include "SWDefines.h"
#include "SWLog.h"
#include "SWProfiler.h"
#include "SWUtil.h"
#include "SWTime.h"
#include "SWInput.h"
#include "SWString.h"
#include "SWFileStream.h"
#include "SWByteBuffer.h"
#include "SWMath.h"
#include "SWShader.h"
#include "SWMaterial.h"

#include "SWAssets.h"

#include "SWTable.h"
#include "SWString.h"
#include "SWNumber.h"
#include "SWArray.h"
#include "SWValue.h"
#include "json.h"

#include <stdio.h>
#include <fstream>

class SWGameContext::Pimpl : public SWRefCountable
{
public:

	SWHardRef<SWGameScene> currentScene;
	SWHardRef<SWGameScene> nextScene;
	bool nextSceneReserved;
	
	tstring resFolder;

	int screenWidth;
	int screenHeight;
	
	SWCriticalSection idleSection;

	TMatrix4x4 modelMatrix;
	TMatrix4x4 viewMatrix;
	TMatrix4x4 projMatrix;
	TMatrix4x4 textureMatrix;

	SWHardRef<SWMaterial> material;
	
	ttable< tstring,unsigned int > textureCache;
	ttable< tstring, SWObject::Ref > storage;

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
		sources.push_back( "#define VERTEX_SHADER 1\n" );
		break;
	case GL_FRAGMENT_SHADER :
		sources.push_back( "#define FRAGMENT_SHADER 1\n" );
		sources.push_back( "#ifdef GL_ES\n" );
		sources.push_back( "precision mediump float;\n" );
		sources.push_back( "#endif\n" );
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
	pimpl->nextSceneReserved = true;
	pimpl->resFolder = resFolder;
	pimpl->screenWidth  = width;
	pimpl->screenHeight = height;
	pimpl->lastBindedTexID = 0;

	SWTime.m_lastFrameTime = SWTime.getTime();

	//! opengl initializing
	{

		// 甕곌쑵�쓠 占쎄깻�뵳�딅선 占쎄퉳占쎄맒 筌욑옙占쎌젟.
		glClearColor(0,0,1,1);
		
		// 甕곌쑵�볩옙�뮞 甕곌쑵�쓠 占쎄텢占쎌뒠
		glEnable(GL_TEXTURE_2D);

		glEnable(GL_BLEND);
		//glEnable(GL_ALPHA_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glAlphaFunc(GL_GREATER, 0);

		//! 夷⑺슆占쎈땰吏� 泥⑺슃夷섏㏏夷섏㎟姨뷀슜

		// 占쎈떭占쎄껴�듊占쎌뭼�듊�쉱 吏뺧옙�뀒�돞占�.
		glViewport(0,0,width,height);

	}

	//! setting default shader and material
	{

		tstring source =
		"uniform   mat4 u_mvpMat;\n"
		"uniform sampler2D s_texture;\n"
		"varying   vec2 v_tex;\n"

		"#ifdef VERTEX_SHADER\n"

		"attribute vec4 a_pos;\n"
		"attribute vec2 a_tex;\n"

		"void main()\n"
		"{\n"
		"   gl_Position = u_mvpMat * a_pos;\n"
		"   v_tex = a_tex;\n"
		"}\n"

		"#endif\n"

		"#ifdef FRAGMENT_SHADER\n"

		"void main()\n"
		"{\n"
		"   gl_FragColor = texture2D( s_texture, v_tex );\n"
		"}\n"

		"#endif\n";

		SWHardRef<SWShader> shader = compileShader( source );
		pimpl->material = new SWMaterial( shader() );
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
	//SWLog( "FPS : %.1f", SWTime.getFPS() );
	//SWLog( "DPS : %.1f", SWTime.getDPS() );
	if ( SWTime.m_accumTime > 10 )
	{
		SWTime.m_accumDraw  /= SWTime.m_accumTime;
		SWTime.m_accumFrame /= SWTime.m_accumTime;
		SWTime.m_accumTime  /= SWTime.m_accumTime;
	}

	if ( pimpl->nextSceneReserved )
	{
		pimpl->nextSceneReserved = false;
		if ( pimpl->currentScene.isValid() ) pimpl->currentScene()->destroy();
		pimpl->currentScene = pimpl->nextScene();
		pimpl->nextScene = NULL;
		if ( pimpl->currentScene.isValid() ) pimpl->currentScene()->awake();
	}

	if ( SWGameScene* scene = pimpl->currentScene() )
	{
		scene->update();
	}

	SWInput.m_deltaX = 0;
	SWInput.m_deltaY = 0;

	SWLogCenter::getInstance().present();
}

void SWGameContext::onRender()
{
	glClear( GL_COLOR_BUFFER_BIT );
	if ( SWGameScene* scene = m_pimpl()->currentScene() )
	{
		scene->draw();
	}
	SWTime.m_accumDraw += 1;
}

void SWGameContext::onTouch( int type, int param1, int param2 )
{
	SWLog( "touch type:%d, x:%d, y:%d", type, param1, param2 );

	SWInput.m_touchState = type;
	if ( type == SW_TouchPress )
	{
		SWInput.m_deltaX = param1;
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
		SWList::Type copy = SWInput.m_listeners;
		SWList::iterator itor = copy.begin();
		for ( ; itor != copy.end() ; ++itor )
		{
			SWDelegator* del = swrtti_cast<SWDelegator>( (*itor)() );
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
		SWList::Type copy = SWInput.m_listeners;
		SWList::iterator itor = copy.begin();
		for ( ; itor != copy.end() ; ++itor )
		{
			SWDelegator* del = swrtti_cast<SWDelegator>( (*itor)() );
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
	m_pimpl()->nextSceneReserved = true;
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
	//glColor4f( 1, 1, 1, 1 );
	glDrawElements( GL_TRIANGLES, count, GL_UNSIGNED_SHORT, indeces );
}

void SWGameContext::drawRect( float left, float top, float right, float bottom )
{
	TVector3f vertex[] = 
	{ TVector3f( left, top, 0 )
	, TVector3f( left, bottom, 0 )
	, TVector3f( right, bottom, 0 )
	, TVector3f( right, top, 0) };
	setVertexBuffer( (float*)&vertex[0] );
	glDrawArrays( GL_LINE_LOOP, 0, 4 );
}

bool SWGameContext::storeItem( const tstring& key, SWObject* item )
{
	SWHardRef< SWObject > hold( item );
	ttable< tstring, SWObject::Ref >::iterator itor = m_pimpl()->storage.find( key );
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
	ttable< tstring, SWObject::Ref >::iterator itor = m_pimpl()->storage.find( key );
	if ( itor == m_pimpl()->storage.end() ) return NULL;
	return itor->second();
}

void SWGameContext::onResize( int width, int height )
{
	// 占쎈떭占쎄껴�듊占쎌뭼�듊�쉱 吏뺧옙�뀒�돞占�.
	glViewport(0,0,width,height);
}

SWHardRef<SWShader> SWGameContext::compileShader( const tstring& source )
{
	SWLog( "%d", __LINE__ );
	tuint shaderID = loadProgram( source.c_str() );
	int bufSize = 0;
	int count = 0;
	tstring name;

	SWLog( "%d", __LINE__ );
	SWHardRef<SWShader> shader = new SWShader();
	shader()->m_shaderID = shaderID;
	SWLog( "%d", __LINE__ );

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
	SWLog( "%d", __LINE__ );

	//! check attribute
	{
		glGetProgramiv( shaderID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &bufSize );
		glGetProgramiv( shaderID, GL_ACTIVE_ATTRIBUTES, &count );
		name.resize( bufSize );
		SWLog( "%d", __LINE__ );
		
		tarray<tstring> attribs;
		for ( tuint i = 0 ; i < count ; ++i )
		{
			GLint sz = 0;
			GLenum type = GL_NONE;
			GLint len = 0;
			glGetActiveAttrib( shaderID, i, bufSize, &len, &sz, &type, &name[0] );
			attribs.push_back( name.substr(0,len) );
		}
		SWLog( "%d", __LINE__ );
		
		for ( tuint i = 0 ; i < count ; ++i )
		{
			int index = -1;
			const tstring attribName = attribs[i];
			if ( attribName == "a_pos" ) index = SW_Attribute_Position;
			else if ( attribName == "a_tex" ) index = SW_Attribute_Texture;
			else continue;

			if ( index < 0 ) continue;
			SWLog( attribName.c_str() );
			glBindAttribLocation( shaderID, index, attribName.c_str() );
			SWLog( "%d", __LINE__ );
			SWLog( shader.isValid()? "safe":"null" );
			SWLog( "shader()->m_attributes : %d", shader()->m_attributes.size() );
			shader()->m_attributes.push_back( (tuint)index );
			SWLog( "%d", __LINE__ );
		}
		SWLog( "%d", __LINE__ );
		glLinkProgram( shaderID );
	}
	SWLog( "%d", __LINE__ );

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
