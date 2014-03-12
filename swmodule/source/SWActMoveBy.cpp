#include "SWActMoveBy.h"
#include "SWTransform.h"
#include "SWGameObject.h"
#include "SWAction.h"

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
	return true;
}

void SWActMoveBy::onUpdate( float delta )
{
	m_spendTime += delta;
	if ( m_spendTime > m_duration ) delta = (m_spendTime - m_duration);

	SWTransform* transform = getAction()->getComponent<SWTransform>();
	transform->move( m_value * (delta / m_duration) );
}