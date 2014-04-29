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

void SWActPlay::onStart()
{
	if ( getAction() == NULL ) return ;
	if ( !m_sequence.isValid() ) return ;
	if ( m_duration <= 0 ) return ;
	m_renderer = getAction()->getComponent<SWMeshRenderer>();
	if ( !m_renderer.isValid() ) return ;
	m_accumulation = 0;
}

void SWActPlay::onUpdate()
{
	if ( !m_renderer.isValid() ) return ;
	float delta = SWTime.getDeltaTime();
	m_accumulation += SWTime.getDeltaTime();
	float rate = ( m_accumulation / m_duration );
	SWLog( "rate: %f", rate );
	if ( rate >= 1.0f ) return ;
	int index = m_sequence()->count() * rate;
	SWSpriteData::ImageRegion* region = NULL;
	region = swrtti_cast<SWSpriteData::ImageRegion>( m_sequence()->get( index ) );
	//m_renderer()->setTexture( region->texID );
}
