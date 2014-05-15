#include "SWSpriteSheet.h"
#include "SWSprite.h"

SWSpriteSheet::SWSpriteSheet()
{
}

SWSpriteSheet::~SWSpriteSheet()
{
}

void SWSpriteSheet::insert( const tstring& name, SWSprite* sprite )
{
	m_spriteTable[ name ] = sprite;
}

SWSprite* SWSpriteSheet::find( const tstring& name ) const
{
	const_iterator itor = m_spriteTable.find( name );
	if ( itor == m_spriteTable.end() ) return NULL;
	return itor->second();
}

void SWSpriteSheet::clear()
{
	m_spriteTable.clear();
}

SWSpriteSheet::iterator SWSpriteSheet::begin()
{
	return m_spriteTable.begin();
}

SWSpriteSheet::iterator SWSpriteSheet::end()
{
	return m_spriteTable.end();
}

SWSpriteSheet::const_iterator SWSpriteSheet::begin() const
{
	return m_spriteTable.begin();
}

SWSpriteSheet::const_iterator SWSpriteSheet::end() const
{
	return m_spriteTable.end();
}
