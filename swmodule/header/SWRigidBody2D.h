#ifndef SWRigidBody2D_h__
#define SWRigidBody2D_h__

#include "SWComponent.h"

class SWRigidBody2D : public SWComponent
{
	SW_RTTI( SWRigidBody2D, SWComponent );

private:

	tufloat m_mass;
	tvec2   m_velocity;
	tfloat  m_drag;
	tfloat  m_elastic;
	tvec2   m_gravityFactor;

	void onStart();
	void onRemove();
	void onUpdate();

public:

	void addForce( const tvec2& force );
	void addAccel( const tvec2& accel );
	void setDrag( float drag );
	void setGravityFactor( const tvec2& factor );
	void setElastic( float elastic );
	void setMass( tufloat mass );

};

#endif // SWRigidBody2D_h__