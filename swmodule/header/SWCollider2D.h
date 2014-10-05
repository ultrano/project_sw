#ifndef SWCollider2D_h__
#define SWCollider2D_h__

#include "SWComponent.h"

class SWCollider2D : public SWComponent
{
	SW_RTTI( SWCollider2D, SWComponent );
public:

	void onAwake();
	void onRemove();
	
};

class SWCollision2D : public SWObject
{
	SW_RTTI( SWCollision2D, SWObject );

public:

	SWCollision2D() : collider( NULL ) {};
	SWCollision2D( SWCollider2D* _collider ) : collider( _collider ) {};
	
	SWWeakRef<SWCollider2D> collider;
};

#endif // SWCollider2D_h__