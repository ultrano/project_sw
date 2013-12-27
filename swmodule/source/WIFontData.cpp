#include "WIFontData.h"
#include "WIFontChar.h"
#include "SWGameContext.h"
#include "SWTable.h"
#include "SWString.h"
#include "SWNumber.h"
#include "SWArray.h"
#include "SWValue.h"

void WIFontData::setFontTexture( int texID )
{
	m_texID = texID;
}

int WIFontData::getFontTexture() const
{
	return m_texID;
}

WIFontChar* WIFontData::getChar( int id )
{
	CharTable::iterator itor = m_charTable.find( id );
	if ( itor == m_charTable.end() ) return NULL;
	return itor->second();
}

void WIFontData::setChar( WIFontChar* fontChar )
{
	if ( fontChar == NULL ) return;
	m_charTable[fontChar->id] = fontChar;
}

void WIFontData::load( SWObject* data )
{
	SWValue* tbl = (SWValue*)( data );
	SWArray* characters = swrtti_cast<SWArray>( tbl->find( "characters" ) );
	if ( characters == NULL ) return;
	SWArray::iterator itor = characters->begin();
	for ( ; itor != characters->end() ; ++itor )
	{
		WIFontChar* charInfo = new WIFontChar;
		SWValue* charVal = (SWValue*)(*itor)();
		charInfo->id = charVal->find( "id" )->asNumber();
		charInfo->x = charVal->find( "x" )->asNumber();
		charInfo->y = charVal->find( "y" )->asNumber();
		charInfo->w = charVal->find( "w" )->asNumber();
		charInfo->h = charVal->find( "h" )->asNumber();
		charInfo->offsetX = charVal->find( "offsetX" )->asNumber();
		charInfo->offsetY = charVal->find( "offsetY" )->asNumber();
		setChar( charInfo );
	}

	setFontTexture( SW_GC.loadTexture( tbl->find( "font_image" )->asString() ) );
}