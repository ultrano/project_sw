#ifndef SWRigidBody2D_h__
#define SWRigidBody2D_h__

#include "SWComponent.h"

class SWRigidBody2D : public SWComponent
{
	SW_RTTI( SWRigidBody2D, SWComponent );

public:
	
	SWRigidBody2D();
	SWRigidBody2D( factory_constructor );
	~SWRigidBody2D();

	void setPosition( const tvec2& center );
	void addForce( const tvec2& force );
	void addForce( const tvec2& force, const tvec2& pos );
	void addAccel( const tvec2& accel );
	void setVelocity( const tvec2& vel );
	void setLinearDrag( float drag );
	void setGravityScale( const tvec2& scale );
	void setElastic( float elastic );
	void setMass( tfloat mass );
	void setInertia( tfloat inertia );
	void setFixedAngle( bool isFixed );

	const tvec2& getPosition() const;
	const tvec2& getVelocity() const;
	const float& getTorque() const;
	bool getFixedAngle() const;

protected:

	void serialize( SWObjectWriter* ow );
	void deserialize( SWObjectReader* or );

private:

	void onAwake();
	void onStart();
	void onRemove();
	void onUpdate();

private:

	tvec2  m_center;
	float  m_angle;

	tvec2  m_velocity;
	tfloat m_torque;

	tfloat m_mass;
	tfloat m_inertia;

	tfloat m_linearDrag;
	tfloat m_angularDrag;

	tfloat m_elastic;
	tvec2  m_gravityScale;

	bool   m_fixedAngle;
};

#endif // SWRigidBody2D_h__
