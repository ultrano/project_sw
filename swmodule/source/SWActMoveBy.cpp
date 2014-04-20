#include "SWActMoveBy.h"
#include "SWTransform.h"
#include "SWGameObject.h"
#include "SWAction.h"
#include "SWTime.h"

SWActMoveBy::SWActMoveBy( float duration, const tvec3& val )
	: m_spendTime( 0 )
	, m_duration( duration )
	, m_value( val )
{
}

SWActMoveBy::~SWActMoveBy()
{
}

bool SWActMoveBy::isDone()
{
	return ( m_spendTime >= m_duration );
}

bool SWActMoveBy::onStart()
{
	m_spendTime = 0;
	if ( m_duration == 0 )
	{
		SWTransform* transform = getAction()->getComponent<SWTransform>();
		transform->move( m_value );
	}
	return true;
}

void SWActMoveBy::onUpdate()
{
	if ( isDone() ) return;
	float delta = SWTime.getDeltaTime();
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
	transform->move( m_value * (delta / m_duration) );
}