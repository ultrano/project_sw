#include "SWPhysics2D.h"


__SWPhysics2D& __SWPhysics2D::instance()
{
	static __SWPhysics2D s_instance;

	return s_instance;
}


__SWPhysics2D::__SWPhysics2D()
	: m_gravityForce( 9.85f )
{
}

__SWPhysics2D::~__SWPhysics2D()
{
}