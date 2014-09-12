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
	gameObject()->addFixedRateUpdateDelegator( GetDelegator( onFixedRateUpdate ) );
	SWTransform* transform = getComponent<SWTransform>();
	m_center = transform->getPosition().xy();
}

void SWRigidBody2D::onRemove()
{
	SWPhysics2D.m_bodies.remove( this );
	gameObject()->removeFixedRateUpdateDelegator( GetDelegator( onFixedRateUpdate ) );
	__super::onRemove();
}

void SWRigidBody2D::onFixedRateUpdate()
{
	SWTransform* transform = getComponent<SWTransform>();
	
	float depth = transform->getPosition().z;
	float gravityForce = SWPhysics2D.getGravityForce();
	float interval = SWPhysics2D.getFixedInterval();//nowTime - m_lastTime;

	addAccel( m_gravityScale * gravityForce * interval );
	
	tvec2 linearStep = m_velocity * interval;
	m_velocity -= linearStep * m_linearDrag;
	m_center   += linearStep;
	transform->setPosition( tvec3( m_center, depth ) );
	
	if ( !m_fixedAngle )
	{
		tfloat angularStep = m_torque * interval;
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

void SWRigidBody2D::serialize( SWObjectWriter* writer )
{
	writer->writeVec2( m_center );
	writer->writeVec2( m_velocity );
	writer->writeVec2( m_gravityScale );
	writer->writeFloat( m_angle );
	writer->writeFloat( m_torque );
	writer->writeFloat( m_mass );
	writer->writeFloat( m_inertia );
	writer->writeFloat( m_elastic );
	writer->writeFloat( m_linearDrag );
	writer->writeFloat( m_angularDrag );
}

void SWRigidBody2D::deserialize( SWObjectReader* reader )
{
	reader->readVec2( m_center );
	reader->readVec2( m_velocity );
	reader->readVec2( m_gravityScale );
	m_angle   = reader->readFloat();
	m_torque  = reader->readFloat();
	m_mass    = reader->readFloat();
	m_inertia = reader->readFloat();
	m_elastic = reader->readFloat();
	m_linearDrag  = reader->readFloat();
	m_angularDrag = reader->readFloat();
}
