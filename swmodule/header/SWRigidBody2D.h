#ifndef SWRigidBody2D_h__
#define SWRigidBody2D_h__

#include "SWComponent.h"

class SWRefNode;
class SWRigidBody2D : public SWComponent
{
	SW_RTTI( SWRigidBody2D, SWComponent );
	friend class SWWorld2D;
public:
	
	SWRigidBody2D( factory_constructor );
	~SWRigidBody2D();

	void addForce( const tvec2& force );
	void setForce( const tvec2& force );
	const tvec2& getForce() const;

	void addTorque( float torque );
	void setTorque( float torque );
	float getTorque() const;

	void addAccel( const tvec2& accel );

	void setPosition( const tvec2& center );
	const tvec2& getPosition() const;

	void setRotate( float rotate );
	float getRotate() const;

	void setLinearVelocity( const tvec2& vel );
	const tvec2& getLinearVelocity() const;

	void setAngularVelocity( float torque );
	float getAngularVelocity() const;

	void setLinearDrag( float drag );
	float getLinearDrag() const;

	float getAngularDrag() const;

	void setGravityScale( const tvec2& scale );
	const tvec2& getGravityScale() const;

	void setElastic( float elastic );

	void setMass( float mass );
	float getMass() const;
	float getInvMass() const;

	void setInertia( float inertia );
	float getInertia() const;
	float getInvInertia() const;

	void setFixedAngle( bool isFixed );
	bool isAngleFixed() const;

	void setFixedPosition( bool isFixed );
	bool isPositionFixed() const;

	void setSleeping( bool awake );
	bool isSleeping() const;

protected:

	void serialize( SWObjectWriter* writer );
	void deserialize( SWObjectReader* reader );

private:

	void onAwake();
	void onStart();
	void onRemove();
	void onLayerChanged();
	void onFixedRateUpdate();

private:

	tvec2  m_position;
	float  m_rotate;

	tvec2  m_linearVel;
	tfloat m_angularVel;

	tvec2 m_force;
	float m_torque;

	tfloat m_mass, m_invMass;
	tfloat m_inertia, m_invInertia;

	tfloat m_linearDrag;
	tfloat m_angularDrag;

	tvec2  m_gravityScale;

	enum { eSleeping, eFixedAngle, eFixedPosition };
	tflag8 m_flags;
	
	SWWeakRef<SWRefNode> m_node;
	SWWeakRef<SWWorld2D> m_world;
};

#endif // SWRigidBody2D_h__
