#include "SWRigidBody2D.h"
#include <float.h>
#include "SWGameObject.h"
#include "SWTransform.h"
#include "SWPhysics2D.h"
#include "SWTime.h"
#include "SWMath.h"
#include "SWWorld2D.h"
#include "SWObjectStream.h"
#include "SWRefNode.h"

SWRigidBody2D::SWRigidBody2D( factory_constructor )
	: m_position( tvec2::zero )
	, m_rotate( 0 )
	, m_linearVel( tvec2::zero )
	, m_angularVel( 0 )
	, m_mass( 5 )
	, m_invMass( 1/m_mass )
	, m_inertia( 20 )
	, m_invInertia( 1/m_inertia )
	, m_linearDrag( 0.1f )
	, m_angularDrag( 0.1f )
	, m_gravityScale( -tvec2::axisY )
	, m_force(tvec2::zero)
	, m_torque(0)
{

}

SWRigidBody2D::~SWRigidBody2D()
{
}

void SWRigidBody2D::onAwake()
{
	__super::onAwake();
	m_world = SWPhysics2D.getWorld( gameObject()->getLayer() );
	m_world()->addBody( this );
}

void SWRigidBody2D::onStart()
{
	__super::onStart();
	gameObject()->addFixedRateUpdateDelegator( GetDelegator( onFixedRateUpdate ) );
	gameObject()->addLayerDelegator( GetDelegator( onLayerChanged ) );
	SWTransform* transform = getComponent<SWTransform>();
	m_position = transform->getPosition().xy();
}

void SWRigidBody2D::onRemove()
{
	m_world()->removeBody( this );
	gameObject()->removeFixedRateUpdateDelegator( GetDelegator( onFixedRateUpdate ) );
	gameObject()->removeLayerDelegator( GetDelegator( onLayerChanged ) );
	__super::onRemove();
}

void SWRigidBody2D::onLayerChanged()
{
	m_world()->removeBody( this );
	m_world = SWPhysics2D.getWorld( gameObject()->getLayer() );
	m_world()->addBody( this );
}

void SWRigidBody2D::onFixedRateUpdate()
{
	if ( !isPositionFixed() || !isAngleFixed() )
	{
		SWTransform* transform = getComponent<SWTransform>();
		if ( !isPositionFixed() )
		{
			float depth = transform->getPosition().z;
			transform->setPosition( tvec3( m_position, depth ) );
		}

		if ( !isAngleFixed() )
		{
			transform->setRotate( tquat().rotate( 0, 0, m_rotate ) );
		}
	}
}

void SWRigidBody2D::addForce( const tvec2& force )
{
	m_force += force;
}

void SWRigidBody2D::setForce( const tvec2& force )
{
	m_force = force;
}

const tvec2& SWRigidBody2D::getForce() const
{
	return m_force;
}

void SWRigidBody2D::addTorque( float torque )
{
	m_torque += torque;
}

void SWRigidBody2D::setTorque( float torque )
{
	m_torque = torque;
}

float SWRigidBody2D::getTorque() const
{
	return m_torque;
}

void SWRigidBody2D::addAccel( const tvec2& accel )
{
	m_linearVel += accel;
}

void SWRigidBody2D::setPosition( const tvec2& center )
{
	m_position = center;
}

const tvec2& SWRigidBody2D::getPosition() const
{
	return m_position;
}

void SWRigidBody2D::setRotate( float rotate )
{
	m_rotate = rotate;
}

float SWRigidBody2D::getRotate() const
{
	return m_rotate;
}

void SWRigidBody2D::setLinearVelocity( const tvec2& vel )
{
	m_linearVel = vel;
}

void SWRigidBody2D::setAngularVelocity( float torque )
{
	m_angularVel = torque;
}

void SWRigidBody2D::setLinearDrag( float drag )
{
	m_linearDrag = drag;
}

void SWRigidBody2D::setGravityScale( const tvec2& scale )
{
	m_gravityScale = scale;
}

const tvec2& SWRigidBody2D::getGravityScale() const
{
	return m_gravityScale;
}

void SWRigidBody2D::setMass( float mass )
{
	if ( mass > 0 )
	{
		m_mass    = mass;
		m_invMass = 1/mass;
	}
	else
	{
		m_mass    = 0;
		m_invMass = 0;
	}
}

float SWRigidBody2D::getMass() const
{
	return m_mass;
}

float SWRigidBody2D::getInvMass() const
{
	return m_invMass;
}

void SWRigidBody2D::setInertia( tfloat inertia )
{
	if ( inertia > 0 )
	{
		m_inertia    = inertia;
		m_invInertia = 1/inertia;
	}
	else
	{
		m_inertia    = 0;
		m_invInertia = 0;
	}
}

float SWRigidBody2D::getInertia() const
{
	return m_inertia;
}

float SWRigidBody2D::getInvInertia() const
{
	return m_invInertia;
}

void SWRigidBody2D::setFixedAngle( bool isFixed )
{
	m_flags.set( eFixedAngle, isFixed );
}

void SWRigidBody2D::setFixedPosition( bool isFixed )
{
	m_flags.set( eFixedPosition, isFixed );
}

void SWRigidBody2D::setSleeping( bool awake )
{
	m_flags.set( eSleeping, !awake );
}

const tvec2& SWRigidBody2D::getLinearVelocity() const
{
	return m_linearVel;
}

float SWRigidBody2D::getAngularVelocity() const
{
	return m_angularVel;
}

bool SWRigidBody2D::isAngleFixed() const
{
	return m_flags.get(eFixedAngle);
}

bool SWRigidBody2D::isPositionFixed() const
{
	return m_flags.get(eFixedPosition);
}

float SWRigidBody2D::getLinearDrag() const
{
	return m_linearDrag;
}

float SWRigidBody2D::getAngularDrag() const
{
	return m_angularDrag;
}

bool SWRigidBody2D::isSleeping() const
{
	return m_flags.get(eSleeping);
}

void SWRigidBody2D::serialize( SWObjectWriter* writer )
{
	writer->writeVec2( m_position );
	writer->writeVec2( m_linearVel );
	writer->writeVec2( m_gravityScale );
	writer->writeFloat( m_rotate );
	writer->writeFloat( m_angularVel );
	writer->writeFloat( m_mass );
	writer->writeFloat( m_inertia );
	writer->writeFloat( m_linearDrag );
	writer->writeFloat( m_angularDrag );
}

void SWRigidBody2D::deserialize( SWObjectReader* reader )
{
	reader->readVec2( m_position );
	reader->readVec2( m_linearVel );
	reader->readVec2( m_gravityScale );
	m_rotate   = reader->readFloat();
	m_angularVel  = reader->readFloat();
	m_mass    = reader->readFloat();
	m_inertia = reader->readFloat();
	m_linearDrag  = reader->readFloat();
	m_angularDrag = reader->readFloat();
}
