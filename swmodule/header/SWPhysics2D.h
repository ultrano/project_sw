#ifndef SWPhysics_h__
#define SWPhysics_h__

#include "SWType.h"
#include "SWMemory.h"
#include "SWCollider2D.h"

#define SWPhysics (__SWPhysics2D::instance())

class __SWPhysics2D : public SWMemory
{

public:

	float getGravityForce() const { return m_gravityForce; }
	void setGravityForce( float force ) { m_gravityForce = force; }

	__SWPhysics2D();
	~__SWPhysics2D();
	static __SWPhysics2D& instance();
	

private:

	friend class SWCollider2D;
	friend class SWRigidBody2D;

	float m_gravityForce;
	SWObject::WList m_colliders;
	SWObject::WList m_bodies;
};

#endif // SWPhysics_h__