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

	tuint index = (m_spendTime / m_sequence()->getDelayPerUnit());
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

void SWActAnimate::serialize( SWObjectWriter* ow )
{
	__super::serialize( ow );
	
	ow->writeFloat( m_speed );
	ow->writeFloat( m_spendTime );
	ow->writeFloat( m_totalTime );
	ow->writeUInt( m_lastIndex );
	ow->writeObject( swrtti_cast<SWObject>( m_sequence() ) );
}

void SWActAnimate::deserialize( SWObjectReader* or )
{
	__super::deserialize( or );
	m_speed = or->readFloat();
	m_spendTime = or->readFloat();
	m_totalTime = or->readFloat();
	m_lastIndex = or->readUInt();
	m_sequence = swrtti_cast<SWSpriteSequence>( or->readObject() );
}
