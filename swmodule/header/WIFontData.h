#ifndef WIFontData_h__
#define WIFontData_h__

#include "SWMemory.h"
#include "SWObject.h"
#include "SWTable.h"

class WIFontChar;
class WIFontData : public SWObject
{
	SW_RTTI( WIFontData, SWObject );

	typedef ttable< int,SWHardRef<WIFontChar> > CharTable;

	int m_texID;
	CharTable m_charTable;
	int m_lineHeight;
	int m_scaleW;
	int m_scaleH;

public:

	void setFontTexture( int texID );
	int getFontTexture() const;

	WIFontChar* getChar( int id );
	void setChar( WIFontChar* fontChar );

	int getLineHeight() const { return m_lineHeight; };
	int getScaleW() const { return m_scaleW; };
	int getScaleH() const { return m_scaleH; };

	void load( SWObject* data );

};

#endif // WIFontData_h__