#include "SWSpriteSheet.h"
#include "SWSprite.h"
#include "SWArray.h"

SWSpriteSheet::SWSpriteSheet()
	: m_sprites( new SWArray() )
{

}

SWSpriteSheet::~SWSpriteSheet()
{

}

void SWSpriteSheet::addSprite( const SWSprite* sprite )
{
	if ( !m_sprites.isValid() ) return;
	m_sprites()->add( sprite );
}

void SWSpriteSheet::setDelayPerUnit( float delay )
{
	m_delayPerUnit = delay;
}

SWSprite* SWSpriteSheet::getSpriteAt( tuint index ) const
{
	SWObject* obj = m_sprites()->get( index );
	return swrtti_cast<SWSprite>( obj );
}

SWHardRef<SWArray> SWSpriteSheet::getSpriteArray() const
{
	SWHardRef<SWArray> copy = new SWArray();

	m_sprites()->copyTo( copy() );

	return copy;
}

float SWSpriteSheet::getDelayPerUnit() const
{
	return m_delayPerUnit;
}

tuint SWSpriteSheet::count() const
{
	return m_sprites()->count();
}
