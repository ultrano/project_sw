#include "SWWorld2D.h"
#include "SWMath.h"
#include "SWRefNode.h"
#include "SWContact2D.h"
#include "SWFixture2D.h"
#include "SWCollider2D.h"
#include "SWGameObject.h"
#include "SWRigidBody2D.h"
#include "SWBroadPhase2D.h"
#include "SWLog.h"

SWWorld2D::SWWorld2D()
	: m_broadPhase( new SWBroadPhase2D )
{

}

SWWorld2D::~SWWorld2D()
{

}

SWContact2D* SWWorld2D::createContact( const SWFixture2D* fixture1, const SWFixture2D* fixture2, float friction, float bounciness )
{
	SWContact2D* contact = new SWContact2D;
	SWRefNode* node = new SWRefNode;

	node->ref = contact;
	node->prev = NULL;
	node->next = m_contactList();
	
	contact->node = node;
	contact->fixture1 = fixture1;
	contact->fixture2 = fixture2;
	contact->friction = friction;
	contact->bounciness = bounciness;

	if ( m_contactList() ) m_contactList()->prev = node;
	m_contactList = node;

	return contact;
}

void SWWorld2D::removeContact( SWContact2D* contact )
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
	SWRefNode* next = node->next();
	SWRefNode* prev = node->prev();
	if ( next ) next->prev = prev;
	if ( prev ) prev->next = next;
	if ( m_contactList() == node ) m_contactList = next;
}

SWBroadPhase2D* SWWorld2D::getBroadPhase() const
{
	return m_broadPhase();
}

void SWWorld2D::addBody( SWRigidBody2D* body )
{
	SWRefNode* node = new SWRefNode;
	node->ref = body;
	node->next = m_bodyList();
	if ( m_bodyList() ) m_bodyList()->prev = node;
	m_bodyList = node;
	body->m_node = node;
}

void SWWorld2D::removeBody( SWRigidBody2D* body )
{
	SWRefNode* node = body->m_node();
	SWRefNode* next = node->next();
	SWRefNode* prev = node->prev();
	if ( next ) next->prev = prev;
	if ( prev ) prev->next = next;
	if ( m_bodyList() == node ) m_bodyList = next;
	body->m_node = NULL;
}

bool SWWorld2D::existContact( const SWCollider2D* collider, const SWFixture2D* fixture1, const SWFixture2D* fixture2 )
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

void SWWorld2D::update( float step )
{
	findNewContacts();
	updateContacts();
}

void SWWorld2D::findNewContacts()
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
		if ( body1 == NULL && body2 == NULL ) continue;

		float friction = SWMath.sqrt( fixture1->getFriction() * fixture2->getFriction() );
		float bounciness = (fixture1->getBounciness() + fixture2->getBounciness()) * 0.5f;
		SWContact2D* contact = createContact(fixture1, fixture2, friction, bounciness);

		if ( collider1 ) collider1->addContactEdge( contact );
		if ( collider2 ) collider2->addContactEdge( contact );
	}
}

void SWWorld2D::updateContacts()
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
			if ( contact ) contact->update();
		}
	}

	//! prototype solving
	do
	{
		//! break;
		SWRefNode* node = m_contactList();
		while ( node )
		{
			SWContact2D* contact = (SWContact2D*)node->ref();
			node = node->next();			
			if ( !contact ) continue;
			if ( !contact->state.get( SWContact2D::eTouching ) ) continue;

			tvec2 vel;
			SWRigidBody2D* body1 = contact->fixture1()->getCollide()->getComponent<SWRigidBody2D>();
			if ( body1 )
			{
				vel = body1->getVelocity();
				vel += -contact->normal * contact->depth;
				body1->setVelocity( vel );
				SWLog( "body1" );
			}
			SWRigidBody2D* body2 = contact->fixture2()->getCollide()->getComponent<SWRigidBody2D>();
			if ( body2 )
			{
				vel = body2->getVelocity();
				vel += contact->normal * contact->depth;
				body2->setVelocity( vel );
				SWLog( "body2" );
			}
		}
	} while (false);

	//! send message
	{
		SWHardRef<SWRefNode> node = m_contactList();
		while ( node.isValid() )
		{
			SWContact2D* contact = (SWContact2D*)node()->ref();
			node = node()->next();
			if ( !contact ) continue;

			SWCollider2D* collider1 = contact->fixture1()->getCollide();
			SWCollider2D* collider2 = contact->fixture2()->getCollide();
			SWWeakRef<SWGameObject> go1 = collider1->gameObject();
			SWWeakRef<SWGameObject> go2 = collider2->gameObject();
			if ( contact->state.get( SWContact2D::eEntering ) )
			{
				go1()->sendMessage( onCollisionEnter, NULL );
				if ( go2.isValid() ) go2()->sendMessage( onCollisionEnter, NULL );
			}
			else if ( contact->state.get( SWContact2D::eStaying ) )
			{
				go1()->sendMessage( onCollisionStay, NULL );
				if ( go2.isValid() ) go2()->sendMessage( onCollisionStay, NULL );
			}
			else if ( contact->state.get( SWContact2D::eLeaving ) )
			{
				go1()->sendMessage( onCollisionLeave, NULL );
				if ( go2.isValid() ) go2()->sendMessage( onCollisionLeave, NULL );
			}
		}
	}

	//! remove contacts that are out of bound
	{
		SWHardRef<SWRefNode> node = m_contactList();
		while ( node.isValid() )
		{
			SWContact2D* contact = (SWContact2D*)node()->ref();
			node = node()->next();
			if ( !contact ) continue;

			tuint proxyID1 = contact->fixture1()->getProxyID();
			tuint proxyID2 = contact->fixture2()->getProxyID();

			if ( !m_broadPhase()->testOverlap( proxyID1, proxyID2 ) )
			{
				removeContact( contact );
			}
		}
	}
}

