#include "CharacterState.h"
#include "GameValues.h"
#include "GameHeaders.h"

Runner::Runner( factory_constructor )
	: m_activate( false )
{
}

Runner::~Runner()
{
}

void Runner::onStart()
{
	__super::onStart();
	
	m_state = State::None;
	m_newState = State::None;
	m_isChanged = false;
	setState( Running );

	m_imgAtlas = SWAssets.loadSpriteAtlas( "textures/runner.png" );
	m_renderer = gameObject()->addComponent<SWSpriteRenderer>();

	m_body = gameObject()->addComponent<SWRigidBody2D>();
	m_body()->setGravityScale( -tvec2::axisY * 80 );
	m_body()->setFixedAngle( true );
	m_body()->setVelocity( m_body()->getVelocity().scale( 1,0 ) );

	SWCircleCollider2D* collider = gameObject()->addComponent<SWCircleCollider2D>();
	collider->setRadius( 20 );

	SWHardRef<SWSpriteAnimation> anim = SWAssets.loadSpriteAnimation( "animations/runner_anim.txt" );
	SWActAnimate* act = new SWActAnimate( 1, anim()->getSequenceAt(0) );

	SWAction* action = gameObject()->addComponent<SWAction>();
	action->setAct( "run", new SWActRepeat( act ) );
	action->play( "run" );

	SWTransform* trans = getComponent<SWTransform>();
	trans->setLocalScale( tvec3( 0.3f, 0.3f, 1 ) );
	trans->setRotate( tquat( tvec3::zero, 0 ));

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
	if ( m_jumpEffect.isValid() ) m_jumpEffect()->destroy();

	__super::onRemove();
}

void Runner::onUpdate()
{
}

void Runner::onFixedRateUpdate()
{
	m_activate = isButtonPushed();

	changeState();
	updateState();
}

void Runner::updateState()
{
	//! update state
	switch ( getState() )
	{
	case State::Running :
		{
			tvec2 pos = m_body()->getPosition();
			if ( pos.y > GroundY && pos.y < RoofY ) setState( Gliding );

			if ( m_activate ) setState( Jumping );
		}
		break;
	case State::Jumping :
		{
			tvec2 vel = m_body()->getVelocity();
			tvec2 pos = m_body()->getPosition();
			if ( pos.y <= GroundY && vel.y <= 0 ) setState( Running );
			if ( pos.y > GroundY && pos.y < RoofY ) setState( Gliding );
		}
		break;
	case  State::Gliding :
		{
			tvec2 vel = m_body()->getVelocity();
			tvec2 pos = m_body()->getPosition();
			if ( pos.y <= GroundY && vel.y <= 0 ) setState( Running );
			if ( m_activate && vel.y <= 50 ) setState( Boosting );
		}
		break;
	case State::Boosting :
		{
			tvec2 vel = m_body()->getVelocity();
			tvec2 pos = m_body()->getPosition();
			if ( pos.y <= GroundY && vel.y <= 0 ) setState( Running );
			if ( pos.y > GroundY && pos.y < RoofY ) setState( Gliding );
			if ( m_activate ) setState( Boosting );

			m_body()->addForce( tvec2( 0, BoostForce ) );

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

void Runner::changeState()
{
	if ( !m_isChanged ) return;
	m_isChanged = false;

	State newState = m_newState;

	//! pause state
	switch ( getState() )
	{
	case State::Running :
		{
			tvec2 pos = m_body()->getPosition();
			pos.y = GroundY;
			m_body()->setPosition( pos );

			SWAction* action = getComponent<SWAction>();
			action->stop();
		}
		break;
	case State::Jumping :break;
	case  State::Gliding :break;
	case State::Boosting : break;
		
	}

	m_state = newState;

	//! resume state
	switch ( getState() )
	{
	case State::Running :
		{
			SWAction* action = getComponent<SWAction>();
			action->play( "run" );
		}
		break;
	case State::Jumping :
		{
			//! jump effect
			{
				SWTransform* trans = m_jumpEffect()->getComponent<SWTransform>();
				tvec3 pos = getComponent<SWTransform>()->getPosition();
				pos.y = GroundY;
				trans->setPosition( pos );

				m_jumpEffect()->setActive( true );
				SWAction* action = m_jumpEffect()->getComponent<SWAction>();
				action->play( "jump_effect" );
			}

			m_renderer()->setSprite( m_imgAtlas()->find( "jump_0" ) );
			m_body()->addForce( tvec2( 0, JumpForce ) );
		}
		break;
	case  State::Gliding :
		{
			m_renderer()->setSprite( m_imgAtlas()->find( "jump_0" ) );
		}
		break;
	case State::Boosting :
		{
			m_renderer()->setSprite( m_imgAtlas()->find( "jump_0" ) );
			m_body()->addForce( tvec2( 0, BoostForce ) );
		}
		break;
	}
}

void Runner::setState( State state )
{
	m_isChanged = true;
	m_newState = state;
}

Runner::State Runner::getState() const
{
	return m_state;
}

void Runner::onCollision( SWCollision2D* )
{
}

void Runner::inactivate( SWActDelegate* del )
{
	del->getAction()->gameObject()->setActive( false );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////


Bird::Bird( factory_constructor )
	: m_doFlapping( false )
	, m_wasPressed( false )
{
}

Bird::~Bird()
{
}

void Bird::onStart()
{
	__super::onStart();

	//! components
	{
		gameObject()->addComponent<SWSpriteRenderer>();

		SWRigidBody2D* body = gameObject()->addComponent<SWRigidBody2D>();
		body->setGravityScale( -tvec2::axisY * 50 );
		body->setFixedAngle( true );
		body->setVelocity( body->getVelocity().scale( 1,0 ) );

		SWCircleCollider2D* collider = gameObject()->addComponent<SWCircleCollider2D>();
		collider->setRadius( 20 );

		SWHardRef<SWSpriteAnimation> anim = SWAssets.loadSpriteAnimation( "animations/flappy_bird_anim.txt" );
		SWActAnimate* act = new SWActAnimate( 1.5f, anim()->getSequenceAt(0) );

		SWAction* action = gameObject()->addComponent<SWAction>();
		action->setAct( "flapping", act );
		action->play( "flapping" );

		SWTransform* trans = getComponent<SWTransform>();
		trans->setLocalScale( tvec3( 2, 2, 1 ) );
	}

	//! magnetic effect
	{
		SWGameObject* go = new SWGameObject;
		go->setName( "magnetic" );

		SWTransform* trans = go->getComponent<SWTransform>();
		trans->setParent( getComponent<SWTransform>() );
		trans->setLocalScale( tvec3( 0,0,0 ) );

		SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "textures/effects.png" );

		SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
		renderer->setSprite( atlas()->find( "circle_0" ) );
		renderer->setColor( tcolor( 1,1,1,0 ) );

		SWAction* action = go->addComponent<SWAction>();
		SWActBunch* bunch1 = new SWActBunch();
		bunch1->addAct( new SWActScaleFrom( 0.5f, tvec3( 1,1,1 ) ) );
		bunch1->addAct( new SWActColorFrom( 0.5f, tcolor( 1,1,1,1 ) ) );
		action->setAct( "magnetic", bunch1 );
	}
}

void Bird::onRemove()
{
	__super::onRemove();
	SWTransform* trans = getComponent<SWTransform>();
	SWTransform* magneticTrans = trans->find( "magnetic" );
	if ( magneticTrans )
	{
		magneticTrans->gameObject()->destroy();
	}
}

void Bird::onUpdate()
{
	SWRigidBody2D* body = getComponent<SWRigidBody2D>();
	SWTransform* trans = getComponent<SWTransform>();

	trans->setLocalRotate( tvec3( 0, 0, body->getVelocity().y/900 ) );

	if ( isButtonPushed() )
	{
		if ( !m_wasPressed )
		{
			m_wasPressed = true;
			m_doFlapping = true;
		}
	}
	else
	{
		m_wasPressed = false;
	}
}

void Bird::onFixedRateUpdate()
{
	SWRigidBody2D* body = getComponent<SWRigidBody2D>();
	SWTransform* trans = getComponent<SWTransform>();

	if ( m_doFlapping )
	{
		m_doFlapping = false;
		body->addForce( tvec2( 0, 300 ) );

		SWAction* action = gameObject()->addComponent<SWAction>();
		action->play( "flapping" );
	}

	SWGameObject* coins = SW_GC.getScene()->findGO( "Coins" );
	SWTransform* parent = coins->getComponent<SWTransform>();

	bool playEffect = false;
	tuint count = parent->getChildrenCount();
	while ( count-- )
	{
		SWTransform* child = parent->getChildAt( count );
		tvec3 pos = child->getPosition();
		tvec3 delta = trans->getPosition() - pos;
		//pos += (delta)/10.0f;
		pos += delta.normal() * 5;
		if ( delta.xy().length() < 100 )
		{
			child->setPosition( pos );
			if ( playEffect == false ) playEffect = true;
		}
	}

	if ( playEffect )
	{
		SWAction* action = trans->find( "magnetic" )->getComponent<SWAction>();
		if ( !action->isPlaying() ) action->play( "magnetic" );
	}

}

void Bird::onCollision( SWCollision2D* )
{
}