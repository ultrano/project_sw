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

SWHardRef<SWTexture> SWTexture::createTexture( const tbyte* buf, tuint len )
{
	int id, w, h;
	id = glLoadTextureFromMemory( buf, len, w, h );
	SWHardRef<SWTexture> texture = new SWTexture( id, w, h );
	return texture;
}
