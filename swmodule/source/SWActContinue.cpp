#include "SWActContinue.h"
#include "SWAction.h"
#include "SWTime.h"

SWActContinue::SWActContinue( SWAct* act )
	: m_act( act )
{

}

SWActContinue::~SWActContinue()
{

}

bool SWActContinue::isDone()
{
	return false;
}

void SWActContinue::onStart()
{
	if ( !m_act.isValid() ) return ;
	m_act()->onStart();
}

void SWActContinue::onUpdate()
{
	if ( m_act()->isDone() ) m_act()->onStart();
	m_act()->onUpdate();
}
