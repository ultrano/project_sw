#ifndef SWFixture2D_h__
#define SWFixture2D_h__

#include "SWRefCounter.h"
#include "SWType.h"

class SWShape2D;
class SWManifold;
class SWCollider2D;
class SWFixture2D : public SWRefCountable
{
public:

	SWFixture2D( SWCollider2D* collide );
	~SWFixture2D();

	void setMaterial( float friction, float bounciness ) { m_friction = friction; m_bounciness = bounciness; };
	void setFriction( float friction ) { m_friction = friction; }
	void setBounciness( float bounciness ) { m_bounciness = bounciness; }

	float getFriction() const { return m_friction; }
	float getBounciness() const { return m_bounciness; }

	void setCircle( const tvec2& center, float radius );
	void setBox( const tvec2& center, float width, float height );
	void setPolygon( const tarray<tvec2>& vertices );

	const SWShape2D* getShape() const;
	SWCollider2D* getCollide() const;

	void setProxyID( tuint proxyID ) { m_proxyID = proxyID; }
	tuint getProxyID() const { return m_proxyID; }

public:
	float m_friction;
	float m_bounciness;
	tuint m_proxyID;
	SWHardRef<SWShape2D> m_shape;
	SWWeakRef<SWCollider2D> m_collide;
};

bool testFixture2D( SWManifold& manifold, const SWFixture2D* fixture1, const SWFixture2D* fixture2 );

#endif // SWFixture2D_h__