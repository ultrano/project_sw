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
	, m_contactCount(0)
	, m_bodyCount(0)
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

	m_contactCount += 1;
	//SWLog( "contact count : %d", m_contactCount );
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

	m_contactCount -= 1;
	//SWLog( "contact count : %d", m_contactCount );
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

	m_bodyCount += 1;
	SWLog( "body count : %d", m_bodyCount );
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

	m_bodyCount -= 1;
	SWLog( "body count : %d", m_bodyCount );
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

		SWRefNode* node = m_contactList();
		while ( node )
		{
			SWContact2D* contact = (SWContact2D*)node->ref();
			node = node->next();			
			if ( !contact ) continue;
			if ( !contact->state.get( SWContact2D::eTouching ) ) continue;

			SWCollider2D* collider1 = contact->fixture1()->getCollide();
			SWCollider2D* collider2 = contact->fixture2()->getCollide();
			SWRigidBody2D* body1 = collider1->getComponent<SWRigidBody2D>();
			SWRigidBody2D* body2 = collider2->getComponent<SWRigidBody2D>();

			const SWManifold& mf = contact->manifold;
			tvec2 normal = mf.normal;
			tvec2 tangent = normal.cross(1);
			

			float invI1(0), invI2(0), invM1(0), invM2(0);
			tvec2 v1(tvec2::zero), v2(tvec2::zero);
			float w1(0), w2(0);
			if ( body1 )
			{
				v1 = body1->getVelocity();
				w1 = body1->getTorque();
				invI1 = 1.0f/330.0f;
				invM1 = 1.0f/200.0f;
			}
			if ( body2 )
			{
				v2 = body2->getVelocity();
				w2 = body2->getTorque();
				invI2 = 1.0f/330.0f;
				invM2 = 1.0f/200.0f;
			}
			for ( tuint i = 0 ; i < mf.count ; ++i )
			{
				tvec2 dv = tvec2::zero;
				tvec2 point = mf.points[i];
				tvec2 r1(tvec2::zero), r2(tvec2::zero);
				float rn1(0), rn2(0), rt1(0), rt2(0);

				float kNormal = 0;
				float kTangent = 0;

				if ( body1 )
				{
					r1 = point - contact->cm1;
					float rn = normal.dot(r1);
					float rt = tangent.dot(r1);
					
					kNormal  += invM1 + invI1*(r1.dot(r1) - rn * rn);
					kTangent += invM1 + invI1*(r1.dot(r1) - rt * rt);
				}

				if ( body2 )
				{
					r2 = point - contact->cm2;
					float rn = normal.dot(r2);
					float rt = tangent.dot(r2);

					float term = (r2.dot(r2) - rn * rn);
					kNormal  += invM2 + invI2*term;
					kTangent += invM2 + invI2*(r2.dot(r2) - rt * rt);
				}

				dv = v2 + r2.cross( -w2 ) - (v1 + r1.cross( -w1 ));
				float massNormal  = 1/kNormal;
				float vn = normal.dot(dv);
				float dPn = massNormal * (-vn );
				dPn = SWMath.max(dPn,0.0f);
				tvec2 Pn = dPn * normal;

				if ( body1 )
				{
					v1 -= Pn * invM1;
					w1 -= r1.cross(Pn) * invI1;
				}

				if ( body2 )
				{
					v2 += Pn * invM2;
					w2 += r2.cross(Pn) * invI2;
				}

				dv = v2 + r2.cross( -w2 ) - (v1 + r1.cross( -w1 ));
				float massTangent = 1/kTangent;
				float vt = tangent.dot(dv);
				float dPt = massTangent * (-vt );
				dPt = SWMath.clamp(dPt,-dPt*0.2f,dPt*0.2f);
				tvec2 Pt = dPt * tangent;

				if ( body1 )
				{
					v1 -= Pt * invM1;
					w1 -= r1.cross(Pt) * invI1;
				}

				if ( body2 )
				{
					v2 += Pt * invM2;
					w2 += r2.cross(Pt) * invI2;
				}
			}

			if ( body1 )
			{
				body1->setVelocity( v1 );
				body1->setTorque( w1 );
				body1->setPosition( body1->getPosition() - normal * mf.depth );
			}

			if ( body2 )
			{
				body2->setVelocity( v2 );
				body2->setTorque( w2 );
				body2->setPosition( body2->getPosition() + normal * mf.depth );
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

