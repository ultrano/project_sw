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
	
	char* imgList[] = { "cloud_0","cloud_1","cloud_2","cloud_3","cloud_4","cloud_5","cloud_6","cloud_7" };

	SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "cloud.png" );
	SWHardRef<SWSpriteRenderer> renderer = gameObject()->addComponent<SWSpriteRenderer>();
	renderer()->setSprite( atlas()->find( imgList[ SWMath.randomInt(0,7) ] ) );
	
	SWHardRef<SWTransform> trans = getComponent<SWTransform>();
	trans()->setLocalScale( tvec3( 0.1f, 0.1f, 1 ) * SWMath.randomInt( 100,200 )/100.0f );

	SWHardRef<SWRigidBody2D> body = gameObject()->addComponent<SWRigidBody2D>();

	SWHardRef<SWAction> action = gameObject()->addComponent<SWAction>();
	
	SWActSequence* seq = new SWActSequence();
	seq->addAct( new SWActColorTo( 1, tcolor( 1,1,1,0 ) ) );
	seq->addAct( new SWActDestroy() );

	SWActBunch* bunch = new SWActBunch;
	bunch->addAct( seq );
	bunch->addAct( new SWActRotateTo(1, tvec3(0, 0, 3.14f)) );
	bunch->addAct( new SWActScaleBy(1, tvec3(0.1f, 0.1f, 0) ) );

	action()->setAct( "fadeout", bunch );
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