#include "Obstacle.h"
#include "GameHeaders.h"

Obstacle::Obstacle( factory_constructor )
{

}

Obstacle::~Obstacle()
{

}

void Obstacle::reset()
{

}

void Obstacle::onAwake()
{
	__super::onAwake();

	gameObject()->setName( "Obstacle" );

	SWSpriteRenderer* renderer = gameObject()->addComponent<SWSpriteRenderer>();
	
	SWAction* action = gameObject()->addComponent<SWAction>();

	SWHardRef<SWSpriteAnimation> spriteAnim = SWAssets.loadSpriteAnimation( "animations/lightning_anim.txt" );
	
	SWHardRef<SWActAnimate>  anim = new SWActAnimate( 1, spriteAnim()->getSequenceAt(0) );
	SWHardRef<SWActRotateBy> rot  = new SWActRotateBy( 1, tvec3( 0, 0, 1 ) );

	action->setAct( "pattern1", new SWActBunch( new SWActRepeat( anim() ), new SWActRepeat( rot() ) ) );

	anim = anim()->clone<SWActAnimate>();
	action->setAct( "pattern2", new SWActRepeat( anim() ) );
	
	action->play( "pattern1" );

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
		CharacterState* charState = go->getComponent<CharacterState>();
		if ( charState->queryRtti() != Runner::getRtti() )
		{
			go->removeComponent( charState->queryRtti() );
			go->addComponent<Runner>();
		}
	}

}
