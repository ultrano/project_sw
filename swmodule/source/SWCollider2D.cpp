#include "SWCollider2D.h"
#include "SWTransform.h"
#include "SWMath.h"
#include "SWPhysics2D.h"
#include "SWObjectStream.h"
#include "SWFixture2D.h"
#include "SWGameObject.h"
#include "SWBroadPhase2D.h"
#include "SWContact2D.h"
#include "SWWorld2D.h"

SWCollider2D::SWCollider2D( factory_constructor )
{

}

SWCollider2D::~SWCollider2D()
{

}

void SWCollider2D::onAwake()
{
	__super::onAwake();

	m_world = SWPhysics2D.getWorld( gameObject()->getLayer() );
	gameObject()->addLayerDelegator( GetDelegator(onLayerChanged) );
	gameObject()->addFixedRateUpdateDelegator( GetDelegator( onFixedUpdate ) );
}

void SWCollider2D::onRemove()
{
	clearContactEdges( m_world() );
	removeAllFixtures();
	gameObject()->removeFixedRateUpdateDelegator( GetDelegator( onFixedUpdate ) );
	gameObject()->removeLayerDelegator( GetDelegator(onLayerChanged) );
	__super::onRemove();
}

void SWCollider2D::onFixedUpdate()
{
	SWShape2D::Transform trans2D;
	getTransform2D( trans2D );

	SWBroadPhase2D* broadPhase = m_world()->getBroadPhase();
	FixtureList::iterator itor = m_fixtures.begin();
	for ( ; itor != m_fixtures.end() ; ++itor )
	{
		SWFixture2D* fixture = (*itor)();
		taabb2d aabb;
		fixture->getShape()->computeAABB( aabb, trans2D );
		broadPhase->updateProxy( fixture->getProxyID(), aabb );
	}
}

void SWCollider2D::onLayerChanged()
{
	SWWorld2D* oldWorld = m_world();
	SWWorld2D* newWorld = SWPhysics2D.getWorld( gameObject()->getLayer() );
	
	SWBroadPhase2D* oldBroadPhase = oldWorld->getBroadPhase();
	SWBroadPhase2D* newBroadPhase = newWorld->getBroadPhase();

	//! remove all contacts and contact-edges from old world
	clearContactEdges( oldWorld );

	//! remove fixtures from old broad-phase
	{
		for ( FixtureList::iterator itor = m_fixtures.begin() 
			; itor != m_fixtures.end() ; ++itor )
		{
			SWFixture2D* fixture = (*itor)();
			oldBroadPhase->destroyProxy( fixture->getProxyID() );
		}
	}

	//! add fixtures to new broad-phase
	{
		taabb2d aabb;
		SWShape2D::Transform trans2D;
		getTransform2D( trans2D );
		for ( FixtureList::iterator itor = m_fixtures.begin() 
			; itor != m_fixtures.end() ; ++itor )
		{
			SWFixture2D* fixture = (*itor)();
			fixture->getShape()->computeAABB( aabb, trans2D );

			tuint proxyID = newBroadPhase->createProxy( aabb, fixture );
			fixture->setProxyID( proxyID );
		}
	}
}

SWFixture2D* SWCollider2D::addCircle( const tvec2& center, float radius )
{
	SWFixture2D* fixture = new SWFixture2D(this);
	fixture->setCircle(center, radius);
	registerFixture( fixture );
	return fixture;
}

SWFixture2D* SWCollider2D::addBox( const tvec2& center, float width, float height )
{
	SWFixture2D* fixture = new SWFixture2D(this);
	fixture->setBox(center, width, height);
	registerFixture( fixture );
	return fixture;
}

SWFixture2D* SWCollider2D::addPolygon( const tarray<tvec2>& vertices )
{
	SWFixture2D* fixture = new SWFixture2D(this);
	fixture->setPolygon(vertices);
	registerFixture( fixture );
	return fixture;
}

void SWCollider2D::registerFixture( SWFixture2D* fixture )
{
	m_fixtures.push_back( fixture );

	SWShape2D::Transform trans2D;
	taabb2d aabb;
	getTransform2D( trans2D );
	fixture->getShape()->computeAABB( aabb, trans2D );

	SWBroadPhase2D* broadPhase = m_world()->getBroadPhase();
	tuint proxyID = broadPhase->createProxy( aabb, fixture );
	fixture->setProxyID( proxyID );
}

void SWCollider2D::removeFixture( SWFixture2D* fixture )
{
	SWBroadPhase2D* broadPhase = m_world()->getBroadPhase();
	broadPhase->destroyProxy( fixture->getProxyID() );
	m_fixtures.remove( fixture );
}

void SWCollider2D::removeAllFixtures()
{
	SWBroadPhase2D* broadPhase = m_world()->getBroadPhase();
	FixtureList::iterator itor = m_fixtures.begin();
	for ( ; itor != m_fixtures.end() ; ++itor )
	{
		SWFixture2D* fixture = (*itor)();
		broadPhase->destroyProxy( fixture->getProxyID() );
	}
	m_fixtures.clear();
}

void SWCollider2D::getTransform2D( SWShape2D::Transform& transform2D ) const
{
	SWTransform* transform = getComponent<SWTransform>();
	const tmat44& world = transform->getWorldMatrix();
	tvec3 row1( world.m11, world.m12, world.m13 );
	tvec3 row2( world.m21, world.m22, world.m23 );
	transform2D.move = tvec2( world.m41, world.m42 );
	transform2D.rotate = SWMath.atan( row1.y, row1.x );
	transform2D.scale.x = row1.xy().length();
	transform2D.scale.y = row2.xy().length();
}

void SWCollider2D::addContactEdge( const SWContact2D* contact )
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

void SWCollider2D::removeContactEdge( const SWContact2D* contact )
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

void SWCollider2D::clearContactEdges( SWWorld2D* world )
{
	SWHardRef<SWContactEdge2D> ce = m_contactEdge();
	while ( ce() )
	{
		SWHardRef<SWContactEdge2D> ce0 = ce();
		ce = ce()->next();
		world->removeContact( ce0()->contact() );
	}
	m_contactEdge = NULL;
}

const SWContactEdge2D* SWCollider2D::getContactEdge() const
{
	return m_contactEdge();
}
