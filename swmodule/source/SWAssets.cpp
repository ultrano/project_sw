#include "SWAssets.h"
#include "SWLog.h"
#include "SWByteBufferStream.h"

__SWAssets::__SWAssets()
{

}

__SWAssets::~__SWAssets()
{

}

__SWAssets& __SWAssets::getInstance()
{
	static __SWAssets instance;
	return instance;
}

SWHardRef<SWInputStream> __SWAssets::loadBuffer( const tstring& filePath )
{
	if ( !m_accessor.isValid() ) return NULL;

	BufferTable::iterator itor = m_bufferCache.find( filePath );
	if ( itor != m_bufferCache.end() )
	{
		if ( itor->second.isValid() )
		{
			SWLog( "asset:\"%s\" cache load", filePath.c_str() );
			return new SWByteBufferInputStream( itor->second() );
		}
		SWLog( "asset:\"%s\" unloaded, trying reload", filePath.c_str() );
	}

	SWHardRef<SWInputStream> ais = m_accessor()->access( filePath );
	if ( ais.isValid() == false ) return NULL;
	if ( ais()->available() <= 0 ) return NULL;

	SWByteBufferInputStream* bbis = new SWByteBufferInputStream( ais() );

	m_bufferCache[ filePath ] = bbis->getBuffer();
	return bbis;
}

SWHardRef<SWTexture> __SWAssets::loadTexture( const tstring& filePath )
{
	if ( !m_accessor.isValid() ) return NULL;

	TextureTable::iterator itor = m_texCache.find( filePath );
	if ( itor != m_texCache.end() )
	{
		if ( itor->second.isValid() )
		{
			SWLog( "asset:\"%s\" cache load", filePath.c_str() );
			return itor->second();
		}
		SWLog( "asset:\"%s\" unloaded, trying reload", filePath.c_str() );
	}

	SWHardRef<SWInputStream> ais = m_accessor()->access( filePath );
	if ( ais.isValid() == false ) return NULL;
	if ( ais()->available() <= 0 ) return NULL;
	
	tarray<tbyte> buffer;
	buffer.resize( ais()->available() );
	ais()->read( &(buffer[0]), buffer.size() );

	SWHardRef<SWTexture> texture = SWTexture::create( &(buffer[0]), buffer.size() );
	 
	m_texCache[ filePath ] = texture();
	return texture;
}

SWHardRef<SWShader> __SWAssets::loadShader( const tstring& filePath )
{
	if ( !m_accessor.isValid() ) return NULL;

	ShaderTable::iterator itor = m_shaderCache.find( filePath );
	if ( itor != m_shaderCache.end() )
	{
		if ( itor->second.isValid() )
		{
			SWLog( "asset:\"%s\" cache load", filePath.c_str() );
			return itor->second();
		}
		SWLog( "asset:\"%s\" unloaded, trying reload", filePath.c_str() );
	}

	SWHardRef<SWInputStream> ais = m_accessor()->access( filePath );
	if ( ais.isValid() == false ) return NULL;
	if ( ais()->available() <= 0 ) return NULL;

	tstring source;
	source.resize( ais()->available() );
	ais()->read( (tbyte*)&(source[0]), source.size() );

	SWHardRef<SWShader> shader = SWShader::compileShader( source );

	m_shaderCache[ filePath ] = shader();
	return shader;
}

SWHardRef<SWSpriteAnimation> __SWAssets::loadSpriteAnimation( const tstring& filePath )
{
	if ( !m_accessor.isValid() ) return NULL;

	AnimationTable::iterator itor = m_animCache.find( filePath );
	if ( itor != m_animCache.end() )
	{
		if ( itor->second.isValid() )
		{
			SWLog( "asset:\"%s\" cache load", filePath.c_str() );
			return itor->second();
		}
		SWLog( "asset:\"%s\" unloaded, trying reload", filePath.c_str() );
	}

	SWHardRef<SWInputStream> ais = m_accessor()->access( filePath );
	if ( ais.isValid() == false ) return NULL;
	if ( ais()->available() <= 0 ) return NULL;

	tstring json;
	json.resize( ais()->available() );
	ais()->read( (tbyte*)&json[0], json.size() );

	SWHardRef<SWSpriteAnimation> animation = SWSpriteAnimation::create( json );

	m_animCache[ filePath ] = animation();
	return animation;
}

bool __SWAssets::findPathOfTexture( SWTexture* texture, tstring& path )
{
	TextureTable::iterator itor = m_texCache.begin();
	for ( ; itor != m_texCache.end() ; ++itor )
	{
		if ( itor->second() == texture )
		{
			path = itor->first.str();
			return true;
		}
	}
	return false;
}

bool __SWAssets::findPathOfShader( SWShader* shader, tstring& path )
{
	ShaderTable::iterator itor = m_shaderCache.begin();
	for ( ; itor != m_shaderCache.end() ; ++itor )
	{
		if ( itor->second() == shader )
		{
			path = itor->first.str();
			return true;
		}
	}
	return false;
}
