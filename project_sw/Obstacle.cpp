#include "Obstacle.h"

Obstacle::Obstacle( factory_constructor )
{

}

Obstacle::~Obstacle()
{

}

void Obstacle::onAwake()
{
	__super::onAwake();

	SWSpriteRenderer* renderer = gameObject()->addComponent<SWSpriteRenderer>();
	SWAction* action = gameObject()->addComponent<SWAction>();

	SWHardRef<SWSpriteAnimation> anim = SWAssets.loadSpriteAnimation( "animations/lightning_anim.txt" );
	SWActAnimate* act = new SWActAnimate( 1, anim()->getSequenceAt(0) );
	SWActRotateBy* rot = new SWActRotateBy( 1, tvec3( 0, 0, 1 ) );

	SWActBunch* bunch = new SWActBunch;
	bunch->addAct( new SWActRepeat( act ) );
	bunch->addAct( new SWActRepeat( rot ) );
	action->setAct( "lightning", bunch );

	SWTransform* trans = getComponent<SWTransform>();
	trans->setPosition( tvec3( 100, 0, 0 ) );
	trans->setLocalScale( tvec3::one * 0.5f );

	SWRectCollider2D* collider = gameObject()->addComponent<SWRectCollider2D>();
	collider->setSize( tvec2( 35, 210 ) );

}

void Obstacle::onCollision( SWCollision2D* )
{
	SWLog( "something hit" );
}
