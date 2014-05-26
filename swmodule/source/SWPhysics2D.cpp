#include "SWPhysics2D.h"
#include "SWTransform.h"
#include "SWGameObject.h"
#include "SWMath.h"
#include "SWLog.h"

__SWPhysics2D& __SWPhysics2D::instance()
{
	static __SWPhysics2D s_instance;

	return s_instance;
}


__SWPhysics2D::__SWPhysics2D()
	: m_gravityForce( 9.85f )
{
}

__SWPhysics2D::~__SWPhysics2D()
{
}

void __SWPhysics2D::simulate()
{
	SWObject::WList copy;

	//! filtering invalid colliders
	{
		copy = m_colliders;
		SWObject::WList::iterator itor = copy.begin();
		for ( ; itor != copy.end() ; ++itor )
		{
			SWWeakRef<SWCollider2D> collider = swrtti_cast<SWCollider2D>( (*itor)() );
			if ( !collider.isValid() ) m_colliders.remove( collider() );
		}
	}
	
	if ( copy.size() != m_colliders.size() )copy = m_colliders;
	SWObject::WList::iterator itor1 = copy.begin();
	for ( ; itor1 != copy.end() ; ++itor1 )
	{
		SWCollider2D* collider1 = swrtti_cast<SWCollider2D>( (*itor1)() );
		
		SWObject::WList::iterator itor2 = itor1;
		for ( ; itor2 != copy.end() ; ++itor2 )
		{
			SWCollider2D* collider2 = swrtti_cast<SWCollider2D>( (*itor2)() );
			if ( collider1 == collider2 ) continue;
			if ( collider1->getComponent<SWRigidBody2D>() == NULL
				&& collider2->getComponent<SWRigidBody2D>() == NULL ) continue;
			const thashstr& layer1 = collider1->gameObject()->getLayerName();
			const thashstr& layer2 = collider2->gameObject()->getLayerName();
			if ( getIgnoreLayer( layer1, layer2 ) ) continue;
			if ( testCollide( collider1, collider2 ) )
			{
				SWLog( "collided!!" );
			}
		}
	}
}

bool __SWPhysics2D::getIgnoreLayer( const thashstr& layer1, const thashstr& layer2 )
{
	thash32 hash1 = SWMath.min( layer1.hash(), layer2.hash() );
	thash32 hash2 = SWMath.max( layer1.hash(), layer2.hash() );
	thash64 hash = ((thash64)hash2<<32) | (hash1);

	IgnoreTable::iterator itor = m_ignoreTable.find( hash );
	if ( itor != m_ignoreTable.end() ) return itor->second;
	return false;
}

void __SWPhysics2D::ignoreLayer( const thashstr& layer1, const thashstr& layer2, bool ignore )
{
	thash32 hash1 = SWMath.min( layer1.hash(), layer2.hash() );
	thash32 hash2 = SWMath.max( layer1.hash(), layer2.hash() );
	thash64 hash = ((thash64)hash2<<32) | (hash1);

	m_ignoreTable[ hash ] = ignore;
}

SWCollider2D* __SWPhysics2D::overlapPoint( const tvec2& point )
{
	SWObject::WList::iterator itor1 = m_colliders.begin();
	for ( ; itor1 != m_colliders.end() ; ++itor1 )
	{
		SWCollider2D* collider = swrtti_cast<SWCollider2D>( (*itor1)() );
		if ( collider == NULL ) continue;
		if ( collider->containPoint( point ) ) return collider;
	}
	return NULL;
}

bool __SWPhysics2D::testCollide( SWCollider2D* collider1, SWCollider2D* collider2 )
{
	if ( collider1 == NULL || collider2 == NULL ) return false;
	if ( collider1 == collider2 ) return false;
	
	struct Tester
	{
		static bool Circles( SWCollider2D* collider1, SWCollider2D* collider2 )
		{
			SWCircleCollider2D* circle1 = swrtti_cast<SWCircleCollider2D>( collider1 );
			SWCircleCollider2D* circle2 = swrtti_cast<SWCircleCollider2D>( collider2 );
			tvec2 dist = circle1->getWorldCenter() - circle2->getWorldCenter();
			float radius = circle1->getWorldRadius() + circle2->getWorldRadius();
			
			return ( dist.length() <= radius );
		}
		static bool CircleAndRect( SWCollider2D* collider1, SWCollider2D* collider2 )
		{
			SWCircleCollider2D* circle = swrtti_cast<SWCircleCollider2D>( collider1 );
			SWRectCollider2D* rect = swrtti_cast<SWRectCollider2D>( collider2 );
			tvec2 center1 = circle->getWorldCenter();
			tvec2 center2 = rect->getWorldCenter();
			tvec2 pos = center2 - center1;
			pos = center1 + (circle->getWorldRadius() * pos.normal());
			return ( rect->containPoint( pos ) );
		}
		static bool Rects( SWCollider2D* collider1, SWCollider2D* collider2 )
		{
			SWRectCollider2D* rect1 = swrtti_cast<SWRectCollider2D>( collider1 );
			SWRectCollider2D* rect2 = swrtti_cast<SWRectCollider2D>( collider2 );
			tvec2 edge1, edge2, edge3, edge4;
			rect1->getWorldEdges( edge1, edge2, edge3, edge4 );
			if ( rect2->containPoint( edge1 ) ) return true;
			if ( rect2->containPoint( edge2 ) ) return true;
			if ( rect2->containPoint( edge3 ) ) return true;
			if ( rect2->containPoint( edge4 ) ) return true;
			return false;
		}
	};

	if ( swrtti_cast<SWCircleCollider2D>( collider1 ) 
		&& swrtti_cast<SWCircleCollider2D>( collider2 ) ) return Tester::Circles( collider1, collider2 );
	
	if ( swrtti_cast<SWCircleCollider2D>( collider1 ) 
		&& swrtti_cast<SWRectCollider2D>( collider2 ) ) return Tester::CircleAndRect( collider1, collider2 );
	
	if ( swrtti_cast<SWRectCollider2D>( collider1 ) 
		&& swrtti_cast<SWCircleCollider2D>( collider2 ) ) return Tester::CircleAndRect( collider2, collider1 );
	
	if ( swrtti_cast<SWRectCollider2D>( collider1 ) 
		&& swrtti_cast<SWRectCollider2D>( collider2 ) ) return Tester::Rects( collider1, collider2 );

	return false;
}