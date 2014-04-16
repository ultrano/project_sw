#include "SWTexture.h"
#include "SWAssets.h"

SWTexture::SWTexture( tuint id, tuint width, tuint height ) 
	: m_texID( id )
	, m_width( width )
	, m_height( height )
{
};

SWTexture::~SWTexture()
{
}