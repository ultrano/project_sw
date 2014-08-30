#include "Rider.h"
#include "GasCloud.h"
#include "GameValues.h"

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

	m_imgAtlas = SWAssets.loadSpriteAtlas( "runner.png" );
	m_renderer = gameObject()->addComponent<SWSpriteRenderer>();

	SWHardRef<SWRigidBody2D> body = gameObject()->addComponent<SWRigidBody2D>();
	body()->setGravityScale( -tvec2::axisY * 30 );

	SWHardRef<SWCircleCollider2D> collider = gameObject()->addComponent<SWCircleCollider2D>();
	collider()->setRadius( 20 );

	SWHardRef<SWSpriteAnimation> anim = SWAssets.loadSpriteAnimation( "runner_anim.txt" );
	SWActAnimate* act = new SWActAnimate( 1, anim()->getSequenceAt(0) );

	SWAction* action = gameObject()->addComponent<SWAction>();
	action->setAct( "run", new SWActRepeat( act ) );
	action->play( "run" );

	SWHardRef<SWTransform> trans = getComponent<SWTransform>();
	trans()->setLocalScale( tvec3( 0.3f, 0.3f, 1 ) );
}

void Rider::onRemove()
{
	__super::onRemove();
}

void Rider::onUpdate()
{
	SWHardRef<SWRigidBody2D> body = getComponent<SWRigidBody2D>();

	tvec2 vel = body()->getVelocity();
	tvec2 pos = body()->getCenter();
	if ( pos.y > RoofY && vel.y > 0 )
	{
		vel.y = 0;
		body()->setVelocity( vel );

		pos.y -= (pos.y - RoofY)/1.5f;
		body()->setCenter( pos );
	}
	else if ( pos.y < GroundY && vel.y < 0 )
	{
		tvec2 vel = body()->getVelocity();

		vel.y = 0;
		body()->setVelocity( vel );

		pos.y -= (pos.y - GroundY)/2;
		body()->setCenter( pos );

		if ( m_state != Running )
		{
			m_state = Running;
			SWAction* action = getComponent<SWAction>();
			action->play( "run" );
		}
	}

}

void Rider::onFixedRateUpdate()
{

	SWHardRef<SWRigidBody2D> body = getComponent<SWRigidBody2D>();
	body()->addForce( tvec2( RunningForce,0 ) );

	bool isActivated = SWInput.getKey( ' ' );
	if ( isActivated )
	{
		SWAction* action = getComponent<SWAction>();
		if ( action->isPlaying() ) action->stop();
	}

	switch ( m_state )
	{
	case State::Running :
		{
			//SWLog( "Running" );
			if ( isActivated )
			{
				body()->addForce( tvec2( 0, JumpForce ) );
				m_state = Gliding;
			}
		}
		break;

	case  State::Gliding :
		{
			m_renderer()->setSprite( m_imgAtlas()->find( "jump_0" ) );
			//SWLog( "Gliding" );
			const tvec2& vel = body()->getVelocity();
			if ( vel.y < 50 && isActivated ) m_state = Flying;
		}
		break;

	case State::Flying :
		{
			m_renderer()->setSprite( m_imgAtlas()->find( "jump_0" ) );
			//SWLog( "Flying" );
			body()->addForce( tvec2( 0, BoostForce ) );
			
			if ( isActivated ) m_state = Flying;
			else m_state = Gliding;
			
			SWGameObject* go = new SWGameObject();
			go->addComponent<GasCloud>();
			tvec3 pos = getComponent<SWTransform>()->getPosition();
			pos.z = -1;
			go->getComponent<SWTransform>()->setPosition( pos );
			go->addComponent<SWRigidBody2D>()->addForce( tvec2( SWMath.randomInt( -100,100 )/20.0f, -30 ) );
		}
		break;
	}
}

void Rider::onCollision( SWCollision2D* )
{
	SWLog( "Hit Something!" );
}