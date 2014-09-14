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

	gameObject()->setName( "Obstacle" );

	SWSpriteRenderer* renderer = gameObject()->addComponent<SWSpriteRenderer>();
	
	SWAction* action = gameObject()->addComponent<SWAction>();

	SWHardRef<SWSpriteAnimation> anim = SWAssets.loadSpriteAnimation( "animations/lightning_anim.txt" );
	SWSpriteSequence* animSeq = anim()->getSequenceAt(0);
	SWActAnimate* act = new SWActAnimate( 1, animSeq );
	SWActRotateBy* rot = new SWActRotateBy( 1, tvec3( 0, 0, 1 ) );

	renderer->setSprite( animSeq->getSpriteAt(0) );

	SWActBunch* bunch = new SWActBunch;
	bunch->addAct( new SWActRepeat( act ) );
	bunch->addAct( new SWActRepeat( rot ) );
	action->setAct( "lightning", bunch );
	action->play( "lightning" );

	SWTransform* trans = getComponent<SWTransform>();
	trans->setLocalScale( tvec3::one * 0.5f );

	SWRectCollider2D* collider = gameObject()->addComponent<SWRectCollider2D>();
	collider->setSize( tvec2( 35, 210 ) );

}

void Obstacle::onCollision( SWCollision2D* coll )
{
	SWGameObject* go = coll->collider()->gameObject();

	if ( go->getName() == "Basket" )
	{
		SWGameObject* poolGO = SW_GC.getScene()->findGO( "Pool" );
		getComponent<SWTransform>()->setParent( poolGO->getComponent<SWTransform>() );
	}
	else if ( go->getName() == "Character" )
	{
		//SWHardRef<SWObject> object = SW_GC.newInstance( "IntroScene" );
		//SWHardRef<SWGameScene> scene = swrtti_cast<SWGameScene>( object() );
		//SW_GC.setNextScene( scene() );
	}

}
