#include "SWAssets.h"
#include "SWByteBufferStream.h"
#include "SWOpenGL.h"

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

SWHardRef<SWInputStream> __SWAssets::load( const tstring& filePath )
{
	if ( !m_accessor.isValid() ) return NULL;

	AssetTable::iterator itor = m_cache.find( filePath );
	if ( itor != m_cache.end() ) return new SWByteBufferInputStream( itor->second() );

	SWHardRef<SWInputStream> ais = m_accessor()->access( filePath );

	SWByteBufferInputStream* bbis = new SWByteBufferInputStream( ais() );

	m_cache.insert( std::make_pair( filePath, bbis->getBuffer() ) );

	return bbis;
}

SWHardRef<SWTexture> __SWAssets::loadTexture( const tstring& filePath )
{
	if ( !m_accessor.isValid() ) return 0;

	TextureTable::iterator itor = m_texCache.find( filePath );
	if ( itor != m_texCache.end() ) return itor->second;
	
	SWHardRef<SWInputStream> ais = m_accessor()->access( filePath );
	SWHardRef<SWByteBuffer> bb = new SWByteBuffer( ais()->available() );
	
	ais()->read( bb()->getBuffer(), bb()->size() );
	
	tint id, w, h;
	id = glLoadTextureFromMemory( bb()->getBuffer(), bb()->size(), w, h );
	SWHardRef<SWTexture> texture = new SWTexture( id, w, h );

	m_texCache.insert( std::make_pair( filePath, texture ) );

	return texture;
}
