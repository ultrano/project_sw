#include "Coin.h"
#include "GameValues.h"

Coin::Coin()
{
}

Coin::Coin( factory_constructor )
{
}

Coin::~Coin()
{
}

void Coin::deposit()
{
	SWTransform* trans = SW_GC.getScene()->findGO( "Pool" )->getComponent<SWTransform>();
	getComponent<SWTransform>()->setParent( trans );

	SWAction* action = getComponent<SWAction>();
	action->stop();
}

void Coin::withDraw()
{
	SWTransform* trans = SW_GC.getScene()->findGO( "Coins" )->getComponent<SWTransform>();
	getComponent<SWTransform>()->setParent( trans );

	SWAction* action = getComponent<SWAction>();
	action->play( "spin" );
}

void Coin::onAwake()
{
	__super::onAwake();

	SWHardRef<SWTransform> trans = getComponent<SWTransform>();
	trans()->setLocalScale( tvec3( 0.5f, 0.5f, 1 ) );

	SWSpriteRenderer* renderer = gameObject()->addComponent<SWSpriteRenderer>();

	//! sharing mesh, coin is same appearance.
	SWGameObject* coinGO = SW_GC.getScene()->findGO( "Pool/Coin" );
	if ( coinGO )
	{
		SWSpriteRenderer* spriteRenderer = coinGO->getComponent<SWSpriteRenderer>();
		renderer->setMesh( spriteRenderer->getMesh() );
	}

	SWHardRef<SWSpriteAnimation> anim = SWAssets.loadSpriteAnimation( "animations/coin_anim.txt" );
	SWActAnimate* act = new SWActAnimate( 1, anim()->getSequenceAt(0) );

	SWAction* action = gameObject()->addComponent<SWAction>();
	action->setAct( "spin", new SWActRepeat( act ) );

	SWHardRef<SWCircleCollider2D> collider = gameObject()->addComponent<SWCircleCollider2D>();
	collider()->setRadius( 5 );

	deposit();
}

void Coin::onFixedRateUpdate()
{
}

void Coin::onCollision( SWCollision2D* )
{
	deposit();
}