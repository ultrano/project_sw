#include "SWActContinue.h"
#include "SWAction.h"

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

bool SWActContinue::onStart()
{
	if ( !m_act.isValid() ) return false;
	getAction()->runAct( m_act() );
	return true;
}

void SWActContinue::onUpdate()
{
	if ( m_act()->isDone() ) getAction()->runAct( m_act() );
}
