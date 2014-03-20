#include "SWRigidBody2D.h"
#include "SWGameObject.h"
#include "SWTransform.h"
#include "SWTime.h"

void SWRigidBody2D::onStart()
{
	gameObject()->addUpdateDelegator( GetDelegator( onUpdate ) );
	m_velocity = tvec2::zero;
	m_elastic  = 0;
	m_mass = 1;
	m_drag = 0.1f;
	m_gravityFactor = tvec2::axisY;
}

void SWRigidBody2D::onRemove()
{
	gameObject()->removeUpdateDelegator( GetDelegator( onUpdate ) );
}

void SWRigidBody2D::onUpdate()
{
	SWTransform* transform = getComponent<SWTransform>();

	addAccel( m_gravityFactor * (-9.85f) );
	tvec2 step = m_velocity * SWTime.getDeltaTime();
	transform->move( tvec3( step.x, step.y, 0 ) );
	m_velocity -= step * m_drag;
}

void SWRigidBody2D::addForce( const tvec2& force )
{
	if ( m_mass == 0 ) return;

	m_velocity += force/m_mass;
}

void SWRigidBody2D::addAccel( const tvec2& accel )
{
	m_velocity += accel;
}

void SWRigidBody2D::setDrag( float drag )
{
	m_drag = drag;
}

void SWRigidBody2D::setGravityFactor( const tvec2& factor )
{
	m_gravityFactor = factor;
}

void SWRigidBody2D::setElastic( float elastic )
{
	m_elastic = elastic;
}

void SWRigidBody2D::setMass( float mass )
{
	if ( mass > 0 ) m_mass = mass;
	else m_mass = FLT_EPSILON;
}