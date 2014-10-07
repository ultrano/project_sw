#ifndef SWWorld2D_h__
#define SWWorld2D_h__

#include "SWRefCounter.h"

class SWBroadPhase2D;
class SWRefNode;
class SWFixture2D;
class SWContact2D;
class SWWorld2D : public SWRefCountable
{
public:

	SWWorld2D();
	~SWWorld2D();

	SWContact2D* createContact( const SWFixture2D* fixture1, const SWFixture2D* fixture2, float friction, float bounciness );
	void removeContact( SWContact2D* contact );
	SWBroadPhase2D* getBroadPhase() const;
	void update( float step );
private:
	void findNewContacts();
	void updateContacts();
private:
	SWHardRef<SWBroadPhase2D> m_broadPhase;
	SWHardRef<SWRefNode> m_contactList;
};

#endif // SWWorld2D_h__