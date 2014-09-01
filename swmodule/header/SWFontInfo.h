#ifndef __SWFontInfo_h_
#define __SWFontInfo_h_

#include "SWResource.h"

class SWInputStream;

class SWFontInfo : public SWResource
{
	SW_RTTI( SWFontInfo, SWResource );

public:

	class Char : public SWRefCountable
	{
	public:
		const tuint32 id;
		const int x, y;
		const int width, height;
		const int xoffset, yoffset;
		const int xadvence, page, chnl;

		Char( tuint32 _id, int _x, int _y, int _w, int _h, int _xo, int _yo, int _xa, int _pa, int _ch )
			: id(_id), x(_x), y(_y), width(_w), height(_h), xoffset(_xo), yoffset(_yo), xadvence(_xa), page(_pa), chnl(_ch)
		{};
	};

	class Kerning : public SWRefCountable
	{
	public:
		const tuint32 first, second;
		const int amount;
		Kerning( tuint32 _f, tuint32 _s, int _a ) : first(_f), second(_s), amount(_a) {};
	};

public:

	static SWHardRef<SWFontInfo> parse( SWInputStream* is );

	SWFontInfo();
	~SWFontInfo();

	Char* getChar( tuint32 id ) const;
	Kerning* getKerning( tuint32 first, tuint32 second ) const;
	
	tuint getLineHeight() const { return m_lineHeight; };
	tuint getScaleH() const { return m_scaleW; };
	tuint getScaleW() const { return m_scaleH; };

protected:

private:

	typedef ttable<tuint32,SWHardRef<Char>> CharTable;
	typedef ttable<tuint64,SWHardRef<Kerning>> KerningTable;

	CharTable m_charTable;
	KerningTable m_kerningTable;
	tuint m_lineHeight;
	tuint m_scaleW, m_scaleH;

};


#endif //! __SWFontInfo_h_