#include "SWSpriteData.h"

SWSpriteData::SWSpriteData()
	: m_sequences( new SWTable )
{

}

SWSpriteData::~SWSpriteData()
{
	m_sequences = NULL;
}

void SWSpriteData::addSequence( const tstring& name, const SWArray* regions )
{
	m_sequences()->insert( name, regions );
}

SWArray* SWSpriteData::findSequence( const tstring& name )
{
	return swrtti_cast<SWArray>( m_sequences()->find( name ) );
}
