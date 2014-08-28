#include "GasCloud.h"

GasCloud::GasCloud()
{
}

GasCloud::GasCloud( factory_constructor )
{
}

GasCloud::~GasCloud()
{
}

void GasCloud::onAwake()
{
	__super::onAwake();
	
	SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "cloud.png" );
	SWHardRef<SWSpriteRenderer> renderer = gameObject()->addComponent<SWSpriteRenderer>();
	renderer()->setSprite( atlas()->find( "cloud_0" ) );
	
	SWHardRef<SWTransform> trans = getComponent<SWTransform>();
	trans()->setLocalScale( tvec3( 0.2f, 0.2f, 1 ) );

	SWHardRef<SWRigidBody2D> body = gameObject()->addComponent<SWRigidBody2D>();

	SWHardRef<SWAction> action = gameObject()->addComponent<SWAction>();
	SWActSequence* seq = new SWActSequence();
	seq->addAct( new SWActColorTo( 3, tcolor( 1,1,1,0 ) ) );
	seq->addAct( new SWActDestroy() );
	action()->setAct( "fadeout", seq );
	action()->play( "fadeout" );


}

void GasCloud::onRemove()
{
	__super::onRemove();
}

void GasCloud::onUpdate()
{
}

void GasCloud::onFixedRateUpdate()
{
}

void GasCloud::onCollision( SWCollision2D* )
{
}