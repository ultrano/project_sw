#include "SWActSendMsg.h"
#include "SWGameObject.h"
#include "SWAction.h"
#include "SWObjectStream.h"

SWActSendMsg::SWActSendMsg( const tstring& eventName )
	: m_eventName( eventName )
{

}

SWActSendMsg::SWActSendMsg( factory_constructor )
	: m_eventName( "" )
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

void SWActSendMsg::serialize( SWObjectWriter* writer )
{
	__super::serialize( writer );
	writer->writeString( m_eventName );
}

void SWActSendMsg::deserialize( SWObjectReader* reader )
{
	__super::deserialize( reader );
	reader->readString( m_eventName );
}
