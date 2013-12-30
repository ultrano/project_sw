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
	SWValue* font = tbl->find( "font" );
	SWValue* chars = (SWValue*)font->find( "chars" );
	SWValue* charList = chars->find( "char" );
	if ( charList == NULL ) return;
	for ( size_t i = 0 ; i < charList->count() ; ++i )
	{
		WIFontChar* charInfo = new WIFontChar;
		SWValue* charVal = (SWValue*)charList->get( i );
		charInfo->id = charVal->find( "-id" )->asNumber();
		charInfo->x = charVal->find( "-x" )->asNumber();
		charInfo->y = charVal->find( "-y" )->asNumber();
		charInfo->w = charVal->find( "-width" )->asNumber();
		charInfo->h = charVal->find( "-height" )->asNumber();
		charInfo->offsetX = charVal->find( "-xoffset" )->asNumber();
		charInfo->offsetY = charVal->find( "-yoffset" )->asNumber();
		setChar( charInfo );
	}

	SWValue* pages = (SWValue*)font->find( "pages" );
	SWValue* page0 = pages->find( "page" );
	setFontTexture( SW_GC.loadTexture( page0->find( "-file" )->asString() ) );
}