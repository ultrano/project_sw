#ifndef SWWorld2D_h__
#define SWWorld2D_h__

#include "SWRefCounter.h"
#include "SWType.h"

class SWBroadPhase2D;
class SWRefNode;
class SWFixture2D;
class SWContact2D;
class SWCollider2D;
class SWRigidBody2D;
class SWWorld2D : public SWRefCountable
{
	friend class SWRigidBody2D;
public:

	SWWorld2D();
	~SWWorld2D();

	SWContact2D* createContact( const SWFixture2D* fixture1, const SWFixture2D* fixture2, float friction, float bounciness );
	void removeContact( SWContact2D* contact );
	SWBroadPhase2D* getBroadPhase() const;
	void update( float step );
private:
	void addBody( SWRigidBody2D* body );
	void removeBody( SWRigidBody2D* body );
	bool existContact( const SWCollider2D* collider, const SWFixture2D* fixture1, const SWFixture2D* fixture2 );
	void findNewContacts();
	void updateContacts();
	void solveContacts();
private:
	SWHardRef<SWBroadPhase2D> m_broadPhase;
	SWHardRef<SWRefNode> m_contactList;
	SWHardRef<SWRefNode> m_bodyList;
	tuint m_contactCount;
	tuint m_bodyCount;
};

#endif // SWWorld2D_h__