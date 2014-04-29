#include "SWActAlphaTo.h"
#include "SWAction.h"
#include "SWTime.h"
#include "WIImage.h"


SWActAlphaTo::SWActAlphaTo( float duration, float alpha )
	: m_duration( duration )
	, m_dstAlpha( alpha )
	, m_srcAlpha( 0 )
	, m_spendTIme( 0 )
{

}

SWActAlphaTo::~SWActAlphaTo()
{

}

bool SWActAlphaTo::isDone()
{
	return ( m_spendTIme >= m_duration );
}

void SWActAlphaTo::onStart()
{
	WIImage* image = getAction()->getComponent<WIImage>();
	if ( image == NULL ) return ;

	m_srcAlpha = image->getColor().a;
	m_spendTIme = 0;
}

void SWActAlphaTo::onUpdate()
{
	m_spendTIme += SWTime.getDeltaTime();
	WIImage* image = getAction()->getComponent<WIImage>();
	if ( image == NULL ) return;
	
	tcolor color = image->getColor();
	color.a = m_srcAlpha + ( m_dstAlpha-m_srcAlpha ) * ( m_spendTIme/m_duration );
	image->setColor( color );
}
