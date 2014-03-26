#include "SWCollider2D.h"
#include "SWTransform.h"

bool SWCircleCollider2D::containPoint( const tvec2 point )
{
	SWTransform* transform = getComponent<SWTransform>();
	tvec3 center = tvec3(m_center, 0)* transform->getWorldMatrix();
	return ( (center.xy()- point).length() <= m_radius );
}