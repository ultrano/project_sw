#include "SWActColor.h"
#include "SWTransform.h"
#include "SWGameObject.h"
#include "SWAction.h"
#include "SWTime.h"
#include "SWSpriteRenderer.h"
#include "SWObjectStream.h"

SWActColor::SWActColor( float duration, const tcolor& from, const tcolor& to )
	: m_duration( duration )
	, m_spendTime( 0 )
	, m_from( from )
	, m_to( to )
{

}

SWActColor::~SWActColor()
{

}

bool SWActColor::isDone()
{
	return ( m_spendTime >= m_duration );
}

void SWActColor::onStart()
{
	m_spendTime = 0;
}

void SWActColor::onUpdate()
{
	if ( isDone() ) return;

	float delta = SWTime.getDeltaTime();
	if ( (m_spendTime + delta) > m_duration )
	{
		delta = ( m_duration - m_spendTime );
	}

	m_spendTime += delta;

	SWSpriteRenderer* renderer = getAction()->getComponent<SWSpriteRenderer>();
	if ( renderer == NULL ) return;
	
	float rate = (m_spendTime/m_duration);
	float r = m_from.r + ((m_to.r - m_from.r) * rate);
	float g = m_from.g + ((m_to.g - m_from.g) * rate);
	float b = m_from.b + ((m_to.b - m_from.b) * rate);
	float a = m_from.a + ((m_to.a - m_from.a) * rate);

	renderer->setColor( tcolor( r, g, b, a ) );
}

void SWActColor::serialize( SWObjectWriter* ow )
{
	__super::serialize( ow );
	ow->writeFloat( m_duration );
	ow->writeFloat( m_spendTime );
	ow->writeColor( m_from );
	ow->writeColor( m_to );
}

void SWActColor::deserialize( SWObjectReader* or )
{
	__super::deserialize( or );
	m_duration = or->readFloat();
	m_spendTime = or->readFloat();
	or->readColor( m_from );
	or->readColor( m_to );
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

SWActColorTo::SWActColorTo( float duration, const tcolor& to )
	: SWActColor( duration, tcolor(0,0,0,0), to )
{

}

SWActColorTo::~SWActColorTo()
{

}

void SWActColorTo::onStart()
{
	__super::onStart();
	SWSpriteRenderer* renderer = getAction()->getComponent<SWSpriteRenderer>();
	if ( renderer == NULL ) return;
	m_from = renderer->getColor();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


SWActColorFrom::SWActColorFrom( float duration, const tcolor& from )
	: SWActColor( duration, from, tcolor(0,0,0,0) )
{

}

SWActColorFrom::~SWActColorFrom()
{

}

void SWActColorFrom::onStart()
{
	__super::onStart();
	SWSpriteRenderer* renderer = getAction()->getComponent<SWSpriteRenderer>();
	if ( renderer == NULL ) return;
	m_to = renderer->getColor();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

SWActColorBy::SWActColorBy( float duration, const tcolor& by )
	: SWActColor( duration, tcolor(0,0,0,0), tcolor(0,0,0,0) )
	, m_by( by )
{

}

SWActColorBy::~SWActColorBy()
{

}

void SWActColorBy::onStart()
{
	__super::onStart();
	SWSpriteRenderer* renderer = getAction()->getComponent<SWSpriteRenderer>();
	if ( renderer == NULL ) return;
	m_from = renderer->getColor();
	m_to.r = m_from.r + (m_by.r* m_duration);
	m_to.g = m_from.g + (m_by.g* m_duration);
	m_to.b = m_from.b + (m_by.b* m_duration);
	m_to.a = m_from.a + (m_by.a* m_duration);
}

void SWActColorBy::serialize( SWObjectWriter* ow )
{
	__super::serialize( ow );
	ow->writeColor( m_by );
}

void SWActColorBy::deserialize( SWObjectReader* or )
{
	__super::deserialize( or );
	or->readColor( m_by );
}