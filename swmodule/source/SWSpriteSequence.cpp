#include "SWSpriteSequence.h"
#include "SWSprite.h"
#include "SWArray.h"
#include "SWObjectStream.h"

SWSpriteSequence::SWSpriteSequence()
	: m_sprites( new SWArray() )
{

}
SWSpriteSequence::SWSpriteSequence( factory_constructor )
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

void SWSpriteSequence::serialize( SWObjectWriter* writer )
{
	writer->writeString( m_name );
	writer->writeFloat( m_delayPerUnit );

	tuint count = m_sprites()->count();
	writer->writeUInt( count );
	for ( tuint i = 0 ; i < count ; ++i )
	{
		writer->writeObject( m_sprites()->get( i ) );
	}
}

void SWSpriteSequence::deserialize( SWObjectReader* reader )
{
	reader->readString( m_name );
	m_delayPerUnit = reader->readFloat();

	m_sprites()->clear();
	tuint count = reader->readUInt();
	for ( tuint i = 0 ; i < count ; ++i )
	{
		m_sprites()->add( reader->readObject() );
	}
}