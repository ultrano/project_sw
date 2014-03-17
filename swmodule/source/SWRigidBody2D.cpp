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
}

void SWRigidBody2D::onRemove()
{
	gameObject()->removeUpdateDelegator( GetDelegator( onUpdate ) );
}

void SWRigidBody2D::onUpdate()
{
	SWTransform* transform = getComponent<SWTransform>();

	tvec2 step = m_velocity * SWTime.getDeltaTime();
	transform->move( tvec3( step.x, step.y, 0 ) );
	m_velocity -= m_velocity * m_drag * SWTime.getDeltaTime();
	
}

void SWRigidBody2D::addForce( const tvec2& force )
{
	if ( m_mass <= 0 ) return;

	m_velocity += force/m_mass;
}
