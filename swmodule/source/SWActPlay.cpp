#include "SWActPlay.h"
#include "SWAction.h"
#include "SWMesh.h"
#include "SWMeshFilter.h"
#include "SWMeshRenderer.h"
#include "SWGameObject.h"
#include "SWTime.h"
#include "SWLog.h"
#include "SWSpriteData.h"

SWActPlay::SWActPlay( SWArray* sequence, float duration )
	: m_sequence( sequence )
	, m_duration( duration )
{

}

SWActPlay::~SWActPlay()
{

}

bool SWActPlay::isDone()
{
	return ( ( m_accumulation / m_duration ) >= 1.0f );
}

bool SWActPlay::onStart()
{
	if ( getAction() == NULL ) return false;
	if ( !m_sequence.isValid() ) return false;
	if ( m_duration <= 0 ) return false;
	m_renderer = getAction()->getComponent<SWMeshRenderer>();
	if ( !m_renderer.isValid() ) return false;
	m_accumulation = 0;
	return true;
}

void SWActPlay::onUpdate( float delta )
{
	if ( !m_renderer.isValid() ) return ;
	m_accumulation += SWTime.getDeltaTime();
	float rate = ( m_accumulation / m_duration );
	SWLog( "rate: %f", rate );
	if ( rate >= 1.0f ) return ;
	int index = m_sequence()->count() * rate;
	SWSpriteData::ImageRegion* region = NULL;
	region = swrtti_cast<SWSpriteData::ImageRegion>( m_sequence()->get( index ) );
	//m_renderer()->setTexture( region->texID );
}
