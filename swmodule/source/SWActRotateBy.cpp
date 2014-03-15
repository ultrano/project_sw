#include "SWActRotateBy.h"
#include "SWTransform.h"
#include "SWGameObject.h"
#include "SWAction.h"

SWActRotateBy::SWActRotateBy( float duration, const tvec3& val )
	: m_spendTime( 0 )
	, m_duration( duration )
	, m_value( val )
{
}

SWActRotateBy::~SWActRotateBy()
{
}

bool SWActRotateBy::isDone()
{
	return ( m_spendTime >= m_duration );
}

bool SWActRotateBy::onStart()
{
	m_spendTime = 0;
	if ( m_duration == 0 )
	{
		SWTransform* transform = getAction()->getComponent<SWTransform>();
		transform->move( m_value );
	}
	return true;
}

void SWActRotateBy::onUpdate( float delta )
{
	if ( isDone() ) return;
	if ( (m_spendTime + delta) > m_duration )
	{
		delta = (m_duration - m_spendTime);
		m_spendTime = m_duration;
	}
	else
	{
		m_spendTime += delta;
	}
	SWTransform* transform = getAction()->getComponent<SWTransform>();
	transform->rotate( m_value * (delta / m_duration) );
}