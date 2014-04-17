#ifndef SWPhysics_h__
#define SWPhysics_h__

#include "SWType.h"
#include "SWMemory.h"

#define SWPhysics (__SWPhysics2D::instance())

class __SWPhysics2D : public SWMemory
{

private:

	float m_gravityForce;

public:

	float getGravityForce() const { return m_gravityForce; }
	void setGravityForce( float force ) { m_gravityForce = force; }

	__SWPhysics2D();
	~__SWPhysics2D();
	static __SWPhysics2D& instance();

};

#endif // SWPhysics_h__