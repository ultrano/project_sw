#include "SWActAnimate.h"
#include "SWAction.h"
#include "SWSpriteSequence.h"
#include "SWSpriteRenderer.h"
#include "SWTime.h"
#include "SWObjectStream.h"

SWActAnimate::SWActAnimate( float speed, SWSpriteSequence* sheet )
	: m_speed( speed )
	, m_sequence( sheet )
{

}

SWActAnimate::SWActAnimate( factory_constructor )
	: m_speed( 0 )
	, m_sequence( NULL )
{
}

SWActAnimate::~SWActAnimate()
{

}

bool SWActAnimate::isDone()
{
	return (!m_sequence.isValid()) || (m_spendTime >= m_totalTime);
}

void SWActAnimate::onStart()
{
	if ( !m_sequence.isValid() ) return;

	m_totalTime = m_sequence()->getDelayPerUnit() * m_sequence()->count();
	m_spendTime = 0;
	m_lastIndex = 0;
	changeSpriteWithAt( m_lastIndex );
}

void SWActAnimate::onUpdate()
{
	m_spendTime += SWTime.getDeltaTime() * m_speed;
	if ( isDone() ) return;

	tuint index = (tuint)(m_spendTime / m_sequence()->getDelayPerUnit());
	if ( index == m_lastIndex ) return;

	changeSpriteWithAt( index );
	m_lastIndex = index;
}

void SWActAnimate::changeSpriteWithAt( tuint index )
{
	if ( !m_sequence.isValid() ) return;

	SWSprite* sprite = m_sequence()->getSpriteAt( index );
	SWSpriteRenderer* renderer = getAction()->getComponent<SWSpriteRenderer>();
	renderer->setSprite( sprite );
}

void SWActAnimate::serialize( SWObjectWriter* writer )
{
	__super::serialize( writer );
	
	writer->writeFloat( m_speed );
	writer->writeFloat( m_spendTime );
	writer->writeFloat( m_totalTime );
	writer->writeUInt( m_lastIndex );
	writer->writeObject( swrtti_cast<SWObject>( m_sequence() ) );
}

void SWActAnimate::deserialize( SWObjectReader* reader )
{
	__super::deserialize( reader );
	m_speed = reader->readFloat();
	m_spendTime = reader->readFloat();
	m_totalTime = reader->readFloat();
	m_lastIndex = reader->readUInt();
	m_sequence = swrtti_cast<SWSpriteSequence>( reader->readObject() );
}
