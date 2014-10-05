#include "SWCollider2D.h"
#include "SWTransform.h"
#include "SWMath.h"
#include "SWPhysics2D.h"
#include "SWObjectStream.h"
#include "SWRigidBody2D.h"

void SWCollider2D::onAwake()
{
	__super::onAwake();
	SWPhysics2D.m_colliders.push_back( this );
}

void SWCollider2D::onRemove()
{
	SWPhysics2D.m_colliders.remove( this );
	__super::onRemove();
}
