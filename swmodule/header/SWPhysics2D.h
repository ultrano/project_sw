#ifndef SWPhysics_h__
#define SWPhysics_h__

#include "SWType.h"
#include "SWMemory.h"
#include "SWCollider2D.h"
#include "SWRigidBody2D.h"
#include "SWDynamicTree2D.h"

#define SWPhysics2D (__SWPhysics2D::instance())

class SWBroadPhase2D;
class SWContact2D;
class SWRefNode;
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

public:

	SWContact2D* createContact();
	void removeContact( SWContact2D* contact );
	void findNewContacts();
	void updateContacts();
public:

	SWHardRef<SWBroadPhase2D> m_broadPhase;
	SWHardRef<SWContact2D> m_contactHead;
	SWHardRef<SWRefNode> m_contactList;

private:

	typedef ttable<thash64,bool> IgnoreTable;
	friend class SWCollider2D;
	friend class SWRigidBody2D;

	float m_gravityForce;
	SWObject::WList m_colliders;
	SWObject::WList m_bodies;
	IgnoreTable m_ignoreTable;
	SWDynamicTree2D m_tree;
	
	SWHardRef<SWCollision2D> m_coll;
};

#endif // SWPhysics_h__