#include "SWActDestroy.h"
#include "SWAction.h"
#include "SWGameObject.h"
#include "SWTime.h"
#include "SWObjectStream.h"

SWActDestroy::SWActDestroy( float delay /*= 0 */ )
	: m_delay( delay )
	, m_destroied( false )
{

}

SWActDestroy::~SWActDestroy()
{

}


bool SWActDestroy::isDone()
{
	return m_destroied;
}

void SWActDestroy::onStart()
{
	m_destroied = false;
	m_accum = m_delay;
}

void SWActDestroy::onUpdate()
{
	if ( ( m_accum -= SWTime.getDeltaTime() ) > 0 ) return;

	float delta = SWTime.getDeltaTime();
	SWAction* action = getAction();
	if ( !action ) return;
	SWGameObject* go = action->gameObject();
	if ( !go ) return;
	m_destroied = true;
	go->destroy();
}

void SWActDestroy::serialize( SWObjectWriter* ow )
{
	__super::serialize( ow );
	ow->writeFloat( m_accum );
	ow->writeFloat( m_delay );
	ow->writeBool( m_destroied );

}

void SWActDestroy::deserialize( SWObjectReader* or )
{
	__super::deserialize( or );
	m_accum = or->readFloat();
	m_delay = or->readFloat();
	m_destroied = or->readBool();
}
