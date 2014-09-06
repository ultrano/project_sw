#include "CharacterState.h"
#include "GameValues.h"
#include "GameHeaders.h"

Runner::Runner( factory_constructor )
{
}

Runner::~Runner()
{
}

void Runner::onAwake()
{
	__super::onAwake();
	
	m_state = State::Running;
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
		seq->addAct( new SWActDelegate( GetDelegator( inactivate ) ) );
		action->setAct( "jump_effect", seq );
	}
}

void Runner::onRemove()
{
	__super::onRemove();

	m_jumpEffect()->destroy();
}

void Runner::onUpdate()
{
	SWHardRef<SWRigidBody2D> body = getComponent<SWRigidBody2D>();

	if ( m_state != Running )
	{
		tvec2 vel = body()->getVelocity();
		tvec2 pos = body()->getPosition();
		if ( pos.y <= GroundY && vel.y <= 0 )
		{
			m_state = Running;
			SWAction* action = getComponent<SWAction>();
			action->play( "run" );
		}
	}
}

void Runner::onFixedRateUpdate()
{
	SWHardRef<SWRigidBody2D> body = getComponent<SWRigidBody2D>();

	bool isActivated = isButtonPushed();
	if ( isActivated )
	{
		SWAction* action = getComponent<SWAction>();
		if ( action->isPlaying() ) action->stop();
	}

	switch ( m_state )
	{
	case State::Running :
		{
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
			const tvec2& vel = body()->getVelocity();
			if ( vel.y < 50 && isActivated ) m_state = Flying;
		}
		break;

	case State::Flying :
		{
			m_renderer()->setSprite( m_imgAtlas()->find( "jump_0" ) );
			body()->addForce( tvec2( 0, BoostForce ) );
			
			//! checking state
			if ( isActivated ) m_state = Flying;
			else m_state = Gliding;
			
			//! make gas cloud
			{
				SWGameObject* go = SW_GC.getScene()->findGO( "Pool/GasCloud" );;
				if ( go == NULL ) go = new SWGameObject();

				GasCloud* gas = go->addComponent<GasCloud>();
				tvec3 pos = getComponent<SWTransform>()->getPosition();
				pos.z = -1;
				gas->reset( pos );
			}
		}
		break;
	}

}

void Runner::onCollision( SWCollision2D* )
{
}

void Runner::inactivate( SWActDelegate* del )
{
	del->getAction()->gameObject()->setActive( false );
	//go->setActive( false );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////


Bird::Bird( factory_constructor )
{
}

Bird::~Bird()
{
}

void Bird::onAwake()
{
	__super::onAwake();

	gameObject()->addComponent<SWSpriteRenderer>();

	SWHardRef<SWRigidBody2D> body = gameObject()->addComponent<SWRigidBody2D>();
	body()->setGravityScale( -tvec2::axisY * 80 );

	SWHardRef<SWCircleCollider2D> collider = gameObject()->addComponent<SWCircleCollider2D>();
	collider()->setRadius( 20 );

	SWHardRef<SWSpriteAnimation> anim = SWAssets.loadSpriteAnimation( "animations/flappy_bird_anim.txt" );
	SWActAnimate* act = new SWActAnimate( 1.5f, anim()->getSequenceAt(0) );

	SWAction* action = gameObject()->addComponent<SWAction>();
	action->setAct( "flapping", act );
	action->play( "flapping" );

	SWHardRef<SWTransform> trans = getComponent<SWTransform>();
	trans()->setLocalScale( tvec3( 2, 2, 1 ) );
}

void Bird::onRemove()
{
	__super::onRemove();
}

void Bird::onUpdate()
{
	SWRigidBody2D* body = getComponent<SWRigidBody2D>();
	SWTransform* trans = getComponent<SWTransform>();

	trans->setLocalRotate( tvec3( 0, 0, body->getVelocity().y/900 ) );
}

void Bird::onFixedRateUpdate()
{
	SWRigidBody2D* body = getComponent<SWRigidBody2D>();
	SWTransform* trans = getComponent<SWTransform>();

	if ( isButtonPushed() )
	{
		body->addForce( tvec2( 0, 200 ) );

		SWAction* action = gameObject()->addComponent<SWAction>();
		action->play( "flapping" );
	}

	SWGameObject* coins = SW_GC.getScene()->findGO( "Coins" );
	SWTransform* parent = coins->getComponent<SWTransform>();

	tuint count = parent->getChildrenCount();
	while ( count-- )
	{
		SWTransform* child = parent->getChildAt( count );
		tvec3 pos = child->getPosition();
		tvec3 delta = trans->getPosition() - pos;
		pos += (delta)/10.0f;
		pos += delta.normal() * 5;
		if ( delta.xy().length() < 100 ) child->setPosition( pos );
	}

}

void Bird::onCollision( SWCollision2D* )
{
	gameObject()->addComponent<Runner>();
	destroy();
}