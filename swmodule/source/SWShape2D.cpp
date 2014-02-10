#include "SWShape2D.h"


tvec2 SWCircle::getFarthest( const tvec2& direction )
{

	tvec2 center = m_center;
	float radius = m_radius;


	return ( m_center + ( direction.normal() * m_radius ) );
}