#ifndef SWCollider2D_h__
#define SWCollider2D_h__

#include "SWComponent.h"
#include "SWShape2D.h"

class SWFixture2D;
class SWWorld2D;
class SWContact2D;
class SWContactEdge2D;
class SWCollider2D : public SWComponent
{
	SW_RTTI( SWCollider2D, SWComponent );
	friend class SWWorld2D;
public:

	SWCollider2D( factory_constructor );
	~SWCollider2D();

	SWFixture2D* addCircle( const tvec2& center, float radius );
	SWFixture2D* addBox( const tvec2& center, float width, float height );
	SWFixture2D* addPolygon( const tarray<tvec2>& vertices );

	void removeFixture( SWFixture2D* fixture );
	void removeAllFixtures();

	void computeMatrix2D( tmat33& mat ) const;
	const tvec2& getCenterOfMass() const;

private:
	void addContactEdge( const SWContact2D* contact );
	void removeContactEdge( const SWContact2D* contact );
	const SWContactEdge2D* getContactEdge() const;
	void clearContactEdges( SWWorld2D* world );
protected:

	void onAwake();
	void onRemove();
	void onFixedUpdate();
	void onLayerChanged();

private:

	void registerFixture( SWFixture2D* fixture );

private:
	typedef tlist<SWHardRef<SWFixture2D>> FixtureList;

	FixtureList m_fixtures;
	SWHardRef<SWContactEdge2D> m_contactEdge;
	SWWeakRef<SWWorld2D> m_world;

	SWMassData m_localMass;
	tvec2 m_centerOfMass;

	enum { eUpdateMass };
	tflag8 m_flags;
};

#endif // SWCollider2D_h__