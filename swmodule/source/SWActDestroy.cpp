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

void SWActDestroy::serialize( SWObjectWriter* writer )
{
	__super::serialize( writer );
	writer->writeFloat( m_accum );
	writer->writeFloat( m_delay );
	writer->writeBool( m_destroied );

}

void SWActDestroy::deserialize( SWObjectReader* reader )
{
	__super::deserialize( reader );
	m_accum = reader->readFloat();
	m_delay = reader->readFloat();
	m_destroied = reader->readBool();
}
