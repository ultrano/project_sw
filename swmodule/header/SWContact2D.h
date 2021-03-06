#ifndef SWContact2D_h__
#define SWContact2D_h__

#include "SWRefCounter.h"
#include "SWType.h"
#include "SWShape2D.h"

class SWFixture2D;
class SWRefNode;
class SWContact2D : public SWRefCountable
{
public:
	enum
	{
		eTouching = 0,
		eEntering = 1,
		eStaying  = 2,
		eLeaving  = 3,
	};
public:
	SWWeakRef<SWFixture2D> fixture1;
	SWWeakRef<SWFixture2D> fixture2;
	float friction;
	float bounciness;
	
	SWManifold manifold;
	tvec2 cm1, cm2; // center of mass in world

	tflag8 state;
	SWWeakRef<SWRefNode> node;

	SWContact2D();
	~SWContact2D();
	bool evaluate();
	void update();
};

class SWCollider2D;
class SWContactEdge2D : public SWRefCountable
{
public:
	SWWeakRef<SWCollider2D> other;
	SWWeakRef<SWContact2D> contact;
	SWHardRef<SWContactEdge2D> next;
	SWWeakRef<SWContactEdge2D> prev;
	SWContactEdge2D();
	~SWContactEdge2D();
};

#endif // SWContact2D_h__