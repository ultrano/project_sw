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
	, m_drag( 0.1f )
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
	tvec2 step = m_velocity * deltaTime;
	transform->move( tvec3( step.x, step.y, 0 ) );
	m_velocity -= step * m_drag;
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
	m_velocity += force/m_mass;
	m_torque   +=(pos - transform->getPosition().xy()).cross( force ) ;
	
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