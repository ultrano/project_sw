#include "SWAnimation.h"
#include "SWGameObject.h"
#include "SWAnimationClip.h"
#include "SWTime.h"

SWAnimation::SWAnimation()
{

}

SWAnimation::SWAnimation( factory_constructor )
{

}

SWAnimation::~SWAnimation()
{

}

void SWAnimation::onAwake()
{
	gameObject()->addUpdateDelegator( GetDelegator( onUpdate ) );
	m_flowTime = 0;
	m_current = NULL;
}

void SWAnimation::onRemove()
{
	gameObject()->removeUpdateDelegator( GetDelegator( onUpdate ) );
}

void SWAnimation::onUpdate()
{
	if ( m_current == NULL ) return;

	m_current->updatePlay( gameObject(), m_flowTime );
	m_flowTime += SWTime.getDeltaTime();
}

void SWAnimation::addClip( const tstring& name, SWAnimationClip* clip )
{
	m_clipTable[ name ] = clip;
}

void SWAnimation::play( const tstring& name )
{
	m_current = m_clipTable[ name ]();
}