#include "PipeGen.h"
#include "Pipe.h"

void PipeGen::onAwake()
{
	__super::onAwake();

	SWGameObject* go = gameObject();
	SWAction* action = go->addComponent<SWAction>();
	SWActDelay* act1 = new SWActDelay( 2.5f );
	SWActSendMsg* act2 = new SWActSendMsg( "NewPipe" );
	SWActSequence* act3 = new SWActSequence();

	act3->addAct( act1 );
	act3->addAct( act2 );

	action->setAct( "create", new SWActRepeat( act3 ) );
	action->play( "create" );

	setMessageDelegator( "NewPipe", GetDelegator( newPipe ) );
}

void PipeGen::newPipe()
{
	SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "flappy_bird.png" );

	tint middle = SWMath.randomInt( -50, 64 );
	tint gap = 90;

	//! upside pipe
	{
		SWGameObject* go = new SWGameObject;
		SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
		SWRectCollider2D* collider = go->addComponent<SWRectCollider2D>();
		SWTransform* transform = go->getComponent<SWTransform>();
		SWAction*    action    = go->addComponent<SWAction>();

		SWAct* act1 = new SWActMoveBy( 5, tvec3( -50, 0, 0 ) );
		action->setAct( "move", act1 );
		action->play( "move" );

		collider->setSize( tvec2( 28, 125 ) );

		renderer->setSprite( atlas()->find( "pipe_0" ) );

		transform->setPosition( tvec3( 100, middle + gap, 0 ) );
	}
	
	//! downside pipe
	{
		SWGameObject* go = new SWGameObject;
		SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
		SWRectCollider2D* collider = go->addComponent<SWRectCollider2D>();
		SWTransform* transform = go->getComponent<SWTransform>();
		SWAction*    action    = go->addComponent<SWAction>();

		SWAct* act1 = new SWActMoveBy( 5, tvec3( -50, 0, 0 ) );
		action->setAct( "move", act1 );
		action->play( "move" );

		collider->setSize( tvec2( 28, 110 ) );

		renderer->setSprite( atlas()->find( "pipe_1" ) );

		transform->setPosition( tvec3( 100, middle - gap, 0 ) );
	}
}