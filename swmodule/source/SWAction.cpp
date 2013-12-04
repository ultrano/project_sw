#include "SWAction.h"
#include "SWList.h"
#include "SWGameObject.h"
#include "SWTime.h"
#include "SWAct.h"

SWAction::SWAction()
	: m_actList( new SWList )
{

}

SWAction::~SWAction()
{

}

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

void SWAction::onUpdate()
{
	SWWeakRef<SWAction> vital = this;
	SWList::Value itorList;
	m_actList()->copy( itorList );
	SWList::iterator itor = itorList.begin();
	for ( ; itor != itorList.end() ; ++itor )
	{
		if ( vital.isValid() == false ) break;
		SWAct* act = swrtti_cast<SWAct>( (*itor)() );
		if ( act == NULL ) break;
		if ( act->isPaused() ) continue;
		if ( act->isDone() )
		{
			stopAct( act );
			continue;
		}
		act->onUpdate();
	}
}

void SWAction::runAct( SWAct* act )
{
	if ( act == NULL ) return;

	SWHardRef<SWAct> hold = act;

	if ( act ) act->m_action = this;
	if ( !act->onStart() ) return;
	m_actList()->add( act );
}

void SWAction::stopAct( SWAct* act )
{
	if ( act == NULL ) return;
	m_actList()->remove( act );
}