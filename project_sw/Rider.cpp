#include "Rider.h"

Rider::Rider()
{
}

Rider::Rider( factory_constructor )
{
}

Rider::~Rider()
{
}

void Rider::onAwake()
{
	__super::onAwake();

	SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "flappy_bird.png" );
	SWHardRef<SWSpriteRenderer> renderer = gameObject()->addComponent<SWSpriteRenderer>();
	renderer()->setSprite( atlas()->find( "bird_0" ) );

	SWHardRef<SWRigidBody2D> body = gameObject()->addComponent<SWRigidBody2D>();

}

void Rider::onRemove()
{
	__super::onRemove();
}

void Rider::onUpdate()
{
}

void Rider::onFixedFrameUpdate()
{
	if ( SWInput.getKey( ' ' ) )
	{
		SWHardRef<SWRigidBody2D> body = gameObject()->getComponent<SWRigidBody2D>();
		body()->addForce( tvec2( 0, 1 ) );
	}
}

void Rider::onCollision( SWCollision2D* )
{
}

