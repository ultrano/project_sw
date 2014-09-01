#include "SWFontInfo.h"
#include "SWIOStream.h"

SWHardRef<SWFontInfo> SWFontInfo::parse( SWInputStream* is )
{
	SWHardRef<SWInputStream> vital = is;
	if ( vital.isValid() == false ) return NULL;

	tstring line;
	SWInputStreamReader reader( is );

	reader.readLine( line );
	reader.readLine( line );
	reader.readLine( line );

	int count = 0;
	tuint32 id;
	int x,y,w,h,xoffset,yoffset,xadvence,page,chnl;
	
	reader.readLine( line );
	sscanf( line.c_str(),"chars count=%d", &count );

	SWHardRef<SWFontInfo> fontInfo = new SWFontInfo();
	while ( count-- )
	{
		reader.readLine( line );
		sscanf( line.c_str(), "char id=%d x=%d y=%d width=%d height=%d xoffset=%d yoffset=%d xadvance=%d page=%d chnl=%d"
			, &id, &x, &y, &w, &h, &xoffset, &yoffset, &xadvence, &page, &chnl);

		fontInfo()->m_charTable[ id ] = new Char( id, x, y, w, h, xoffset, yoffset, xadvence, page, chnl );
	}
	
	reader.readLine( line );
	sscanf( line.c_str(),"kernings count=%d", &count );

	tuint32 first, second;
	int amount;
	while ( count-- )
	{
		reader.readLine( line );
		sscanf( line.c_str(), "kerning first=%d second=%d amount=%d", &first, &second, &amount );
		tuint64 key = first;
		key = (key << 32) | second;
		fontInfo()->m_kerningTable[ key ] = new Kerning( first, second, amount );
	}

	return fontInfo();
}

SWFontInfo::SWFontInfo()
{
}

SWFontInfo::~SWFontInfo()
{
}

SWFontInfo::Char* SWFontInfo::getChar( tuint32 id ) const
{
	CharTable::const_iterator itor = m_charTable.find( id );
	return ( itor != m_charTable.end() )? itor->second() : NULL;
}

SWFontInfo::Kerning* SWFontInfo::getKerning( tuint32 first, tuint32 second ) const
{
	tuint64 key = first;
	key = (key<<32) | second;
	KerningTable::const_iterator itor = m_kerningTable.find( key );
	return ( itor != m_kerningTable.end() )? itor->second() : NULL;
}