#include "SWRigidBody2D.h"
#include "SWGameObject.h"
#include "SWTransform.h"
#include "SWPhysics.h"
#include "SWTime.h"
#include "SWMath.h"

SWRigidBody2D::SWRigidBody2D()
	: m_velocity( tvec2::zero )
	, m_elastic( 0 )
	, m_mass( 1 )
	, m_inertia( 10 )
	, m_linearDrag( 0.1f )
	, m_angularDrag( 0.1f )
	, m_gravityScale( -tvec2::axisY )
{
}

SWRigidBody2D::~SWRigidBody2D()
{
}

void SWRigidBody2D::onStart()
{
	gameObject()->addUpdateDelegator( GetDelegator( onUpdate ) );
}

void SWRigidBody2D::onRemove()
{
	gameObject()->removeUpdateDelegator( GetDelegator( onUpdate ) );
}

void SWRigidBody2D::onUpdate()
{
	SWTransform* transform = getComponent<SWTransform>();
	
	float deltaTime = SWTime.getDeltaTime();
	
	addAccel( m_gravityScale * ( SWPhysics.getGravityForce() ) * deltaTime );
	
	tvec2  linearStep  = m_velocity * deltaTime;
	tfloat angularStep = m_torque * deltaTime;
	
	transform->move( tvec3( linearStep.x, linearStep.y, 0 ) );
	transform->rotate( 0, 0, angularStep );

	m_velocity -= linearStep * m_linearDrag;
	m_torque   -= angularStep * m_angularDrag;
}

void SWRigidBody2D::addForce( const tvec2& force )
{
	if ( m_mass == 0 ) return;

	m_velocity += force/m_mass;
}

void SWRigidBody2D::addForce( const tvec2& force, const tvec2& pos )
{
	if ( m_mass == 0 ) return;
	SWTransform* transform = getComponent<SWTransform>();
	tvec2 radius = (pos - transform->getPosition().xy());
	float torque = radius.cross( force );

	m_velocity += force/m_mass;
	m_torque   += torque/m_inertia;
}

void SWRigidBody2D::addAccel( const tvec2& accel )
{
	m_velocity += accel;
}

void SWRigidBody2D::setLinearDrag( float drag )
{
	m_linearDrag = drag;
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

void SWRigidBody2D::setInertia( tufloat inertia )
{
	if ( inertia > 0 ) m_inertia = inertia;
	else m_inertia = FLT_EPSILON;
}
