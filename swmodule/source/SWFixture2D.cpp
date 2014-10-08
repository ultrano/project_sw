#include "SWFixture2D.h"
#include "SWShape2D.h"
#include "SWCollider2D.h"

SWFixture2D::SWFixture2D( SWCollider2D* collide )
	: m_friction(0)
	, m_bounciness(1)
	, m_collide( collide )
{

}

SWFixture2D::~SWFixture2D()
{

}

void SWFixture2D::setCircle( const tvec2& center, float radius )
{
	SWCircleShape2D* shape = new SWCircleShape2D();
	shape->set( center, radius );
	m_shape = shape;
}

void SWFixture2D::setBox( const tvec2& center, float width, float height )
{
	SWPolygonShape2D* shape = new SWPolygonShape2D;
	shape->setBox(center, width, height);
	m_shape = shape;
}

void SWFixture2D::setPolygon( const tarray<tvec2>& vertices )
{
	SWPolygonShape2D* shape = new SWPolygonShape2D;
	shape->set( vertices );
	m_shape = shape;
}

const SWShape2D* SWFixture2D::getShape() const
{
	return m_shape();
}

SWCollider2D* SWFixture2D::getCollide() const
{
	return m_collide();
}

bool testFixture2D( SWManifold& manifold, const SWFixture2D* fixture1, const SWFixture2D* fixture2 )
{
	SWShapeTransform2D transform1, transform2;
	fixture1->getCollide()->getTransform2D( transform1 );
	fixture2->getCollide()->getTransform2D( transform2 );
	const SWShape2D* shape1 = fixture1->getShape();
	const SWShape2D* shape2 = fixture2->getShape();

	return testShape2D( manifold, shape1, transform1, shape2, transform2 );
}
