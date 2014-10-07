#include "SWPhysics2D.h"
#include "SWTransform.h"
#include "SWGameObject.h"
#include "SWRefNode.h"
#include "SWMath.h"
#include "SWLog.h"
#include "SWBroadPhase2D.h"
#include "SWFixture2D.h"
#include "SWContact2D.h"

__SWPhysics2D& __SWPhysics2D::instance()
{
	static __SWPhysics2D s_instance;

	return s_instance;
}


__SWPhysics2D::__SWPhysics2D()
	: m_gravityForce( 0.985f )
	, m_broadPhase( new SWBroadPhase2D )
{
}

__SWPhysics2D::~__SWPhysics2D()
{
}

void __SWPhysics2D::simulate()
{
	findNewContacts();
	updateContacts();
}

SWContact2D* __SWPhysics2D::createContact()
{
	SWContact2D* contact = new SWContact2D;
	SWRefNode* node = new SWRefNode;
	node->ref = contact;
	node->prev = NULL;
	node->next = m_contactList();
	contact->node = node;
	if ( m_contactList() ) m_contactList()->prev = node;
	m_contactList = node;
	return contact;
}

void __SWPhysics2D::removeContact( SWContact2D* contact )
{
	if ( !contact ) return;
	if ( SWFixture2D* fixture = contact->fixture1() )
	{
		SWCollider2D* collider = fixture->getCollide();
		if ( collider ) collider->removeContactEdge( contact );
	}
	if ( SWFixture2D* fixture = contact->fixture2() )
	{
		SWCollider2D* collider = fixture->getCollide();
		if ( collider ) collider->removeContactEdge( contact );
	}

	//! disconnect each other
	SWRefNode* node = contact->node();
	node->ref = NULL;
	contact->node = NULL;
}

bool existContact( const SWCollider2D* collider, const SWFixture2D* fixture1, const SWFixture2D* fixture2 )
{
	const SWContactEdge2D* contactEdge = collider->getContactEdge();
	while ( contactEdge )
	{
		SWFixture2D* f1 = contactEdge->contact()->fixture1();
		SWFixture2D* f2 = contactEdge->contact()->fixture2();
		if ( f1 == fixture1 && f2 == fixture2 ) return true;
		if ( f1 == fixture2 && f2 == fixture1 ) return true;
		contactEdge = contactEdge->next();
	}
	return false;
}

void __SWPhysics2D::findNewContacts()
{
	SWBroadPhase2D* broadPhase = m_broadPhase();
	SWBroadPhase2D::OverlapResult result;
	broadPhase->queryNewOverlap( result );

	SWBroadPhase2D::OverlapResult::iterator itor = result.begin();
	for ( ; itor != result.end() ; ++itor )
	{
		const SWBroadPhase2D::OverlapPair& overlapPair = (*itor);

		SWFixture2D* fixture1 = (SWFixture2D*)broadPhase->getUserData( overlapPair.first );
		SWFixture2D* fixture2 = (SWFixture2D*)broadPhase->getUserData( overlapPair.second );

		SWCollider2D* collider1 = fixture1->getCollide();
		SWCollider2D* collider2 = fixture2->getCollide();
		if ( collider1 == collider2 ) continue;
		if ( existContact( collider1, fixture1, fixture2 ) ) continue;
		if ( existContact( collider2, fixture1, fixture2 ) ) continue;

		SWRigidBody2D* body1 = collider1->getComponent<SWRigidBody2D>();
		SWRigidBody2D* body2 = collider2->getComponent<SWRigidBody2D>();
		//if ( body1 == NULL && body2 == NULL ) continue;

		SWContact2D* contact = createContact();
		contact->fixture1 = fixture1;
		contact->fixture2 = fixture2;
		contact->friction = SWMath.sqrt( fixture1->getFriction() * fixture2->getFriction() );
		contact->bounciness = (fixture1->getBounciness() + fixture2->getBounciness()) * 0.5f;

		if ( collider1 ) collider1->addContactEdge( contact );
		if ( collider2 ) collider2->addContactEdge( contact );
	}
}

void __SWPhysics2D::updateContacts()
{
	static thashstr onCollisionEnter = "onCollisionEnter";
	static thashstr onCollisionStay  = "onCollisionStay";
	static thashstr onCollisionLeave = "onCollisionLeave";

	//! update contacts
	{
		SWRefNode* node = m_contactList();
		while ( node )
		{
			SWContact2D* contact = (SWContact2D*)node->ref();
			node = node->next();
			contact->update();
		}
	}

	//! sending message
	{
		SWRefNode* node = m_contactList();
		while ( node )
		{
			SWContact2D* contact = (SWContact2D*)node->ref();

			SWCollider2D* collider1 = contact->fixture1()->getCollide();
			SWCollider2D* collider2 = contact->fixture2()->getCollide();
			if ( contact->state.get( SWContact2D::eEntering ) )
			{
				collider1->gameObject()->sendMessage( onCollisionEnter, NULL );
				collider2->gameObject()->sendMessage( onCollisionEnter, NULL );
			}
			else if ( contact->state.get( SWContact2D::eStaying ) )
			{
				collider1->gameObject()->sendMessage( onCollisionStay, NULL );
				collider2->gameObject()->sendMessage( onCollisionStay, NULL );
			}
			else if ( contact->state.get( SWContact2D::eLeaving ) )
			{
				collider1->gameObject()->sendMessage( onCollisionLeave, NULL );
				collider2->gameObject()->sendMessage( onCollisionLeave, NULL );
			}

			//! make the iteration safe from the removing during iterate (lazy delete node)
			if ( node->ref.isValid() ) node = node->next();
			else while ( node && !node->ref.isValid() )
			{
				SWRefNode* next = node->next();
				SWRefNode* prev = node->prev();
				if ( next ) next->prev = prev;
				if ( prev ) prev->next = next;
				if ( m_contactList() == node ) m_contactList = next;
				node = next;
			}
		}
	}

	//! remove contacts that are out of bound
	{
		SWRefNode* node = m_contactList();
		while ( node )
		{
			SWContact2D* contact = (SWContact2D*)node->ref();
			node = node->next();

			tuint proxyID1 = contact->fixture1()->getProxyID();
			tuint proxyID2 = contact->fixture2()->getProxyID();

			if ( !m_broadPhase()->testOverlap( proxyID1, proxyID2 ) )
			{
				SWContact2D* next = contact->next();
				removeContact( contact );
				contact = next;
				continue;
			}
			contact = contact->next();
		}
	}
}

float __SWPhysics2D::getFixedInterval()
{
	static const float interval = 1.0f/30.0f;
	return interval;
}
