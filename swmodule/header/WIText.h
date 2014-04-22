#ifndef WIText_h__
#define WIText_h__

#include "SWComponent.h"
#include "SWString.h"
#include "WIFontData.h"

class SWMesh;
class WIText : public SWComponent
{
	SW_RTTI( WIText, SWComponent );

public:

	WIText();
	WIText( factory_constructor );
	~WIText();

	void setFont( WIFontData* font );

	void setFontSize( size_t fontSize );
	size_t getFontSize() const;

	void setText( const twstring& text );
	const twstring& getText() const;

	void updateMesh();

private:

	void onAwake();
	void onRemove();
	void onPreRender();

private:

	SWHardRef<WIFontData> m_font;
	SWHardRef<SWMesh> m_mesh;
	twstring m_text;
	bool m_updateMesh;
	size_t m_fontSize;
};

#endif // WIText_h__