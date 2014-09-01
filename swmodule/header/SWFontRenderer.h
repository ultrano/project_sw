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
	
	void setText( const tstring& text );
	const tstring& getText() const;

	void setFontTexture( SWTexture* texture );
	SWTexture* getFontTexture() const;

	void setFontInfo( SWFontInfo* fontInfo );
	SWFontInfo* getFontInfo() const;

	void setColor( const tcolor& color );
	const tcolor& getColor() const;

protected:

	virtual void onAwake();
	virtual void render( SWCamera* camera );
	void serialize( SWObjectWriter* ow );
	void deserialize( SWObjectReader* or );

private:

	void updateMesh();

private:
	
	SWHardRef<SWMesh>     m_mesh;
	SWHardRef<SWMaterial> m_material;
	bool m_textChanged;
	
	tstring m_text;
	SWHardRef<SWTexture>  m_texture;
	SWHardRef<SWFontInfo> m_fontInfo;
	tcolor  m_color;
};

#endif //! SWFontRenderer_h__