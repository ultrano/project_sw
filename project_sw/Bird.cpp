#include "Bird.h"


Bird::Bird( factory_constructor )
{
}

void Bird::onAwake()
{
	__super::onAwake();

	SWGameObject* go = gameObject();

	SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
	SWRigidBody2D* body = go->addComponent<SWRigidBody2D>();
	SWAction* action = go->addComponent<SWAction>();
	SWCircleCollider2D* collider = go->addComponent<SWCircleCollider2D>();

	collider->setRadius( 10 );

	SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "flappy_bird.png" );
	renderer->setSprite( atlas()->find( "bird_1" ) );

	SWHardRef<SWSpriteAnimation> anim = SWAssets.loadSpriteAnimation( "anim.txt" );
	action->setAct( "flying", new SWActRepeat( new SWActAnimate( 1, anim()->getSequenceByName( "flying" ) ) ) ); 
	action->play( "flying" );
	
	body->setGravityScale( tvec2( 0, -10 ) );
}

void Bird::onUpdate()
{
	__super::onUpdate();

	SWTransform* transform = getComponent<SWTransform>();

	tvec3 pos = transform->getPosition();
	transform->setLocalRotate( tvec3( 0, 0, SW_PI/2.0f * ( pos.y/128.0f ) ) );
}

void Bird::onCollision( SWCollision2D* )
{
	SWLog( "hit!" );

	SWHardRef<SWGameScene> scene = SW_GC.newInstance<SWGameScene>( "IntroScene" );
	SW_GC.setNextScene( scene() );
}