#ifndef SWPhysics_h__
#define SWPhysics_h__

#include "SWType.h"
#include "SWMemory.h"
#include "SWCollider2D.h"
#include "SWRigidBody2D.h"

#define SWPhysics2D (__SWPhysics2D::instance())

class __SWPhysics2D : public SWMemory
{

public:

	float getGravityForce() const { return m_gravityForce; }
	void setGravityForce( float force ) { m_gravityForce = force; }

	__SWPhysics2D();
	~__SWPhysics2D();
	static __SWPhysics2D& instance();
	
	void simulate();
	bool testCollide( SWCollider2D* collider1, SWCollider2D* collider2 );
private:

	friend class SWCollider2D;
	friend class SWRigidBody2D;

	float m_gravityForce;
	SWObject::WList m_colliders;
	SWObject::WList m_bodies;
};

#endif // SWPhysics_h__