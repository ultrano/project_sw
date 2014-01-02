#ifndef WIText_h__
#define WIText_h__

#include "SWComponent.h"
#include "SWString.h"
#include "WIFontData.h"

class SWMesh;
class WIText : public SWComponent
{
	SW_RTTI( WIText, SWComponent );

	SWHardRef<WIFontData> m_font;
	SWHardRef<SWMesh> m_mesh;
	std::wstring m_text;
	bool m_updateMesh;
	size_t m_fontSize;

public:

	void onAwake();
	void onRemove();

	void setFont( WIFontData* font );

	void setFontSize( size_t fontSize );
	size_t getFontSize() const;

	void setText( const std::wstring& text );
	const std::wstring& getText() const;

	void updateMesh();
};

#endif // WIText_h__