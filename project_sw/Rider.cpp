#include "Rider.h"
#include "GasCloud.h"
#include "GameValues.h"
#include "Coin.h"

Rider::Rider()
	: m_score( 0 )
{
}

Rider::Rider( factory_constructor )
	: m_score( 0 )
{
}

Rider::~Rider()
{
}

void Rider::onAwake()
{
	__super::onAwake();

	m_imgAtlas = SWAssets.loadSpriteAtlas( "textures/runner.png" );
	m_renderer = gameObject()->addComponent<SWSpriteRenderer>();

	SWHardRef<SWRigidBody2D> body = gameObject()->addComponent<SWRigidBody2D>();
	body()->setGravityScale( -tvec2::axisY * 80 );

	SWHardRef<SWCircleCollider2D> collider = gameObject()->addComponent<SWCircleCollider2D>();
	collider()->setRadius( 20 );

	SWHardRef<SWSpriteAnimation> anim = SWAssets.loadSpriteAnimation( "animations/runner_anim.txt" );
	SWActAnimate* act = new SWActAnimate( 1, anim()->getSequenceAt(0) );

	SWAction* action = gameObject()->addComponent<SWAction>();
	action->setAct( "run", new SWActRepeat( act ) );
	action->play( "run" );

	SWHardRef<SWTransform> trans = getComponent<SWTransform>();
	trans()->setLocalScale( tvec3( 0.3f, 0.3f, 1 ) );

	//! jump effect
	{
		SWGameObject* go = new SWGameObject();
		m_jumpEffect = go;
		SWHardRef<SWSpriteAnimation> aniData = SWAssets.loadSpriteAnimation( "animations/effects_anim.txt" );

		go->addComponent<SWSpriteRenderer>();

		SWAction* action = go->addComponent<SWAction>();
		SWActAnimate* anim = new SWActAnimate( 1.5f, aniData()->getSequenceByName( "jump_effect" ) );
		SWActSequence* seq = new SWActSequence();
		seq->addAct( anim );
		seq->addAct( new SWActDelegate( GetDelegator( collectJumpEffect ) ) );
		action->setAct( "jump_effect", seq );
	}

	m_score = 0;

}

void Rider::onStart()
{
	__super::onStart();
	m_meterScore = SW_GC.getScene()->findGO( "MeterScore" )->getComponent<SWFontRenderer>();
	m_coinScore  = SW_GC.getScene()->findGO( "CoinScore" )->getComponent<SWFontRenderer>();
}

void Rider::onRemove()
{
	__super::onRemove();
}

void Rider::onUpdate()
{
	SWHardRef<SWRigidBody2D> body = getComponent<SWRigidBody2D>();

	tvec2 vel = body()->getVelocity();
	tvec2 pos = body()->getPosition();
	if ( pos.y > RoofY && vel.y > 0 )
	{
		vel.y = 0;
		body()->setVelocity( vel );

		pos.y -= (pos.y - RoofY)/1.5f;
		body()->setPosition( pos );
	}
	else if ( pos.y < GroundY && vel.y < 0 )
	{
		tvec2 vel = body()->getVelocity();

		vel.y = 0;
		body()->setVelocity( vel );

		pos.y -= (pos.y - GroundY)/2;
		body()->setPosition( pos );

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
				
				m_jumpEffect()->setActive( true );
				SWAction* action = m_jumpEffect()->getComponent<SWAction>();
				action->play( "jump_effect" );

				SWTransform* trans = m_jumpEffect()->getComponent<SWTransform>();
				tvec3 pos = getComponent<SWTransform>()->getPosition();
				pos.y = GroundY;
				trans->setPosition( pos );
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
			
			//! checking state
			if ( isActivated ) m_state = Flying;
			else m_state = Gliding;
			
			//! make gas cloud
			{
				SWGameObject* go = SW_GC.getScene()->findGO( "GasCloudPool/GasCloud" );;
				if ( go == NULL ) go = new SWGameObject();

				GasCloud* gas = go->addComponent<GasCloud>();
				tvec3 pos = getComponent<SWTransform>()->getPosition();
				pos.z = -1;
				gas->reset( pos );
			}
		}
		break;
	}

	tuint meters = (tuint)getComponent<SWTransform>()->getPosition().x/100;
	m_meterScore()->setText( "%d Meters", meters );
}

void Rider::onCollision( SWCollision2D* coll )
{
	if ( !coll->collider.isValid() ) return;
	
	SWGameObject* go = coll->collider()->gameObject();
	if ( go->getName() == "Coin" )
	{
		m_coinScore()->setText( "%d Coins", ++m_score );
	}
}

void Rider::collectJumpEffect()
{
	m_jumpEffect()->setActive( false );
}