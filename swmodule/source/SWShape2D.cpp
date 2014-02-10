#include "SWShape2D.h"
#include "SWMath.h"

tvec2 SWCircle::getFarthest( const tvec2& direction )
{

	//! test circle
	{
		tvec2 center = m_center;
		float radius = m_radius;

		return ( m_center + ( direction.normal() * m_radius ) );
	}

	//! test box
	{
		float width;
		float height;
		tvec2 center;

		float halfW = width/2;
		float halfH = height/2;

		float left   = -halfW + center.x;
		float right  = +halfW + center.x;
		float top    = +halfH + center.y;
		float bottom = -halfH + center.y;

		float dist = 0;
		tvec2 pos( 0, 0 );
		float farDist = 0;
		tvec2 farPos( 0, 0 );
		
		pos  = tvec2( left, top );
		dist = direction.dot( farPos );
		
		farDist = dist;
		farPos  = pos;
		
		pos = tvec2( left, bottom );
		if ( farDist < ( dist = direction.dot( pos ) ) )
		{
			farDist = dist;
			farPos  = pos;
		}
		
		pos = tvec2( right, top );
		if ( farDist < ( dist = direction.dot( pos ) ) )
		{
			farDist = dist;
			farPos  = pos;
		}
		
		pos = tvec2( right, bottom );
		if ( farDist < ( dist = direction.dot( pos ) ) )
		{
			farDist = dist;
			farPos  = pos;
		}

		return farPos;
	}

	//! test polygon
	{
		tarray<tvec2> posArray;
		
		tvec2 farPos( posArray[0] );
		float farDist = direction.dot( farPos );

		for ( tuint i = 1 ; i < posArray.size() ; ++i )
		{
			const tvec2& pos = posArray[i];
			float dist = direction.dot( pos );
			if ( farDist < dist )
			{
				farPos  = pos;
				farDist = dist;
			}
		}

		return farPos;
	}
}