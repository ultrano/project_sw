#include "SWPhysics2D.h"
#include "SWWorld2D.h"

__SWPhysics2D& __SWPhysics2D::instance()
{
	static __SWPhysics2D s_instance;

	return s_instance;
}


__SWPhysics2D::__SWPhysics2D()
	: m_gravityForce( 0.985f )
	, m_world( new SWWorld2D )
{
}

__SWPhysics2D::~__SWPhysics2D()
{
}

void __SWPhysics2D::simulate()
{
	for ( tuint i = 0 ; i < SW_MaxLayerCount ; ++i )
	{
		SWWorld2D* world = m_worlds[i]();
		if ( world )
		{
			world->update( getFixedInterval() );
		}
	}
}

float __SWPhysics2D::getFixedInterval()
{
	static const float interval = 1.0f/30.0f;
	return interval;
}

SWWorld2D* __SWPhysics2D::getWorld( tuint layer )
{
	if ( layer >= SW_MaxLayerCount ) return NULL;
	SWWorld2D* world = m_worlds[layer]();
	if ( world == NULL )
	{
		world = new SWWorld2D;
		m_worlds[layer] = world;
	}
	return world;
}
