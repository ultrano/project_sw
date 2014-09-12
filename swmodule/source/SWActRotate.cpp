#include "SWActRotate.h"
#include "SWTransform.h"
#include "SWGameObject.h"
#include "SWAction.h"
#include "SWTime.h"
#include "SWObjectStream.h"

SWActRotate::SWActRotate( float duration, const tvec3& from, const tvec3& to )
	: m_duration( duration )
	, m_spendTime( 0 )
	, m_from( from )
	, m_to( to )
{

}

SWActRotate::SWActRotate( factory_constructor )
	: m_duration( 0 )
	, m_spendTime( 0 )
	, m_from( tvec3::zero )
	, m_to(  tvec3::zero )
{

}

SWActRotate::~SWActRotate()
{

}

bool SWActRotate::isDone()
{
	return ( m_spendTime >= m_duration );
}

void SWActRotate::onStart()
{
	m_spendTime = 0;
}

void SWActRotate::onUpdate()
{
	if ( isDone() ) return;

	float delta = SWTime.getDeltaTime();
	if ( (m_spendTime + delta) > m_duration )
	{
		delta = ( m_duration - m_spendTime );
	}

	m_spendTime += delta;
	tvec3 euler = m_from + ((m_to - m_from) * (m_spendTime/m_duration));

	SWTransform* transform = getAction()->getComponent<SWTransform>();
	transform->setLocalRotate( euler );
}

void SWActRotate::serialize( SWObjectWriter* writer )
{
	__super::serialize( writer );
	writer->writeFloat( m_duration );
	writer->writeFloat( m_spendTime );
	writer->writeVec3( m_from );
	writer->writeVec3( m_to );
}

void SWActRotate::deserialize( SWObjectReader* reader )
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

SWActRotateTo::SWActRotateTo( float duration, const tvec3& to )
	: SWActRotate( duration, tvec3::zero, to )
{

}

SWActRotateTo::SWActRotateTo( factory_constructor arg )
	: SWActRotate( arg )
{

}

SWActRotateTo::~SWActRotateTo()
{

}

void SWActRotateTo::onStart()
{
	__super::onStart();
	SWTransform* transform = getAction()->getComponent<SWTransform>();
	m_from = transform->getLocalEuler();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


SWActRotateFrom::SWActRotateFrom( float duration, const tvec3& from )
	: SWActRotate( duration, from, tvec3::zero )
{

}

SWActRotateFrom::SWActRotateFrom( factory_constructor arg )
	: SWActRotate( arg )
{

}

SWActRotateFrom::~SWActRotateFrom()
{

}

void SWActRotateFrom::onStart()
{
	__super::onStart();
	SWTransform* transform = getAction()->getComponent<SWTransform>();
	m_to = transform->getLocalEuler();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

SWActRotateBy::SWActRotateBy( float duration, const tvec3& by )
	: SWActRotate( duration, tvec3::zero, tvec3::zero )
	, m_by( by )
{

}

SWActRotateBy::SWActRotateBy( factory_constructor arg )
	: SWActRotate( arg )
{

}

SWActRotateBy::~SWActRotateBy()
{

}

void SWActRotateBy::onStart()
{
	__super::onStart();
	SWTransform* transform = getAction()->getComponent<SWTransform>();
	m_from = transform->getLocalEuler();
	m_to = m_from + (m_by * m_duration);
}

void SWActRotateBy::serialize( SWObjectWriter* writer )
{
	__super::serialize( writer );
	writer->writeVec3( m_by );
}

void SWActRotateBy::deserialize( SWObjectReader* reader )
{
	__super::deserialize( reader );
	reader->readVec3( m_by );
}