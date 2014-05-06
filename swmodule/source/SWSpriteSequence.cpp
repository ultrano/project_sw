#include "SWSpriteSequence.h"
#include "SWSprite.h"
#include "SWArray.h"

SWSpriteSequence::SWSpriteSequence()
	: m_sprites( new SWArray() )
{

}

SWSpriteSequence::~SWSpriteSequence()
{

}

void SWSpriteSequence::setName( const tstring& name )
{
	m_name = name;
}

const tstring& SWSpriteSequence::getName() const
{
	return m_name;
}

void SWSpriteSequence::addSprite( const SWSprite* sprite )
{
	if ( !m_sprites.isValid() ) return;
	m_sprites()->add( sprite );
}

void SWSpriteSequence::setDelayPerUnit( float delay )
{
	m_delayPerUnit = delay;
}

SWSprite* SWSpriteSequence::getSpriteAt( tuint index ) const
{
	SWObject* obj = m_sprites()->get( index );
	return swrtti_cast<SWSprite>( obj );
}

float SWSpriteSequence::getDelayPerUnit() const
{
	return m_delayPerUnit;
}

tuint SWSpriteSequence::count() const
{
	return m_sprites()->count();
}
