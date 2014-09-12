#include "GasCloud.h"
#include "GameValues.h"

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

	const char* imgList[] = { "cloud_0","cloud_1","cloud_2","cloud_3","cloud_4","cloud_5","cloud_6","cloud_7" };

	gameObject()->setName( "GasCloud" );

	SWHardRef<SWSpriteRenderer> renderer = gameObject()->addComponent<SWSpriteRenderer>();
	SWHardRef<SWRigidBody2D> body = gameObject()->addComponent<SWRigidBody2D>();
	SWHardRef<SWAction> action = gameObject()->addComponent<SWAction>();
	SWHardRef<SWTransform> trans = getComponent<SWTransform>();


	//! setting renderer
	{
		SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "textures/cloud.png" );
		renderer()->setSprite( atlas()->find( imgList[ SWMath.randomInt(0,7) ] ) );
	}

	//! setting rigid body
	body()->addForce( tvec2( SWMath.randomInt( -100,100 )/20.0f, -30 ) );
	body()->setGravityScale( tvec2::zero );
	body()->setLinearDrag( 0.2f );

	//! setting transform
	trans()->setLocalScale( tvec3( 0.1f, 0.1f, 1 ) * SWMath.randomInt( 100,200 )/100.0f );
	
	//! setting action
	{
		float time = 0.5f;
		SWActSequence* seq = new SWActSequence();
		seq->addAct( new SWActColorTo( time, tcolor( 1,1,1,0 ) ) );
		seq->addAct( new SWActDelegate( GetDelegator( goToPool ) ) );

		SWActBunch* bunch = new SWActBunch;
		bunch->addAct( seq );
		bunch->addAct( new SWActRotateTo( time, tvec3(0, 0, 3.14f)) );
		bunch->addAct( new SWActScaleBy( time, tvec3(0.1f, 0.1f, 0) ) );

		action()->setAct( "fadeout", bunch );
		action()->play( "fadeout" );
	}
}

void GasCloud::goToPool()
{
	SWTransform* poolTrans = SW_GC.getScene()->findGO( "Pool" )->getComponent<SWTransform>();
	getComponent<SWTransform>()->setParent( poolTrans );
}

void GasCloud::reset( const tvec3& pos )
{
	SWHardRef<SWSpriteRenderer> renderer = gameObject()->addComponent<SWSpriteRenderer>();
	SWHardRef<SWRigidBody2D> body = gameObject()->addComponent<SWRigidBody2D>();
	SWHardRef<SWAction> action = gameObject()->addComponent<SWAction>();
	SWHardRef<SWTransform> trans = getComponent<SWTransform>();

	renderer()->setColor( tcolor( 1,1,1,1 ) );
	body()->setPosition( pos.xy() );
	body()->setVelocity( tvec2(SWMath.randomInt(-50,0)/100.0f,-1) * SWMath.randomInt(200,0)/10.0f );
	trans()->setPosition( pos );
	trans()->setLocalScale( tvec3( 0.1f, 0.1f, 1 ) * SWMath.randomInt( 100,200 )/100.0f );
	trans()->setParent( NULL );
	action()->stop();
	action()->play( "fadeout" );
}
