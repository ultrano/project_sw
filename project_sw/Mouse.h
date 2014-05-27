#ifndef Mouse_h__
#define Mouse_h__

#include "SWBehavior.h"
#include "SWGameObject.h"
#include "SWAction.h"
#include "SWActMove.h"
#include "SWActRotate.h"
#include "SWActScale.h"
#include "SWActRepeat.h"
#include "SWActSequence.h"
#include "SWActDelay.h"
#include "SWActDelegate.h"
#include "SWActAnimate.h"

#include "SWPhysics2D.h"

#include "SWSprite.h"
#include "SWSpriteSequence.h"
#include "SWSpriteRenderer.h"
#include "SWSpriteAnimation.h"
#include "SWAssets.h"

#include "SWRigidBody2D.h"
#include "SWCollider2D.h"

#include "SWInput.h"

class Mouse : public SWBehavior
{
	SW_RTTI( Mouse, SWBehavior );

public:

	Mouse( factory_constructor )
	{

	}

	~Mouse()
	{

	}
	virtual void onAwake()
	{
		__super::onAwake();
		gameObject()->addComponent<SWSpriteRenderer>();

		SWRigidBody2D* body = gameObject()->addComponent<SWRigidBody2D>();
		body->setGravityScale( tvec2::zero );
		body->setLinearDrag( 0.9f );

		SWCircleCollider2D* collider = gameObject()->addComponent<SWCircleCollider2D>();
		collider->setRadius(20);

		SWHardRef<SWSpriteAnimation> anim = SWAssets.loadSpriteAnimation( "animation.txt" );

		SWAct* idle = new SWActRepeat( new SWActAnimate( 1, anim()->getSequenceByName( "idle" ) ) );
		SWAct* run  = new SWActRepeat( new SWActAnimate( 1, anim()->getSequenceByName( "run" ) ) );
		SWAction* action = gameObject()->addComponent<SWAction>();
		action->setAct( "idle", idle );
		action->setAct( "run", run );
		action->play( "idle" );
	}
	virtual void onCollision( SWCollision2D* coll )
	{
		SWLog("sfdsdfs");
	}
	virtual void onUpdate()
	{
		__super::onUpdate();

		tvec2 delta( SWInput.getDeltaX(), -SWInput.getDeltaY() );
		
		SWRigidBody2D* body = getComponent<SWRigidBody2D>();
		body->addForce( delta.normal() * 10 );

		if ( delta.length() != 0 )
		{
			getComponent<SWTransform>()->setLocalScale( tvec3( ( delta.x < 0 )?-1:+1,1,1 ) );
		}

		const tvec2& velocity = body->getVelocity();
		if ( velocity.length() >= 10 )
		{
			SWAction* action = getComponent<SWAction>();
			if ( !action->isPlaying( "run" ) ) action->play( "run" );
		}
		else
		{
			body->setVelocity( tvec2::zero );
			SWAction* action = getComponent<SWAction>();
			if ( !action->isPlaying( "idle" ) ) action->play( "idle" );
		}
	}
};

#endif // Mouse_h__