#ifndef __SWTexture_h__
#define __SWTexture_h__

#include "SWRefCounter.h"
#include "SWRtti.h"
#include "SWType.h"

class SWTexture : public SWRefCountable
{
	SW_RTTI_ROOT( SWTexture );
public:

	static SWHardRef<SWTexture> createTexture( const tbyte* buf, tuint len );

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
