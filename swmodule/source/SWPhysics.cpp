#include "SWPhysics.h"


__SWPhysics& __SWPhysics::instance()
{
	static __SWPhysics s_instance;

	return s_instance;
}


__SWPhysics::__SWPhysics()
	: m_gravityForce( 9.85f )
{
}

__SWPhysics::~__SWPhysics()
{
}