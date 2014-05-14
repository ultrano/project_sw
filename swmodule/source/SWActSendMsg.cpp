#include "SWActSendMsg.h"
#include "SWGameObject.h"
#include "SWAction.h"
#include "SWObjectStream.h"

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

void SWActSendMsg::serialize( SWObjectWriter* ow )
{
	__super::serialize( ow );
	ow->writeString( m_eventName );
}

void SWActSendMsg::deserialize( SWObjectReader* or )
{
	__super::deserialize( or );
	or->readString( m_eventName );
}
