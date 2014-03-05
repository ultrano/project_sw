#include "SWAnimationClip.h"
#include "SWGameObject.h"
#include "SWComponent.h"

SWAnimationClip::AnimInfo::AnimInfo( const SWRtti* _type, const thashstr& _key, SWAnimationLine* _line )
	: type( _type ), key( _key ), line( _line ) {};

void SWAnimationClip::addLine( const SWRtti* type, const tstring& key, SWAnimationLine* line )
{
	m_animInfos.push_back( AnimInfo( type, key, line ) );
}

void SWAnimationClip::addLine( const SWRtti* type, const tstring& key, SWAnimationLine::Ref line )
{
	addLine( type, key, line() );
}

void SWAnimationClip::updatePlay( SWGameObject* go, float time )
{
	for ( int i = 0 ; i < m_animInfos.size() ; ++i )
	{
		const AnimInfo& info = m_animInfos[i];
		SWComponent* comp = go->getComponent( info.type );
		if ( comp == NULL ) continue;

		comp->onAnimate( info.key, info.line()->evaluate( time ) );
	}
}
