#include "Pipe.h"

Pipe::Pipe( factory_constructor )
{
}

void Pipe::onAwake()
{
	__super::onAwake();

	SWGameObject* go = gameObject();
	SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
	SWRectCollider2D* collider = go->addComponent<SWRectCollider2D>();
	SWTransform* transform = go->getComponent<SWTransform>();
	SWAction* action = go->addComponent<SWAction>();

	SWAct* act1 = new SWActMoveBy( 5, tvec3( -50, 0, 0 ) );
	action->setAct( "move", act1 );
	action->play( "move" );

	transform->setPosition( tvec3( 100, SWMath.randomInt( 60, 130 ) ,0 ) );

	collider->setSize( tvec2( 28, 125 ) );
	
	SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "flappy_bird.png" );

	renderer->setSprite( atlas()->find( "pipe_0" ) );


}