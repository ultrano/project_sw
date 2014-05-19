#ifndef SWSpriteAtlas_h__
#define SWSpriteAtlas_h__

#include "SWResource.h"

class SWSprite;
class SWSpriteAtlas : public SWResource
{
	SW_RTTI( SWSpriteAtlas, SWResource );

public:

	typedef ttable<tstring,SWHardRef<SWSprite>> SpriteTable;
	typedef SpriteTable::iterator iterator;
	typedef SpriteTable::const_iterator const_iterator;

public:

	SWSpriteAtlas();
	~SWSpriteAtlas();

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

#endif // SWSpriteAtlas_h__