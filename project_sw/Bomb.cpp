#include "Bomb.h"


Bomb::Bomb( factory_constructor )
{

}

Bomb::~Bomb()
{

}

void Bomb::reset( const tvec3& pos )
{
	gameObject()->setActive( true );
	gameObject()->setName( "Bomb" );

	SWTransform* parent = getComponent<SWTransform>();
	parent->setPosition( pos );

	//! 
	tuint count = parent->getChildrenCount();
	while ( count-- )
	{
		SWTransform* shrapnelTrans = parent->getChildAt( count );
		SWRigidBody2D* body = shrapnelTrans->getComponent<SWRigidBody2D>();
		body->setPosition( pos.xy() );

		Shrapnel* shrapnel = shrapnelTrans->getComponent<Shrapnel>();
		shrapnel->reset();
	}

	SWAction* action = gameObject()->getComponent<SWAction>();
	action->play( "boom" );

	m_sound()->play();
}

void Bomb::onAwake()
{
	__super::onAwake();

	//! sound
	{
		SWHardRef<SWAudioClip> audioClip = SWAssets.loadAudioClip( "audios/rocket_explode_1.wav" );
		m_sound = audioClip()->createSource();
	}

	//! set animation
	{
		gameObject()->addComponent<SWSpriteRenderer>();

		SWHardRef<SWSpriteAnimation> anim = SWAssets.loadSpriteAnimation( "animations/boom_anim.txt" );
		SWActAnimate* act = new SWActAnimate( 1, anim()->getSequenceAt(0) );

		SWAction* action = gameObject()->addComponent<SWAction>();
		action->setAct( "boom", new SWActSequence( act, new SWActDelay(1), new SWActDelegate( GetDelegator(onEndAction) ) ) );
	}

	tuint count = 10;
	while ( count-- )
	{
		SWGameObject* go = new SWGameObject;

		SWTransform* shrapnelTrans = go->getComponent<SWTransform>();
		shrapnelTrans->setParent( getComponent<SWTransform>() );

		go->addComponent<Shrapnel>();
	}

	gameObject()->setActive( false );
}

void Bomb::onStart()
{
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

void Bomb::onEndAction()
{
	gameObject()->setActive( false );
}

//////////////////////////////////////////////////////////////////////////

Shrapnel::Shrapnel( factory_constructor )
{

}

Shrapnel::~Shrapnel()
{

}

void Shrapnel::reset()
{
	float radian = SWMath.angleToRadian( SWMath.randomInt(0,45) );
	tvec2 dir( SWMath.cos(radian), SWMath.sin(radian) );

	SWRigidBody2D* body = gameObject()->addComponent<SWRigidBody2D>();
	body->addForce( dir*SWMath.randomInt(15,30) );

	m_delay = 0;
}

void Shrapnel::onAwake()
{
	__super::onAwake();
	gameObject()->setName( "Shrapnel" );
	SWRigidBody2D* body = gameObject()->addComponent<SWRigidBody2D>();
	body->setGravityScale( -tvec2::axisY*0.5f );
	body->setLinearDrag( 0.05f );

	SWGameObject* poolGO = new SWGameObject;
	poolGO->setName( "Pool" );
	poolGO->getComponent<SWTransform>()->setParent( getComponent<SWTransform>() );

	m_atlas = SWAssets.loadSpriteAtlas( "textures/cloud.png" );
}

void Shrapnel::onStart()
{
}

void Shrapnel::onFixedRateUpdate()
{
	if ( --m_delay > 0 ) return;
	m_delay = 5;

	SWTransform* dirst = getComponent<SWTransform>()->find("Pool/Dirst");
	if ( dirst == NULL )
	{
		SWGameObject* go = new SWGameObject;
		go->setName( "Dirst" );

		SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
		renderer->setSprite( m_atlas()->find("cloud_0") );

		dirst = go->getComponent<SWTransform>();
		dirst->setParent( getComponent<SWTransform>() );

		SWRigidBody2D* body = go->addComponent<SWRigidBody2D>();
		body->setGravityScale( tvec2::zero );

		SWAction* action = go->addComponent<SWAction>();
		SWActBunch* bunch = new SWActBunch( new SWActColorTo(1, tcolor(1,1,1,0)), new SWActScaleTo( 1, tvec3::zero ) );
		action->setAct( "fadeout", new SWActSequence( bunch, new SWActDelegate(GetDelegator(onEndAction)) ) );
	}

	{
		dirst->setLocalScale( tvec3::one * SWMath.randomInt(50,100)/100.0f );
		dirst->setParent( getComponent<SWTransform>() );

		SWAction* action = dirst->getComponent<SWAction>();
		action->play( "fadeout" );

		SWSpriteRenderer* renderer = dirst->getComponent<SWSpriteRenderer>();
		renderer->setColor( tcolor(1,1,1,1) );

		SWRigidBody2D* body = dirst->getComponent<SWRigidBody2D>();
		body->setPosition( getComponent<SWTransform>()->getPosition().xy() );
	}

}

void Shrapnel::onEndAction( SWActDelegate* act )
{
	SWTransform* trans = act->getAction()->getComponent<SWTransform>();
	trans->setParent( getComponent<SWTransform>()->find("Pool") );
}
