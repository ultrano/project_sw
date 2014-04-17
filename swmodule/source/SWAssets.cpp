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

	tarray<tbyte> buffer;
	buffer.resize( ais()->available() );
	ais()->read( &(buffer[0]), buffer.size() );

	SWHardRef<SWTexture> texture = SWTexture::createTexture( &(buffer[0]), buffer.size() );

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

	tstring source;
	source.resize( ais()->available() );
	ais()->read( (tbyte*)&(source[0]), source.size() );

	SWHardRef<SWShader> shader = SWShader::compileShader( source );

	m_shaderCache[ filePath ] = shader();
	return shader;
}
