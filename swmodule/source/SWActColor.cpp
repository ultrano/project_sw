#include "SWActColor.h"
#include "SWTransform.h"
#include "SWGameObject.h"
#include "SWAction.h"
#include "SWTime.h"
#include "SWRenderer.h"
#include "SWMaterial.h"
#include "SWObjectStream.h"

SWActColor::SWActColor( float duration, const tcolor& from, const tcolor& to )
	: m_duration( duration )
	, m_spendTime( 0 )
	, m_from( from )
	, m_to( to )
{

}

SWActColor::SWActColor( factory_constructor )
	: m_duration( 0 )
	, m_spendTime( 0 )
	, m_from( tcolor( 0,0,0,0 ) )
	, m_to( m_from )
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

	//! check renderer material
	SWRenderer* renderer = getAction()->getComponent<SWRenderer>();
	if ( renderer ) m_material = renderer->getMaterial();
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
	
	float rate = (m_spendTime/m_duration);
	float r = m_from.r + ((m_to.r - m_from.r) * rate);
	float g = m_from.g + ((m_to.g - m_from.g) * rate);
	float b = m_from.b + ((m_to.b - m_from.b) * rate);
	float a = m_from.a + ((m_to.a - m_from.a) * rate);
	
	setColor( tcolor( r, g, b, a ) );
}

void SWActColor::serialize( SWObjectWriter* writer )
{
	__super::serialize( writer );
	writer->writeFloat( m_duration );
	writer->writeFloat( m_spendTime );
	writer->writeColor( m_from );
	writer->writeColor( m_to );
}

void SWActColor::deserialize( SWObjectReader* reader )
{
	__super::deserialize( reader );
	m_duration = reader->readFloat();
	m_spendTime = reader->readFloat();
	reader->readColor( m_from );
	reader->readColor( m_to );
}

void SWActColor::setColor( const tcolor& color )
{
	//! check material
	if ( !m_material.isValid() ) return;

	m_material()->setVector4( "COLOR", tquat( color.r, color.g, color.b, color.a ) );
}

void SWActColor::getColor( tcolor& color ) const
{
	//! check material
	if ( !m_material.isValid() ) return;

	tquat vec4;
	m_material()->getVector4( "COLOR", vec4 );
	color.r = vec4.x;
	color.g = vec4.y;
	color.b = vec4.z;
	color.a = vec4.w;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

SWActColorTo::SWActColorTo( float duration, const tcolor& to )
	: SWActColor( duration, tcolor(0,0,0,0), to )
{

}

SWActColorTo::SWActColorTo( factory_constructor arg )
	: SWActColor( arg )
{
}

SWActColorTo::~SWActColorTo()
{

}

void SWActColorTo::onStart()
{
	__super::onStart();

	getColor( m_from );
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


SWActColorFrom::SWActColorFrom( float duration, const tcolor& from )
	: SWActColor( duration, from, tcolor(0,0,0,0) )
{

}

SWActColorFrom::SWActColorFrom( factory_constructor arg )
	: SWActColor( arg )
{
}

SWActColorFrom::~SWActColorFrom()
{

}

void SWActColorFrom::onStart()
{
	__super::onStart();

	getColor( m_to );
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

SWActColorBy::SWActColorBy( float duration, const tcolor& by )
	: SWActColor( duration, tcolor(0,0,0,0), tcolor(0,0,0,0) )
	, m_by( by )
{

}

SWActColorBy::SWActColorBy( factory_constructor arg )
	: SWActColor( arg )
{
}

SWActColorBy::~SWActColorBy()
{

}

void SWActColorBy::onStart()
{
	__super::onStart();

	getColor( m_from );

	m_to.r = m_from.r + (m_by.r* m_duration);
	m_to.g = m_from.g + (m_by.g* m_duration);
	m_to.b = m_from.b + (m_by.b* m_duration);
	m_to.a = m_from.a + (m_by.a* m_duration);
}

void SWActColorBy::serialize( SWObjectWriter* writer )
{
	__super::serialize( writer );
	writer->writeColor( m_by );
}

void SWActColorBy::deserialize( SWObjectReader* reader )
{
	__super::deserialize( reader );
	reader->readColor( m_by );
}