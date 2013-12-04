#include "SWSpriteData.h"

SWSpriteData::SWSpriteData()
	: m_sequences( new SWTable )
{

}

SWSpriteData::~SWSpriteData()
{
	m_sequences = NULL;
}

void SWSpriteData::addSequence( const SWString::Value& name, const SWArray* regions )
{
	m_sequences()->insert( name, regions );
}

SWArray* SWSpriteData::findSequence( const SWString::Value& name )
{
	return swrtti_cast<SWArray>( m_sequences()->find( name ) );
}
