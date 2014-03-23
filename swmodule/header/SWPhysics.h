#ifndef SWPhysics_h__
#define SWPhysics_h__

#include "SWType.h"
#include "SWMemory.h"

#define SWPhysics (__SWPhysics::instance())

class __SWPhysics : public SWMemory
{

private:

	float m_gravityForce;

public:

	float getGravityForce() const { return m_gravityForce; }
	float setGravityForce( float force ) { m_gravityForce = force; }

	__SWPhysics();
	~__SWPhysics();
	static __SWPhysics& instance();

};

#endif // SWPhysics_h__