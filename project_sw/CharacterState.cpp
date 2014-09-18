#include "CharacterState.h"
#include "GameValues.h"
#include "GameHeaders.h"
#include "ResultScene.h"

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

	m_boostDelayFrame = 0;
	m_imgAtlas = SWAssets.loadSpriteAtlas( "textures/runner.png" );
	m_renderer = gameObject()->addComponent<SWSpriteRenderer>();
	m_renderer()->setSprite( m_imgAtlas()->find( "jump_0" ) );

	m_body = gameObject()->addComponent<SWRigidBody2D>();
	m_body()->setFixedAngle( true );
	m_body()->setVelocity( m_body()->getVelocity().scale( 1,0 ) );
	m_body()->setLinearDrag(0.01f);

	SWCircleCollider2D* collider = gameObject()->addComponent<SWCircleCollider2D>();
	collider->setRadius( 20 );

	SWHardRef<SWSpriteAnimation> anim = SWAssets.loadSpriteAnimation( "animations/runner_anim.txt" );
	SWActAnimate* act = new SWActAnimate( 1, anim()->getSequenceAt(0) );

	SWAction* action = gameObject()->addComponent<SWAction>();
	action->setAct( "run", new SWActRepeat( act ) );

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

	//! load sound
	{
		Character* character = getComponent<Character>();
		SWHardRef<SWAudioClip> audioClip = NULL;

		audioClip = character->getAudioClip( "audios/boots_jump.wav");
		m_jumpSound = audioClip()->createSource();

		audioClip = character->getAudioClip( "audios/boots_land.wav");
		m_landSound = audioClip()->createSource();

		audioClip = character->getAudioClip( "audios/jetpack_steam_lp.wav");
		m_steamSound = audioClip()->createSource();
		m_steamSound()->setLooping( true );

		audioClip = character->getAudioClip( "audios/player_bones.wav");
		m_shockSound = audioClip()->createSource();

		audioClip = character->getAudioClip( "audios/boots_step_left_1.wav");
		m_stepLSound[0] = audioClip()->createSource();
		audioClip = character->getAudioClip( "audios/boots_step_left_2.wav");
		m_stepLSound[1] = audioClip()->createSource();
		audioClip = character->getAudioClip( "audios/boots_step_left_3.wav");
		m_stepLSound[2] = audioClip()->createSource();

		audioClip = character->getAudioClip( "audios/boots_step_right_1.wav");
		m_stepRSound[0] = audioClip()->createSource();
		audioClip = character->getAudioClip( "audios/boots_step_right_2.wav");
		m_stepRSound[1] = audioClip()->createSource();
		audioClip = character->getAudioClip( "audios/boots_step_right_3.wav");
		m_stepRSound[2] = audioClip()->createSource();

		m_stepSound = m_stepLSound[0]();
		m_whichStep = false;
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
	tuint state = getComponent<Character>()->getState();

	m_activate = isButtonPushed();

	switch ( state )
	{
	case Character::Landing :
		{
			SWAction* action = getComponent<SWAction>();
			action->play( "run" );
			m_landSound()->play();
		}
		break;
	case Character::OnGround :
		{
			SWAction* action = getComponent<SWAction>();
			if ( !action->isPlaying() ) action->play("run");

			if ( m_activate )
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

				m_body()->addForce( tvec2( 0, JumpForce ) );
				m_jumpSound()->play();
			}

			if ( !m_stepSound->isPlaying() )
			{
				tuint index = (tuint)SWMath.repeat( SWTime.getTime(), 2.9f );
				m_whichStep = !m_whichStep;
				if ( m_whichStep )
				{
					m_stepSound = m_stepRSound[index]();
				}
				else
				{
					m_stepSound = m_stepLSound[index]();
				}
				m_stepSound->play();
			}
		}
		break;
	case Character::TakeOff :
		{
			m_boostDelayFrame = 10;
		}
		break;
	case Character::AirBorne :
		{
			SWAction* action = getComponent<SWAction>();
			action->stop();
			m_renderer()->setSprite( m_imgAtlas()->find( "jump_0" ) );

			m_boostDelayFrame -= 1;

			if ( m_activate && m_boostDelayFrame <= 0 )
			{
				m_boostDelayFrame = 0;
				if ( !m_steamSound()->isPlaying() ) m_steamSound()->play();

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
			else
			{
				m_steamSound()->stop();
			}
		}
		break;
	}
}

void Runner::onCollision( SWCollision2D* coll )
{
	if ( !coll->collider.isValid() ) return;

	SWGameObject* go = coll->collider()->gameObject();
	if ( go->getName() == "Obstacle" )
	{
		ResultScene* scene = new ResultScene( 111, 111 );
		SW_GC.setNextScene( scene );
	}
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
		//body->setGravityScale( -tvec2::axisY * 50 );
		body->setFixedAngle( true );
		body->setVelocity( body->getVelocity().scale( 1,0 ) );
		body->setLinearDrag(0.01f);

		SWCircleCollider2D* collider = gameObject()->addComponent<SWCircleCollider2D>();
		collider->setRadius( 10 );

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

	//! load flapping sound
	{
		Character* character = getComponent<Character>();
		SWHardRef<SWAudioClip> audioClip = NULL;

		audioClip = character->getAudioClip( "audios/bird_flap_1.wav");
		m_flapSound[0] = audioClip()->createSource();

		audioClip = character->getAudioClip( "audios/bird_flap_2.wav");
		m_flapSound[1] = audioClip()->createSource();

		audioClip = character->getAudioClip( "audios/bird_flap_3.wav");
		m_flapSound[2] = audioClip()->createSource();

		audioClip = character->getAudioClip( "audios/bird_land.wav");
		m_landSound = audioClip()->createSource();

		audioClip = character->getAudioClip( "audios/bird_slidelp.wav");
		m_slideSound = audioClip()->createSource();
		m_slideSound()->setLooping( true );
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

	trans->setLocalRotate( tvec3( 0, 0, body->getVelocity().y/20 ) );
	//trans->setLocalRotate( tvec3( 0, 0, SWMath.pingPong( SWTime.getTime(), SWMath.pi*2 ) ) );

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

	if ( body->isFixedPosition() ) return;

	if ( m_doFlapping )
	{
		m_doFlapping = false;
		body->addForce( tvec2( 0, 15 ) );

		SWAction* action = gameObject()->addComponent<SWAction>();
		action->play( "flapping" );

		tuint index = SWMath.randomInt(0,2);
		m_flapSound[index]()->play();
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
		
		if ( delta.xy().length() >= 100 ) continue;
		
		pos += (delta)/10.0f;
		pos += delta.normal()*body->getVelocity().length();
		child->setPosition( pos );

		if ( playEffect == false ) playEffect = true;
	}

	if ( playEffect )
	{
		SWAction* action = trans->find( "magnetic" )->getComponent<SWAction>();
		if ( !action->isPlaying() ) action->play( "magnetic" );
	}

	tuint state = getComponent<Character>()->getState();
	switch ( state )
	{
	case Character::Landing :
		{
			m_landSound()->play();
			m_slideSound()->play();
		}
		break;
	case Character::OnGround :
		{
			if ( !m_slideSound()->isPlaying() ) m_slideSound()->play();
		}
		break;
	case Character::TakeOff :
		{
			m_slideSound()->stop();
		}
		break;
	}
}

void Bird::onCollision( SWCollision2D* coll )
{
	SWGameObject* go = coll->collider()->gameObject();

	if ( go->getName() == "Obstacle" )
	{
		SWGameObject* bombGO = SW_GC.getScene()->findGO("Bomb");
		if ( bombGO == NULL ) bombGO = new SWGameObject;
		Bomb* bomb = bombGO->addComponent<Bomb>();
		bomb->reset( getComponent<SWTransform>()->getPosition() );
	}

}