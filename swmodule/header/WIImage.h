#ifndef __UIImage_h_
#define __UIImage_h_

#include "SWComponent.h"

class SWTexture;

class WIImage : public SWComponent
{
	SW_RTTI( WIImage, SWComponent );

public:

	WIImage();
	~WIImage();

	void setSize( float width, float height );
	void setSizeToTexture( float scaleW = 1, float scaleH = 1 );

	float getWidth() const { return m_width; }
	float getHeight() const { return m_height; };

	void setAlignV( int align );
	void setAlignH( int align );
	void setAlign( int alignV, int alignH );
	void setUVRect( float left, float top, float right, float bottom );

	void setTexture( SWTexture* texture );
	void setTexture( SWHardRef<SWTexture> texture );

	void setColor( float r, float g, float b, float a );

private:

	virtual void onAwake();
	virtual void onRemove();
	void onUpdate();

private:

	float m_width;
	float m_height;
	trect m_uvRect;

	int m_alignV;
	int m_alignH;

	bool m_updateVert;
	bool m_updateTex;
};

#endif // __UIImage_h_