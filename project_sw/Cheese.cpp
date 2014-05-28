#include "Cheese.h"
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
#include "SWActSendMsg.h"

#include "SWPhysics2D.h"

#include "SWSprite.h"
#include "SWSpriteSequence.h"
#include "SWSpriteAnimation.h"

#include "SWSpriteRenderer.h"
#include "SWTransform.h"
#include "SWRigidBody2D.h"
#include "SWCollider2D.h"

#include "SWInput.h"
#include "SWLog.h"
#include "SWAssets.h"
#include "SWMath.h"

Cheese::Cheese( factory_constructor )
{
}
Cheese::~Cheese()
{
}
void Cheese::onAwake()
{
	__super::onAwake();
	SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "img1.png" );
	SWSpriteRenderer* renderer = gameObject()->addComponent<SWSpriteRenderer>();
	renderer->setSprite( atlas()->find( "cheese_icon_01" ) );

	float duration = (float)SWMath.randomInt( 10, 100 )/100.0f;
	SWAction* action = gameObject()->addComponent<SWAction>();
	SWActScale* actScale1 = new SWActScale( duration, tvec3::one*0.8f, tvec3::one );
	SWActScale* actScale2 = new SWActScale( duration, tvec3::one, tvec3::one*0.8f );
	SWActSequence* seq = new SWActSequence();
	seq->addAct( actScale1 );
	seq->addAct( actScale2 );
	action->setAct( "idle", new SWActRepeat( seq ) );
	action->play( "idle" );

	SWCircleCollider2D* collider = gameObject()->addComponent<SWCircleCollider2D>();
	collider->setRadius( 20 );

	setMessageDelegator( "deleteGO", GetDelegator( onDeleteGO ) );

}
void Cheese::onCollision( SWCollision2D* coll )
{
	const thashstr& name = coll->getCollider()->gameObject()->getName();
	if ( name != "mouse" ) return;

	SWAction* action = getComponent<SWAction>();
	if ( action->isPlaying( "boom" ) ) return;

	SWHardRef<SWSpriteAnimation> animation = SWAssets.loadSpriteAnimation( "animation2.txt" );
	SWActAnimate* anim = new SWActAnimate( 1, animation()->getSequenceByName( "boom" ) );
	SWActSequence* seq = new SWActSequence();
	seq->addAct( anim );
	seq->addAct( new SWActSendMsg( "deleteGO" ) );
	action->setAct( "boom", seq );
	action->play( "boom" );

	gameObject()->removeComponent<SWCircleCollider2D>();
}
void Cheese::onUpdate()
{
}
void Cheese::onDeleteGO()
{
	gameObject()->destroy();
}