#include "Bomb.h"


Bomb::Bomb( factory_constructor )
{

}

Bomb::~Bomb()
{

}

void Bomb::onStart()
{
	//! sound
	{
		SWHardRef<SWAudioClip> audioClip = SWAssets.loadAudioClip( "audios/rocket_explode_1.wav" );
		m_sound = audioClip()->createSource();
		m_sound()->play();
	}

	//! set animation
	{
		gameObject()->addComponent<SWSpriteRenderer>();

		SWHardRef<SWSpriteAnimation> anim = SWAssets.loadSpriteAnimation( "animations/boom_anim.txt" );
		SWActAnimate* act = new SWActAnimate( 1, anim()->getSequenceAt(0) );

		SWAction* action = gameObject()->addComponent<SWAction>();
		action->setAct( "boom", new SWActSequence( act, new SWActDelay(3), new SWActDestroy() ) );
		action->play( "boom" );
	}

	SWTransform* parent = getComponent<SWTransform>();
	//parent->setLocalScale( tvec3::one * 0.5f );

	//! 
	tuint count = 10;
	while ( count-- )
	{
		SWGameObject* go = new SWGameObject;

		SWTransform* trans = go->getComponent<SWTransform>();
		trans->setPosition( parent->getPosition() );

		go->addComponent<Shrapnel>();
	}
	
}

void Bomb::onUpdate()
{

}

void Bomb::onFixedRateUpdate()
{

}

void Bomb::onCollision( SWCollision2D* )
{

}

//////////////////////////////////////////////////////////////////////////

Shrapnel::Shrapnel( factory_constructor )
{

}

Shrapnel::~Shrapnel()
{

}

void Shrapnel::onStart()
{
	m_delay = 10;
	float radian = SWMath.angleToRadian( SWMath.randomInt(0,45) );
	tvec2 dir( SWMath.cos(radian), SWMath.sin(radian) );

	SWRigidBody2D* body = gameObject()->addComponent<SWRigidBody2D>();
	body->addForce( dir*SWMath.randomInt(15,30) );
	body->setGravityScale( -tvec2::axisY*0.5f );
	body->setLinearDrag( 0.05f );

	SWAction* action = gameObject()->addComponent<SWAction>();
	action->setAct( "life", new SWActSequence( new SWActDelay(3), new SWActDestroy() ) );
	action->play( "life" );

	m_atlas = SWAssets.loadSpriteAtlas( "textures/cloud.png" );
}

void Shrapnel::onFixedRateUpdate()
{
	if ( --m_delay > 0 ) return;
	m_delay = 5;

	SWGameObject* go = new SWGameObject;
	
	SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
	renderer->setSprite( m_atlas()->find("cloud_0") );

	SWTransform* trans = go->getComponent<SWTransform>();
	trans->setLocalScale( tvec3::one * SWMath.randomInt(50,100)/100.0f );
	trans->setParent( getComponent<SWTransform>() );

	SWRigidBody2D* body = go->addComponent<SWRigidBody2D>();
	body->setGravityScale( tvec2::zero );

	SWAction* action = go->addComponent<SWAction>();
	SWActBunch* bunch = new SWActBunch( new SWActColorTo(1, tcolor(1,1,1,0)), new SWActScaleTo( 1, tvec3::zero ) );
	action->setAct( "fadeout", new SWActSequence( bunch, new SWActDestroy() ) );
	action->play( "fadeout" );

}
