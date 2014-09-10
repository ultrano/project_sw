#include "SWRigidBody2D.h"
#include <float.h>
#include "SWGameObject.h"
#include "SWTransform.h"
#include "SWPhysics2D.h"
#include "SWTime.h"
#include "SWMath.h"
#include "SWObjectStream.h"

SWRigidBody2D::SWRigidBody2D()
	: m_center( tvec2::zero )
	, m_angle( 0 )
	, m_velocity( tvec2::zero )
	, m_torque( 0 )
	, m_elastic( 0 )
	, m_mass( 1 )
	, m_inertia( 10 )
	, m_linearDrag( 0.1f )
	, m_angularDrag( 0.1f )
	, m_gravityScale( -tvec2::axisY )
	, m_fixedAngle( false )
{
}

SWRigidBody2D::SWRigidBody2D( factory_constructor )
	: m_center( tvec2::zero )
	, m_angle( 0 )
	, m_velocity( tvec2::zero )
	, m_torque( 0 )
	, m_elastic( 0 )
	, m_mass( 1 )
	, m_inertia( 10 )
	, m_linearDrag( 0.1f )
	, m_angularDrag( 0.1f )
	, m_gravityScale( -tvec2::axisY )
	, m_fixedAngle( false )
{

}

SWRigidBody2D::~SWRigidBody2D()
{
}

void SWRigidBody2D::onAwake()
{
	__super::onAwake();
	SWPhysics2D.m_bodies.push_back( this );
}

void SWRigidBody2D::onStart()
{
	__super::onStart();
	gameObject()->addUpdateDelegator( GetDelegator( onUpdate ) );
	SWTransform* transform = getComponent<SWTransform>();
	m_center = transform->getPosition().xy();
}

void SWRigidBody2D::onRemove()
{
	SWPhysics2D.m_bodies.remove( this );
	gameObject()->removeUpdateDelegator( GetDelegator( onUpdate ) );
	__super::onRemove();
}
void SWRigidBody2D::onUpdate()
{
	float deltaTime = SWTime.getDeltaTime();
	if ( deltaTime == 0 ) return;

	SWTransform* transform = getComponent<SWTransform>();
	
	float depth = transform->getPosition().z;

	addAccel( m_gravityScale * ( SWPhysics2D.getGravityForce() ) * deltaTime );
	
	tvec2 linearStep = m_velocity* deltaTime;
	m_velocity -= linearStep * m_linearDrag;
	m_center   += linearStep;
	transform->setPosition( tvec3( m_center, depth ) );
	
	if ( !m_fixedAngle )
	{
		tfloat angularStep = m_torque* deltaTime;
		m_torque -= angularStep * m_angularDrag;
		m_angle  += angularStep;
		transform->setRotate( tquat().rotate( 0, 0, m_angle ) );
	}
}

void SWRigidBody2D::addForce( const tvec2& force )
{
	if ( m_mass == 0 ) return;

	m_velocity += force/m_mass;
}

void SWRigidBody2D::addForce( const tvec2& force, const tvec2& pos )
{
	if ( m_mass == 0 ) return;
	tvec2 radius = (pos - m_center);
	float torque = radius.cross( force );

	m_velocity += force/m_mass;
	m_torque   += torque/m_inertia;
}

void SWRigidBody2D::addAccel( const tvec2& accel )
{
	m_velocity += accel;
}

void SWRigidBody2D::setPosition( const tvec2& center )
{
	m_center = center;
}

const tvec2& SWRigidBody2D::getPosition() const
{
	return m_center;
}

void SWRigidBody2D::setVelocity( const tvec2& vel )
{
	m_velocity = vel;
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

void SWRigidBody2D::setMass( tfloat mass )
{
	if ( mass > 0 ) m_mass = mass;
	else m_mass = FLT_EPSILON;
}

void SWRigidBody2D::setInertia( tfloat inertia )
{
	if ( inertia > 0 ) m_inertia = inertia;
	else m_inertia = FLT_EPSILON;
}

void SWRigidBody2D::setFixedAngle( bool isFixed )
{
	m_fixedAngle = isFixed;
}

const tvec2& SWRigidBody2D::getVelocity() const
{
	return m_velocity;
}

const float& SWRigidBody2D::getTorque() const
{
	return m_torque;
}

bool SWRigidBody2D::getFixedAngle() const
{
	return m_fixedAngle;
}

void SWRigidBody2D::serialize( SWObjectWriter* ow )
{
	ow->writeVec2( m_center );
	ow->writeVec2( m_velocity );
	ow->writeVec2( m_gravityScale );
	ow->writeFloat( m_angle );
	ow->writeFloat( m_torque );
	ow->writeFloat( m_mass );
	ow->writeFloat( m_inertia );
	ow->writeFloat( m_elastic );
	ow->writeFloat( m_linearDrag );
	ow->writeFloat( m_angularDrag );
}

void SWRigidBody2D::deserialize( SWObjectReader* or )
{
	or->readVec2( m_center );
	or->readVec2( m_velocity );
	or->readVec2( m_gravityScale );
	m_angle   = or->readFloat();
	m_torque  = or->readFloat();
	m_mass    = or->readFloat();
	m_inertia = or->readFloat();
	m_elastic = or->readFloat();
	m_linearDrag  = or->readFloat();
	m_angularDrag = or->readFloat();
}
