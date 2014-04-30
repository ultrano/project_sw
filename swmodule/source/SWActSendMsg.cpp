#include "SWActSendMsg.h"
#include "SWGameObject.h"
#include "SWAction.h"

SWActSendMsg::SWActSendMsg( const tstring& eventName )
	: m_eventName( eventName )
{

}

SWActSendMsg::~SWActSendMsg()
{

}

bool SWActSendMsg::isDone()
{
	return true;
}

void SWActSendMsg::onStart()
{
	SWAction* action = getAction();
	if ( action == NULL ) return;

	SWGameObject* go = action->gameObject();
	if ( go == NULL ) return;

	go->sendMessage( m_eventName, NULL );
}

void SWActSendMsg::onUpdate()
{
}