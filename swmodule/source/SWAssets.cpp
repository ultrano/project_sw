#include "SWAssets.h"
#include "SWLog.h"
#include "SWUtil.h"
#include "SWByteBufferStream.h"
#include "SWSprite.h"
#include "SWSpriteSequence.h"

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
			SWLog( "asset:\"%s\" is loaded in cache", filePath.c_str() );
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
			SWLog( "asset:\"%s\" is loaded in cache", filePath.c_str() );
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
			SWLog( "asset:\"%s\" is loaded in cache", filePath.c_str() );
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
			SWLog( "asset:\"%s\" is loaded in cache", filePath.c_str() );
			return itor->second();
		}
		SWLog( "asset:\"%s\" unloaded, trying reload", filePath.c_str() );
	}

	SWHardRef<SWInputStream> ais = m_accessor()->access( filePath );
	if ( ais.isValid() == false ) return NULL;
	if ( ais()->available() <= 0 ) return NULL;

	SWInputStreamReader reader( ais() );
	tstring line;
	if ( !reader.readLine( line ) ) return NULL;

	SWHardRef<SWSpriteAtlas> atlas = loadSpriteAtlas( line );
	if ( !atlas.isValid() ) return NULL;
	
	SWHardRef<SWSpriteAnimation> animation = new SWSpriteAnimation();
	while ( reader.readLine( line ) )
	{
		char name[125] = {0};
		float delay = 0;
		char buf[256] = {0};
		sscanf( line.c_str(), "%s %f %s", &name[0], &delay, &buf[0] );

		SWHardRef<SWSpriteSequence> sequence = new SWSpriteSequence();
		sequence()->setName( name );
		sequence()->setDelayPerUnit( delay );
		animation()->addSequence( sequence() );

		line = buf;
		tuint begin = 0;
		while( begin < line.length() )
		{
			tuint end = line.find( ",", begin );
			if ( end == tstring::npos ) end = line.length();
			tstring spriteName = line.substr( begin, end - begin );
			sequence()->addSprite( atlas()->find( spriteName ) );
			begin = end + 1;
		}
	}
	m_animCache[ filePath ] = animation();
	return animation;
}

SWHardRef<SWSpriteAtlas> __SWAssets::loadSpriteAtlas( const tstring& filePath )
{
	if ( !m_accessor.isValid() ) return NULL;

	tstring sheetFile = filePath + ".atlas";
	AtlasTable::iterator itor = m_atlasCache.find( sheetFile );
	if ( itor != m_atlasCache.end() )
	{
		if ( itor->second.isValid() )
		{
			SWLog( "asset:\"%s\" is loaded in cache", sheetFile.c_str() );
			return itor->second();
		}
		SWLog( "asset:\"%s\" unloaded, trying reload", sheetFile.c_str() );
	}
	
	SWHardRef<SWTexture> texture = loadTexture( filePath );
	if ( !texture.isValid() ) return NULL;

	SWHardRef<SWInputStream> ais = m_accessor()->access( sheetFile );
	if ( ais.isValid() == false ) return NULL;
	if ( ais()->available() <= 0 ) return NULL;

	SWHardRef<SWSpriteAtlas> sheet = new SWSpriteAtlas();
	SWInputStreamReader reader( ais() );

	tstring line;
	while( reader.readLine( line ) )
	{
		char name[128] = {0};
		int x, y, w, h;
		sscanf( line.c_str(), "%s = %d %d %d %d", &name[0], &x, &y, &w, &h );
		sheet()->insert( name, new SWSprite( texture, x, y, w, h ) );
	}

	m_atlasCache[ filePath ] = sheet();
	return sheet;
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
