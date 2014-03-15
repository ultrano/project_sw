#ifndef SWRigidBody_h__
#define SWRigidBody_h__

#include "SWComponent.h"

class SWRigidBody : public SWComponent
{
	SW_RTTI( SWRigidBody, SWComponent );

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

#endif // SWRigidBody_h__