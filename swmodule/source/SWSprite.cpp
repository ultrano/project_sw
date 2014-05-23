#include "SWSprite.h"
#include "SWTexture.h"
#include "SWAssets.h"
#include "SWObjectStream.h"

SWSprite::SWSprite( SWHardRef<SWTexture> texture, int x, int y, int width, int height )
	: m_texture( texture )
	, m_offset( (float)x, (float)y )
	, m_size( (float)width, (float)height )
	, m_scaledOffset( 0.0f, 0.0f )
	, m_scaledSize( 0.0f, 0.0f )
{
	if ( m_texture.isValid() )
	{
		float texW = (float)m_texture()->getWidth();
		float texH = (float)m_texture()->getHeight();
		m_scaledOffset.x = x/texW;
		m_scaledOffset.y = y/texH;
		m_scaledSize.x = width/texW;
		m_scaledSize.y = height/texH;
	}
}

SWSprite::SWSprite( factory_constructor )
	: m_texture( NULL )
	, m_offset( 0, 0 )
	, m_size( 0, 0 )
	, m_scaledOffset( 0, 0 )
	, m_scaledSize( 0, 0 )
{
}

SWSprite::~SWSprite()
{

}

SWTexture* SWSprite::getTexture() const
{
	return m_texture();
}

const tvec2& SWSprite::getOffset() const
{
	return m_offset;
}

const tvec2& SWSprite::getSize() const
{
	return m_size;
}

const tvec2& SWSprite::getScaledOffset() const
{
	return m_scaledOffset;
}

const tvec2& SWSprite::getScaledSize() const
{
	return m_scaledSize;
}

void SWSprite::serialize( SWObjectWriter* ow )
{
	tstring path;
	bool isAsset = SWAssets.findPathOfTexture( m_texture(), path );

	ow->writeBool( isAsset );
	if ( isAsset ) ow->writeString( path );

	ow->writeVec2( m_offset );
	ow->writeVec2( m_size );
	ow->writeVec2( m_scaledOffset );
	ow->writeVec2( m_scaledSize );
}

void SWSprite::deserialize( SWObjectReader* or )
{
	bool isAsset = or->readBool();
	if ( isAsset )
	{
		tstring path;
		or->readString( path );
		m_texture = SWAssets.loadTexture( path );
	}

	or->readVec2( m_offset );
	or->readVec2( m_size );
	or->readVec2( m_scaledOffset );
	or->readVec2( m_scaledSize );
}