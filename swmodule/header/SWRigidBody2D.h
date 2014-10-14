#ifndef SWRigidBody2D_h__
#define SWRigidBody2D_h__

#include "SWComponent.h"

class SWRefNode;
class SWRigidBody2D : public SWComponent
{
	SW_RTTI( SWRigidBody2D, SWComponent );
	friend class SWWorld2D;
public:
	
	SWRigidBody2D();
	SWRigidBody2D( factory_constructor );
	~SWRigidBody2D();

	void setPosition( const tvec2& center );
	void setRotate( float rotate );
	void addForce( const tvec2& force );
	void addForce( const tvec2& force, const tvec2& pos );
	void addAccel( const tvec2& accel );
	void setVelocity( const tvec2& vel );
	void setTorque( float torque );
	void setLinearDrag( float drag );
	void setGravityScale( const tvec2& scale );
	void setElastic( float elastic );
	void setMass( tfloat mass );
	void setInertia( tfloat inertia );
	void setFixedAngle( bool isFixed );
	void setFixedPosition( bool isFixed );
	void setSleeping( bool awake );

	const tvec2& getPosition() const;
	float getRotate() const;
	const tvec2& getVelocity() const;
	float getTorque() const;
	bool  isFixedAngle() const;
	bool  isFixedPosition() const;
	float getLinearDrag() const;
	float getAngularDrag() const;
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

	tvec2  m_velocity;
	tfloat m_torque;

	tfloat m_mass;
	tfloat m_inertia;

	tfloat m_linearDrag;
	tfloat m_angularDrag;

	tfloat m_elastic;
	tvec2  m_gravityScale;

	bool   m_fixedAngle;
	bool   m_fixedPosition;

	SWWeakRef<SWRefNode> m_node;
	SWWeakRef<SWWorld2D> m_world;

	enum { eSleeping };
	tflag8 m_flags;
};

#endif // SWRigidBody2D_h__
