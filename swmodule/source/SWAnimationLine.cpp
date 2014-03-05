#include "SWAnimationLine.h"
#include <algorithm>

SWAnimationLine::SWAnimationLine()
{

}

SWAnimationLine::~SWAnimationLine()
{

}

int SWAnimationLine::addKey( float time, float value )
{
	//! is there same one.
	for ( int i = 0 ; i < m_keyFrames.size() ; ++i)
	{
		if ( m_keyFrames[i].time == time ) return -1;
	}

	//! add key
	int index = m_keyFrames.size();
	m_keyFrames.push_back( KeyFrame( time, value ) );

	//! align key by time
	AlignedIndices::iterator itor = m_aligned.begin();
	for ( ; itor != m_aligned.end() ; ++itor )
	{
		const KeyFrame& key = m_keyFrames[ *itor ];
		if ( time < key.time ) break;
	}
	m_aligned.insert( itor, index );

	return index;
}

void SWAnimationLine::removeKey( int index )
{
	int count = m_keyFrames.size();
	if ( index < 0 || index >= count ) return;
	
	while ( ++index < count ) m_keyFrames[ index-1 ] = m_keyFrames[ index ];

	m_keyFrames.resize( count-1 );
	std::remove( m_aligned.begin(), m_aligned.end(), index );
}

float SWAnimationLine::evaluate( float time )
{
	if ( m_aligned.size() == 0 ) return 0;

	//! search a round of time
	const KeyFrame* last = &(m_keyFrames[ m_aligned[0] ]);
	for ( AlignedIndices::iterator itor = m_aligned.begin()
		; itor != m_aligned.end() 
		; ++itor )
	{
		const KeyFrame& key = m_keyFrames[ *itor ];
		if ( time == key.time ) return key.value;
		if ( time < key.time )
		{
			float rate = (key.time - last->time);
			if ( rate == 0 ) return key.value;
			
			rate = (time - last->time)/rate;
			return last->value + ( (key.value - last->value) * rate );
		}
		last = &key;
	}
	return last->value;
}


SWAnimationLine::Ref SWAnimationLine::Linear( float beginTime, float endTime, float beginVal, float endVal )
{
	SWAnimationLine::Ref ret = new SWAnimationLine;
	ret()->addKey( beginTime, beginVal );
	ret()->addKey( endTime, endVal );
	return ret;
}