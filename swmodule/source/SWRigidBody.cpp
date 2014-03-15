#include "SWRigidBody.h"
#include "SWGameObject.h"
#include "SWTransform.h"
#include "SWTime.h"

void SWRigidBody::onStart()
{
	gameObject()->addUpdateDelegator( GetDelegator( onUpdate ) );
	m_velocity = tvec3::zero;
	m_elastic  = 0;
	m_mass = 1;
	m_drag = 0.1f;
}

void SWRigidBody::onRemove()
{
	gameObject()->removeUpdateDelegator( GetDelegator( onUpdate ) );
}

void SWRigidBody::onUpdate()
{
	SWTransform* transform = getComponent<SWTransform>();

	transform->move( m_velocity * SWTime.getDeltaTime() );
	m_velocity -= m_velocity * m_drag * SWTime.getDeltaTime();
	
}

void SWRigidBody::addForce( const tvec3& force )
{
	if ( m_mass <= 0 ) return;

	m_velocity += force/m_mass;
}
