#ifndef SWRigidBody2D_h__
#define SWRigidBody2D_h__

#include "SWComponent.h"

class SWRigidBody2D : public SWComponent
{
	SW_RTTI( SWRigidBody2D, SWComponent );

private:

	tvec2   m_center;
	float   m_angle;
	
	tvec2   m_velocity;
	tfloat  m_torque;

	tufloat m_mass;
	tufloat m_inertia;

	tfloat  m_linearDrag;
	tfloat  m_angularDrag;

	tfloat  m_elastic;
	tvec2   m_gravityScale;

	void onStart();
	void onRemove();
	void onUpdate();

public:
	
	SWRigidBody2D();
	~SWRigidBody2D();
	void addForce( const tvec2& force );
	void addForce( const tvec2& force, const tvec2& pos );
	void addAccel( const tvec2& accel );
	void setLinearDrag( float drag );
	void setGravityScale( const tvec2& scale );
	void setElastic( float elastic );
	void setMass( tufloat mass );
	void setInertia( tufloat inertia );

};

#endif // SWRigidBody2D_h__