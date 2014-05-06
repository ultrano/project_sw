#include "SWActAnimate.h"
#include "SWAction.h"
#include "SWSpriteSequence.h"
#include "SWSpriteRenderer.h"
#include "SWTime.h"

SWActAnimate::SWActAnimate( float speed, SWSpriteSequence* sheet )
	: m_speed( speed )
	, m_sheet( sheet )
{

}

SWActAnimate::~SWActAnimate()
{

}

bool SWActAnimate::isDone()
{
	return (!m_sheet.isValid()) || (m_spendTime >= m_totalTime);
}

void SWActAnimate::onStart()
{
	if ( !m_sheet.isValid() ) return;

	m_totalTime = m_sheet()->getDelayPerUnit() * m_sheet()->count();
	m_spendTime = 0;
	m_lastIndex = 0;
	changeSpriteWithAt( m_lastIndex );
}

void SWActAnimate::onUpdate()
{
	m_spendTime += SWTime.getDeltaTime() * m_speed;
	if ( isDone() ) return;

	tuint index = (m_spendTime / m_sheet()->getDelayPerUnit());
	if ( index == m_lastIndex ) return;

	changeSpriteWithAt( index );
	m_lastIndex = index;
}

void SWActAnimate::changeSpriteWithAt( tuint index )
{
	if ( !m_sheet.isValid() ) return;

	SWSprite* sprite = m_sheet()->getSpriteAt( index );
	SWSpriteRenderer* renderer = getAction()->getComponent<SWSpriteRenderer>();
	renderer->setSprite( sprite );
}
