#ifndef SWRigidBody2D_h__
#define SWRigidBody2D_h__

#include "SWComponent.h"

class SWRigidBody2D : public SWComponent
{
	SW_RTTI( SWRigidBody2D, SWComponent );

private:

	float m_mass;
	tvec3 m_velocity;
	float m_drag;
	float m_elastic;

	void onStart();
	void onRemove();
	void onUpdate();

public:

	void addForce( const tvec3& force );

};

#endif // SWRigidBody2D_h__