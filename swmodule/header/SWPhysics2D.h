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
	bool getIgnoreLayer( const thashstr& layer1, const thashstr& layer2 );
	void ignoreLayer( const thashstr& layer1, const thashstr& layer2, bool ignore );
	bool testCollide( SWCollider2D* collider1, SWCollider2D* collider2 );
private:

	typedef ttable<thash64,bool> IgnoreTable;
	friend class SWCollider2D;
	friend class SWRigidBody2D;

	float m_gravityForce;
	SWObject::WList m_colliders;
	SWObject::WList m_bodies;
	IgnoreTable m_ignoreTable;
};

#endif // SWPhysics_h__