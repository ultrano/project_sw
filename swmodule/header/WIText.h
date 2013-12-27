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
	SWString::Value m_text;
	bool m_updateMesh;

public:

	void onAwake();
	void onRemove();

	void setFont( WIFontData* font );

	void setText( const SWString::Value& text );
	const SWString::Value& getText() const;

	void updateMesh();
};

#endif // WIText_h__