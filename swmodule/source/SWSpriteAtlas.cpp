#include "SWSpriteAtlas.h"
#include "SWSprite.h"

SWSpriteAtlas::SWSpriteAtlas()
{
}

SWSpriteAtlas::~SWSpriteAtlas()
{
}

void SWSpriteAtlas::insert( const tstring& name, SWSprite* sprite )
{
	m_spriteTable[ name ] = sprite;
}

SWSprite* SWSpriteAtlas::find( const tstring& name ) const
{
	const_iterator itor = m_spriteTable.find( name );
	if ( itor == m_spriteTable.end() ) return NULL;
	return itor->second();
}

void SWSpriteAtlas::clear()
{
	m_spriteTable.clear();
}

SWSpriteAtlas::iterator SWSpriteAtlas::begin()
{
	return m_spriteTable.begin();
}

SWSpriteAtlas::iterator SWSpriteAtlas::end()
{
	return m_spriteTable.end();
}

SWSpriteAtlas::const_iterator SWSpriteAtlas::begin() const
{
	return m_spriteTable.begin();
}

SWSpriteAtlas::const_iterator SWSpriteAtlas::end() const
{
	return m_spriteTable.end();
}
