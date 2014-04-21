#include "SWActScaleTo.h"
#include "SWAction.h"
#include "SWTransform.h"
#include "SWTime.h"

SWActScaleTo::SWActScaleTo( float duration, const tvec3& scaleTo )
	: m_duration( duration )
	, m_scaleTo( scaleTo )
{

}

SWActScaleTo::~SWActScaleTo()
{

}

bool SWActScaleTo::isDone()
{
	return ( m_spendTime >= m_duration );
}

bool SWActScaleTo::onStart()
{
	SWTransform* transform = getAction()->getComponent<SWTransform>();
	m_scaleFrom = transform->getLocalScale();
	m_spendTime = 0;
	return true;
}

void SWActScaleTo::onUpdate()
{
	m_spendTime += SWTime.getDeltaTime();
	tvec3 scale = tvec3::one;
	if ( isDone() )
	{
		scale = m_scaleTo;
	}
	else
	{
		scale = m_scaleFrom + ( m_scaleTo-m_scaleFrom ) * ( m_spendTime/m_duration );
	}

	SWTransform* transform = getAction()->getComponent<SWTransform>();
	transform->setLocalScale( scale );

}
