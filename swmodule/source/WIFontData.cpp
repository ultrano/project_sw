#include "WIFontData.h"
#include "WIFontChar.h"
#include "SWGameContext.h"
#include "SWTable.h"
#include "SWString.h"
#include "SWNumber.h"
#include "SWArray.h"
#include "SWValue.h"
#include "SWAssets.h"

void WIFontData::setFontTexture( SWTexture* texture )
{
	m_texture = texture;
}

SWTexture* WIFontData::getFontTexture() const
{
	return m_texture();
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
	if ( data == NULL ) return;

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
		charInfo->offsetX  = charVal->find( "-xoffset" )->asNumber();
		charInfo->offsetY  = charVal->find( "-yoffset" )->asNumber();
		charInfo->advanceX = charVal->find( "-xadvance" )->asNumber();
		setChar( charInfo );
	}

	SWValue* pages = (SWValue*)font->find( "pages" );
	SWValue* page0 = pages->find( "page" );
	SWHardRef<SWTexture> texture = SWAssets.loadTexture( page0->find( "-file" )->asString() );
	setFontTexture( texture() );

	SWValue* common = (SWValue*)font->find( "common" );
	m_lineHeight = common->find( "-lineHeight" )->asNumber();
	m_scaleW     = common->find( "-scaleW" )->asNumber();
	m_scaleH     = common->find( "-scaleH" )->asNumber();
}
