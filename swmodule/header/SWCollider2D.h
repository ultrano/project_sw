#ifndef SWCollider2D_h__
#define SWCollider2D_h__

#include "SWComponent.h"
#include "SWShape2D.h"

class SWFixture2D;
class SWBroadPhase2D;
class SWContact2D;
class SWContactEdge2D;
class SWCollider2D : public SWComponent
{
	SW_RTTI( SWCollider2D, SWComponent );
public:

	SWCollider2D( factory_constructor );
	~SWCollider2D();

	SWFixture2D* addCircle( const tvec2& center, float radius );
	SWFixture2D* addBox( const tvec2& center, float width, float height );
	SWFixture2D* addPolygon( const tarray<tvec2>& vertices );

	void removeFixture( SWFixture2D* fixture );
	void removeAllFixtures();

	void getTransform2D( SWShape2D::Transform& transform2D ) const;
 
	void addContactEdge( const SWContact2D* contact );
	void removeContactEdge( const SWContact2D* contact );
	void removeAllContactEdges();
	const SWContactEdge2D* getContactEdge() const;

protected:

	void onAwake();
	void onStart();
	void onRemove();
	void onFixedUpdate();

private:

	void addFixture( SWFixture2D* fixture );

private:
	typedef tlist<SWHardRef<SWFixture2D>> FixtureList;

	FixtureList m_fixtures;
	SWShape2D::Transform m_transform2D;
	SWWeakRef<SWBroadPhase2D> m_broadPhase;
	SWHardRef<SWContactEdge2D> m_contactEdge;
};

class SWCollision2D : public SWObject
{
	SW_RTTI( SWCollision2D, SWObject );

public:

	SWCollision2D() : collider( NULL ) {};
	SWCollision2D( SWCollider2D* _collider ) : collider( _collider ) {};
	
	SWWeakRef<SWCollider2D> collider;
};

#endif // SWCollider2D_h__