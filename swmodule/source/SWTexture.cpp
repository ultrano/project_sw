#include "SWTexture.h"
#include "SWAssets.h"
#include "SWOpenGL.h"

SWTexture::SWTexture( tuint id, tuint width, tuint height ) 
	: m_texID( id )
	, m_width( width )
	, m_height( height )
{
};

SWTexture::~SWTexture()
{
	glDeleteTextures( 1, &m_texID );
}

SWHardRef<SWTexture> SWTexture::create( const tbyte* buf, tuint len )
{
	int id, w, h;
	id = glLoadTextureFromMemory( buf, len, w, h );
	SWHardRef<SWTexture> texture = new SWTexture( id, w, h );
	return texture;
}

tuint SWTexture::getTextureID()
{
	return m_texID;
}

tuint SWTexture::getWidth()
{
	return m_width;
}

tuint SWTexture::getHeight()
{
	return m_height;
}

void SWTexture::setFilter( int magFilter, int minFilter )
{
	glBindTexture( GL_TEXTURE_2D, m_texID );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter );
}

void SWTexture::setUVWrap( int wrapU, int wrapV )
{
	glBindTexture( GL_TEXTURE_2D, m_texID );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapU );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapV );
}
