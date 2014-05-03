#ifndef __SWTexture_h__
#define __SWTexture_h__

#include "SWResource.h"

class SWTexture : public SWResource
{
	SW_RTTI( SWTexture, SWResource );
public:

	static SWHardRef<SWTexture> create( const tbyte* buf, tuint len );

public:

	~SWTexture();

	tuint getTextureID();
	tuint getWidth();
	tuint getHeight();

	void setFilter( int magFilter, int minFilter );
	void setUVWrap( int wrapU, int wrapV );

private:
	
	SWTexture();
	SWTexture( tuint id, tuint width, tuint height );
	
private:
	tuint m_texID;
	tuint m_width;
	tuint m_height;
};

#endif // __SWTexture_h__
