#include "SWCollider2D.h"
#include "SWTransform.h"
#include "SWMath.h"
#include "SWPhysics2D.h"
#include "SWObjectStream.h"
#include "SWRigidBody2D.h"
#include "SWFixture2D.h"
#include "SWGameObject.h"
#include "SWBroadPhase2D.h"
#include "SWContact2D.h"

SWCollider2D::SWCollider2D( factory_constructor )
{

}

SWCollider2D::~SWCollider2D()
{

}

void SWCollider2D::onAwake()
{
	__super::onAwake();
	SWPhysics2D.m_colliders.push_back( this );
	m_broadPhase = SWPhysics2D.m_broadPhase();
	gameObject()->addFixedRateUpdateDelegator( GetDelegator( onFixedUpdate ) );
}

void SWCollider2D::onStart()
{
	updateTransform2D();
}

void SWCollider2D::onRemove()
{
	gameObject()->removeFixedRateUpdateDelegator( GetDelegator( onFixedUpdate ) );
	SWPhysics2D.m_colliders.remove( this );
	__super::onRemove();
}

void SWCollider2D::onFixedUpdate()
{
	updateTransform2D();

	FixtureList::iterator itor = m_fixtures.begin();
	for ( ; itor != m_fixtures.end() ; ++itor )
	{
		SWFixture2D* fixture = (*itor)();
		taabb2d aabb;
		fixture->getShape()->computeAABB( aabb, m_transform2D );
		m_broadPhase()->updateProxy( fixture->getProxyID(), aabb );
	}
}

void SWCollider2D::updateTransform2D()
{
	SWTransform* transform = getComponent<SWTransform>();
	const tmat44& world = transform->getWorldMatrix();
	tvec3 row1( world.m11, world.m12, world.m13 );
	tvec3 row2( world.m21, world.m22, world.m23 );
	m_transform2D.move = tvec2( world.m41, world.m42 );
	m_transform2D.rotate = SWMath.atan( row1.y, row1.x );
	m_transform2D.scale.x = row1.length();
	m_transform2D.scale.y = row2.length();
}

SWFixture2D* SWCollider2D::addCircle( const tvec2& center, float radius )
{
	SWFixture2D* fixture = new SWFixture2D(this);
	fixture->setCircle(center, radius);
	addFixture( fixture );
	return fixture;
}

SWFixture2D* SWCollider2D::addBox( const tvec2& center, float width, float height )
{
	SWFixture2D* fixture = new SWFixture2D(this);
	fixture->setBox(center, width, height);
	addFixture( fixture );
	return fixture;
}

SWFixture2D* SWCollider2D::addPolygon( const tarray<tvec2>& vertices )
{
	SWFixture2D* fixture = new SWFixture2D(this);
	fixture->setPolygon(vertices);
	addFixture( fixture );
	return fixture;
}

void SWCollider2D::addFixture( SWFixture2D* fixture )
{
	m_fixtures.push_back( fixture );
	
	taabb2d aabb;
	fixture->getShape()->computeAABB( aabb, m_transform2D );

	tuint proxyID = m_broadPhase()->createProxy( aabb, fixture );
	fixture->setProxyID( proxyID );
}

void SWCollider2D::removeFixture( SWFixture2D* fixture )
{
	m_broadPhase()->destroyProxy( fixture->getProxyID() );
	m_fixtures.remove( fixture );
}

void SWCollider2D::removeAllFixture()
{
	FixtureList::iterator itor = m_fixtures.begin();
	for ( ; itor != m_fixtures.end() ; ++itor )
	{
		SWFixture2D* fixture = (*itor)();
		m_broadPhase()->destroyProxy( fixture->getProxyID() );
	}
	m_fixtures.clear();
}

void SWCollider2D::addContact( const SWContact2D* contact )
{
	SWContactEdge2D* contactEdge = new SWContactEdge2D;
	contactEdge->next = m_contactEdge;
	contactEdge->contact = contact;

	SWCollider2D* collider1 = contact->fixture1()->getCollide();
	SWCollider2D* collider2 = contact->fixture2()->getCollide();
	if ( collider1 == this ) contactEdge->other = collider2;
	if ( collider2 == this ) contactEdge->other = collider1;

	if ( m_contactEdge() ) m_contactEdge()->prev = contactEdge;
	m_contactEdge = contactEdge;
}

void SWCollider2D::removeContact( const SWContact2D* contact )
{
	SWContactEdge2D* contactEdge = m_contactEdge();
	while ( contactEdge )
	{
		if ( contactEdge->contact() == contact )
		{
			SWContactEdge2D* next = contactEdge->next();
			SWContactEdge2D* prev = contactEdge->prev();
			if ( prev ) prev->next = next;
			if ( next ) next->prev = prev;
			if ( contactEdge == m_contactEdge() ) m_contactEdge = next;
			break;
		}
		contactEdge = contactEdge->next();
	}
}

const SWContactEdge2D* SWCollider2D::getContactEdge() const
{
	return m_contactEdge();
}
