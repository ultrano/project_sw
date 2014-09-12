#include "SWActScale.h"
#include "SWTransform.h"
#include "SWGameObject.h"
#include "SWAction.h"
#include "SWTime.h"
#include "SWObjectStream.h"

SWActScale::SWActScale( float duration, const tvec3& from, const tvec3& to )
	: m_duration( duration )
	, m_spendTime( 0 )
	, m_from( from )
	, m_to( to )
{

}

SWActScale::SWActScale( factory_constructor )
	: m_duration( 0 )
	, m_spendTime( 0 )
	, m_from( tvec3::zero )
	, m_to( tvec3::zero )
{

}

SWActScale::~SWActScale()
{

}

bool SWActScale::isDone()
{
	return ( m_spendTime >= m_duration );
}

void SWActScale::onStart()
{
	m_spendTime = 0;
}

void SWActScale::onUpdate()
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
	transform->setLocalScale( pos );
}

void SWActScale::serialize( SWObjectWriter* writer )
{
	__super::serialize( writer );
	writer->writeFloat( m_duration );
	writer->writeFloat( m_spendTime );
	writer->writeVec3( m_from );
	writer->writeVec3( m_to );
}

void SWActScale::deserialize( SWObjectReader* reader )
{
	__super::deserialize( reader );
	m_duration = reader->readFloat();
	m_spendTime = reader->readFloat();
	reader->readVec3( m_from );
	reader->readVec3( m_to );
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

SWActScaleTo::SWActScaleTo( float duration, const tvec3& to )
	: SWActScale( duration, tvec3::zero, to )
{

}

SWActScaleTo::SWActScaleTo( factory_constructor arg )
	: SWActScale( arg )
{

}

SWActScaleTo::~SWActScaleTo()
{

}

void SWActScaleTo::onStart()
{
	__super::onStart();
	SWTransform* transform = getAction()->getComponent<SWTransform>();
	m_from = transform->getLocalScale();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


SWActScaleFrom::SWActScaleFrom( float duration, const tvec3& from )
	: SWActScale( duration, from, tvec3::zero )
{

}

SWActScaleFrom::SWActScaleFrom( factory_constructor arg )
	: SWActScale( arg )
{

}

SWActScaleFrom::~SWActScaleFrom()
{

}

void SWActScaleFrom::onStart()
{
	__super::onStart();
	SWTransform* transform = getAction()->getComponent<SWTransform>();
	m_to = transform->getLocalScale();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

SWActScaleBy::SWActScaleBy( float duration, const tvec3& by )
	: SWActScale( duration, tvec3::zero, tvec3::zero )
	, m_by( by )
{

}

SWActScaleBy::SWActScaleBy( factory_constructor arg )
	: SWActScale( arg )
{

}

SWActScaleBy::~SWActScaleBy()
{

}

void SWActScaleBy::onStart()
{
	__super::onStart();
	SWTransform* transform = getAction()->getComponent<SWTransform>();
	m_from = transform->getLocalScale();
	m_to = m_from + (m_by * m_duration);
}

void SWActScaleBy::serialize( SWObjectWriter* writer )
{
	__super::serialize( writer );
	writer->writeVec3( m_by );
}

void SWActScaleBy::deserialize( SWObjectReader* reader )
{
	__super::deserialize( reader );
	reader->readVec3( m_by );
}