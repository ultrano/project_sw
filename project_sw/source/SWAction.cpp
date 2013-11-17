#include "SWAction.h"
#include "SWGameObject.h"
#include "SWTime.h"
#include "SWAct.h"

void SWAction::onStart()
{
	__super::onStart();
	gameObject()->addUpdateDelegate( GetDelegate( onUpdate ) );
}

void SWAction::onRemove()
{
	gameObject()->removeUpdateDelegate( GetDelegate( onUpdate ) );
	__super::onRemove();
}

void SWAction::onUpdate( SWObject* param )
{
	SWAct* act = m_act();
	if ( act == NULL ) return;
	if ( act->isDone() ) return;
	if ( act->isPaused() ) return;
	m_spendTime += SWTime.getDeltaTime();
	act->onUpdate();
}

void SWAction::setAct( SWAct* act )
{
	if ( act ) act->m_action = this;
	m_act = act;
	act->onStart();
}

SWAct* SWAction::getAct() const
{
	return m_act();
}
