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
	m_gravityScale = tvec2::axisY;
}

void SWRigidBody2D::onRemove()
{
	gameObject()->removeUpdateDelegator( GetDelegator( onUpdate ) );
}

void SWRigidBody2D::onUpdate()
{
	SWTransform* transform = getComponent<SWTransform>();
	
	float deltaTime = SWTime.getDeltaTime();
	addAccel( m_gravityScale * (-9.85f) * deltaTime );
	tvec2 step = m_velocity * deltaTime;
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

void SWRigidBody2D::setGravityScale( const tvec2& scale )
{
	m_gravityScale = scale;
}

void SWRigidBody2D::setElastic( float elastic )
{
	m_elastic = elastic;
}

void SWRigidBody2D::setMass( tufloat mass )
{
	if ( mass > 0 ) m_mass = mass;
	else m_mass = FLT_EPSILON;
}