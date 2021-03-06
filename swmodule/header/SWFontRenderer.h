#ifndef SWFontRenderer_h__
#define SWFontRenderer_h__

#include "SWRenderer.h"

class SWMesh;
class SWTexture;
class SWMaterial;
class SWFontInfo;
class SWFontRenderer : public SWRenderer
{
	SW_RTTI( SWFontRenderer, SWRenderer );

public:

	SWFontRenderer( factory_constructor );
	~SWFontRenderer();
	
	void setText( const char* format, ... );
	void setText( const tstring& text );
	const tstring& getText() const;

	void setFontTexture( SWTexture* texture );
	SWTexture* getFontTexture() const;

	void setFontInfo( SWFontInfo* fontInfo );
	SWFontInfo* getFontInfo() const;

	void setColor( const tcolor& color );
	const tcolor& getColor() const;

	void setAlignH( int LCR );
	int  getAlignH() const;

	void setAlignV( int TCB );
	int  getAlignV() const;

protected:

	virtual void onAwake();
	virtual void render( SWCamera* camera );
	void serialize( SWObjectWriter* writer );
	void deserialize( SWObjectReader* reader );

private:

	void updateMesh();
	tuint getLineWidth( const tstring& text, tuint offset ) const;
	tuint getLinesHeight( const tstring& text ) const;

private:
	
	int m_alignH;
	int m_alignV;
	bool m_textChanged;
	
	tstring m_text;
	SWHardRef<SWTexture>  m_texture;
	SWHardRef<SWFontInfo> m_fontInfo;
	tcolor  m_color;
};

#endif //! SWFontRenderer_h__