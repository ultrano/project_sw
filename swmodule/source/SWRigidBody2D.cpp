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

SWRigidBody2D::SWRigidBody2D()
	: m_position( tvec2::zero )
	, m_rotate( 0 )
	, m_velocity( tvec2::zero )
	, m_torque( 0 )
	, m_elastic( 0 )
	, m_mass( 1 )
	, m_inertia( 10 )
	, m_linearDrag( 0.1f )
	, m_angularDrag( 0.1f )
	, m_gravityScale( -tvec2::axisY )
	, m_fixedAngle( false )
	, m_fixedPosition( false )
{
}

SWRigidBody2D::SWRigidBody2D( factory_constructor )
	: m_position( tvec2::zero )
	, m_rotate( 0 )
	, m_velocity( tvec2::zero )
	, m_torque( 0 )
	, m_elastic( 0 )
	, m_mass( 1 )
	, m_inertia( 10 )
	, m_linearDrag( 0.1f )
	, m_angularDrag( 0.1f )
	, m_gravityScale( -tvec2::axisY )
	, m_fixedAngle( false )
	, m_fixedPosition( false )
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
	SWTransform* transform = getComponent<SWTransform>();
	
	float depth = transform->getPosition().z;
	float gravityForce = SWPhysics2D.getGravityForce();

	if ( !isFixedPosition() )
	{
		addAccel( m_gravityScale * gravityForce );
		m_position += m_velocity;
		addAccel( -m_velocity * m_linearDrag );
		transform->setPosition( tvec3( m_position, depth ) );
	}
	
	if ( !isFixedAngle() )
	{
		m_torque -= m_torque * m_angularDrag;
		m_rotate  += m_torque;
		transform->setRotate( tquat().rotate( 0, 0, m_rotate ) );
	}
}

void SWRigidBody2D::addForce( const tvec2& force )
{
	if ( m_mass == 0 ) return;

	addAccel( force/m_mass );
}

void SWRigidBody2D::addForce( const tvec2& force, const tvec2& pos )
{
	if ( m_mass == 0 ) return;
	tvec2 radius = (pos - m_position);
	float torque = radius.cross( force );

	addAccel( force/m_mass );
	m_torque   += torque/m_inertia;
}

void SWRigidBody2D::addAccel( const tvec2& accel )
{
	m_velocity += accel;
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

void SWRigidBody2D::setFixedPosition( bool isFixed )
{
	m_fixedPosition = isFixed;
}

const tvec2& SWRigidBody2D::getVelocity() const
{
	return m_velocity;
}

const float& SWRigidBody2D::getTorque() const
{
	return m_torque;
}

bool SWRigidBody2D::isFixedAngle() const
{
	return m_fixedAngle;
}

bool SWRigidBody2D::isFixedPosition() const
{
	return m_fixedPosition;
}

void SWRigidBody2D::serialize( SWObjectWriter* writer )
{
	writer->writeVec2( m_position );
	writer->writeVec2( m_velocity );
	writer->writeVec2( m_gravityScale );
	writer->writeFloat( m_rotate );
	writer->writeFloat( m_torque );
	writer->writeFloat( m_mass );
	writer->writeFloat( m_inertia );
	writer->writeFloat( m_elastic );
	writer->writeFloat( m_linearDrag );
	writer->writeFloat( m_angularDrag );
}

void SWRigidBody2D::deserialize( SWObjectReader* reader )
{
	reader->readVec2( m_position );
	reader->readVec2( m_velocity );
	reader->readVec2( m_gravityScale );
	m_rotate   = reader->readFloat();
	m_torque  = reader->readFloat();
	m_mass    = reader->readFloat();
	m_inertia = reader->readFloat();
	m_elastic = reader->readFloat();
	m_linearDrag  = reader->readFloat();
	m_angularDrag = reader->readFloat();
}
