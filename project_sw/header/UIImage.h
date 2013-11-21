#ifndef __UIImage_h_
#define __UIImage_h_

#include "SWComponent.h"
class UIImage : public SWComponent
{
	SW_RTTI( UIImage, SWComponent );

	float m_width;
	float m_height;

	int m_texID;
	
	int m_alignV;
	int m_alignH;

	bool m_updateMesh;

	virtual void onAwake();
	virtual void onRemove();
	void onUpdate();

public:

	void setSize( float width, float height );
	void setSizeToTexture( float scaleW = 1, float scaleH = 1 );
	
	float getWidth() const { return m_width; }
	float getHeight() const { return m_height; };

	void setAlignV( int align );
	void setAlignH( int align );
	void setAlign( int alignV, int alignH );

	void setTexture( const std::string& filePath );

};

#endif // __UIImage_h_