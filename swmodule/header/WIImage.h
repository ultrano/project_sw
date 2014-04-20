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
	void setUVRect( float x, float y, float width, float height );

	void setTexture( SWTexture* texture );
	void setTexture( SWHardRef<SWTexture> texture );
	SWTexture* getTexture() const;

	void setColor( float r, float g, float b, float a );
	void setColor( const tcolor& color );
	const tcolor& getColor() const;

private:

	virtual void onAwake();
	virtual void onRemove();
	void onUpdate();
	void onPreRender();

private:

	float m_width;
	float m_height;
	trect m_uvRect;

	int m_alignV;
	int m_alignH;

	bool m_updateVert;
	bool m_updateTex;

	SWHardRef<SWTexture> m_texture;
	tcolor m_color;
};

#endif // __UIImage_h_