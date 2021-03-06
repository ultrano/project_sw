#ifndef SWPhysics_h__
#define SWPhysics_h__

#include "SWType.h"
#include "SWMemory.h"
#include "SWObject.h"
#include "SWDefines.h"

#define SWPhysics2D (__SWPhysics2D::instance())

class SWBroadPhase2D;
class SWContact2D;
class SWRefNode;
class SWWorld2D;
class __SWPhysics2D : public SWMemory
{
public:

	float getGravityForce() const { return m_gravityForce; }
	void setGravityForce( float force ) { m_gravityForce = force; }

	__SWPhysics2D();
	~__SWPhysics2D();
	static __SWPhysics2D& instance();
	
	void simulate();

	float getFixedInterval();

	SWWorld2D* getWorld( tuint layer );
public:

	SWHardRef<SWWorld2D> m_world;
	SWHardRef<SWWorld2D> m_worlds[SW_MaxLayerCount];

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