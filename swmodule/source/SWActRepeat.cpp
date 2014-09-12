#include "SWActRepeat.h"
#include "SWTime.h"
#include "SWMath.h"
#include "SWObjectStream.h"

SWActRepeat::SWActRepeat( SWAct* act, tuint count )
	: m_act( act )
	, m_repeatCount(0)
	, m_limitCount( count )
{
}

SWActRepeat::SWActRepeat( factory_constructor )
	: m_act( NULL )
	, m_repeatCount(0)
	, m_limitCount(0)
{
}

SWActRepeat::~SWActRepeat()
{
}

bool SWActRepeat::isDone()
{
	return ( m_limitCount > 0 )? ( m_repeatCount >= m_limitCount ) : false;
}

void SWActRepeat::onStart()
{
	if ( !m_act.isValid() ) return ;
	m_act()->setAction( getAction() );
	m_act()->onStart();
}

void SWActRepeat::onUpdate()
{
	if ( isDone() ) return;

	float delta = SWTime.getDeltaTime();
	m_act()->onUpdate();
	if ( m_act()->isDone() )
	{
		m_repeatCount += 1;
		m_act()->onStart();
		m_act()->onUpdate();
	}
}

void SWActRepeat::serialize( SWObjectWriter* writer )
{
	__super::serialize( writer );
	writer->writeObject( m_act() );
	writer->writeUInt( m_limitCount );
	writer->writeUInt( m_repeatCount );
}

void SWActRepeat::deserialize( SWObjectReader* reader )
{
	__super::deserialize( reader );
	m_act = swrtti_cast<SWAct>( reader->readObject() );
	m_limitCount = reader->readUInt();
	m_repeatCount = reader->readUInt();

}
