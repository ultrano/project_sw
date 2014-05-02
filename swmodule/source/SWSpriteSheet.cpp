#include "SWSpriteSheet.h"
#include "SWSprite.h"
#include "SWArray.h"


SWSpriteSheet::SWSpriteSheet( factory_constructor )
	: m_sprites( new SWArray() )
{

}

SWSpriteSheet::~SWSpriteSheet()
{

}

void SWSpriteSheet::addSprite( const SWSprite* sprite )
{
	m_sprites()->add( sprite );
}

void SWSpriteSheet::setDelayPerUnit( float delay )
{
	m_delayPerUnit = delay;
}

SWHardRef<SWArray> SWSpriteSheet::getSprites() const
{
	SWHardRef<SWArray> copy = new SWArray();

	m_sprites()->copyTo( copy() );

	return copy;
}

float SWSpriteSheet::getDelayPerUnit() const
{
	return m_delayPerUnit;
}
