#include "SWActMove.h"
#include "SWTransform.h"
#include "SWGameObject.h"
#include "SWAction.h"
#include "SWTime.h"

SWActMove::SWActMove( float duration, const tvec3& from, const tvec3& to )
	: m_duration( duration )
	, m_spendTime( 0 )
	, m_from( from )
	, m_to( to )
{

}

SWActMove::~SWActMove()
{

}

bool SWActMove::isDone()
{
	return ( m_spendTime >= m_duration );
}

void SWActMove::onStart()
{
	m_spendTime = 0;
}

void SWActMove::onUpdate()
{
	if ( isDone() ) return;

	float delta = SWTime.getDeltaTime();
	if ( (m_spendTime + delta) > m_duration )
	{
		delta = ( m_duration - m_spendTime );
	}

	m_spendTime += delta;
	tvec3 pos = m_from + ((m_to - m_from) * (m_spendTime/m_duration));

	SWTransform* transform = getAction()->getComponent<SWTransform>();
	transform->setLocalPosition( pos );
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

SWActMoveTo::SWActMoveTo( float duration, const tvec3& to )
	: SWActMove( duration, tvec3::zero, to )
{

}

SWActMoveTo::~SWActMoveTo()
{

}

void SWActMoveTo::onStart()
{
	__super::onStart();
	SWTransform* transform = getAction()->getComponent<SWTransform>();
	m_from = transform->getLocalPosition();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


SWActMoveFrom::SWActMoveFrom( float duration, const tvec3& from )
	: SWActMove( duration, from, tvec3::zero )
{

}

SWActMoveFrom::~SWActMoveFrom()
{

}

void SWActMoveFrom::onStart()
{
	__super::onStart();
	SWTransform* transform = getAction()->getComponent<SWTransform>();
	m_to = transform->getLocalPosition();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

SWActMoveBy::SWActMoveBy( float duration, const tvec3& by )
	: SWActMove( duration, tvec3::zero, tvec3::zero )
	, m_by( by )
{

}

SWActMoveBy::~SWActMoveBy()
{

}

void SWActMoveBy::onStart()
{
	__super::onStart();
	SWTransform* transform = getAction()->getComponent<SWTransform>();
	m_from = transform->getLocalPosition();
	m_to = m_from + (m_by * m_duration);
}
