#include "Rider.h"
#include "GasCloud.h"

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
	body()->setGravityScale( -tvec2::axisY * 10 );

}

void Rider::onRemove()
{
	__super::onRemove();
}

void Rider::onUpdate()
{
}

void Rider::onFixedRateUpdate()
{
	SWHardRef<SWRigidBody2D> body = gameObject()->getComponent<SWRigidBody2D>();
	body()->addForce( tvec2( 0.1f,0 ) );
	if ( SWInput.getKey( ' ' ) )
	{
		body()->addForce( tvec2( 0, 30 ) );

		SWGameObject* go = new SWGameObject();
		go->addComponent<GasCloud>();
		tvec3 pos = getComponent<SWTransform>()->getPosition();
		pos.z = -1;
		go->getComponent<SWTransform>()->setPosition( pos );

		go->addComponent<SWRigidBody2D>()->addForce( tvec2( SWMath.randomInt( -100,100 )/20.0f, -30 ) );

	}
}

void Rider::onCollision( SWCollision2D* )
{
}

