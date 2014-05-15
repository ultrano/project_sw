#ifndef SWSpriteSheet_h__
#define SWSpriteSheet_h__

#include "SWResource.h"

class SWSprite;
class SWSpriteSheet : public SWResource
{
	SW_RTTI( SWSpriteSheet, SWResource );

public:

	typedef ttable<tstring,SWHardRef<SWSprite>> SpriteTable;
	typedef SpriteTable::iterator iterator;
	typedef SpriteTable::const_iterator const_iterator;

public:

	SWSpriteSheet();
	~SWSpriteSheet();

	void insert( const tstring& name, SWSprite* sprite );
	SWSprite* find( const tstring& name ) const;
	void clear();

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;


private:

	SpriteTable m_spriteTable;

};

#endif // SWSpriteSheet_h__