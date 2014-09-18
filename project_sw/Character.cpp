#include "Character.h"
#include "GameHeaders.h"

Character::Character( factory_constructor )
	: m_score(0)
{
}

Character::~Character()
{
}

void Character::onAwake()
{
	__super::onAwake();
	gameObject()->addComponent<Runner>();
	gameObject()->setName( "Character" );

	SWHardRef<SWAudioClip> audioClip = NULL;

	audioClip = getAudioClip( "audios/coin_pickup_1.wav");
	m_coinSound[0] = audioClip()->createSource();
	
	audioClip = getAudioClip( "audios/coin_pickup_2.wav");
	m_coinSound[1] = audioClip()->createSource();

	audioClip = getAudioClip( "audios/coin_pickup_3.wav");
	m_coinSound[2] = audioClip()->createSource();

	m_state = None;
}

void Character::onStart()
{
	__super::onStart();
	m_meterScore = SW_GC.getScene()->findGO( "MeterScore" )->getComponent<SWFontRenderer>();
	m_coinScore  = SW_GC.getScene()->findGO( "CoinScore" )->getComponent<SWFontRenderer>();
}

void Character::onUpdate()
{
}

void Character::onFixedRateUpdate()
{
	SWTransform* trans = getComponent<SWTransform>();
	m_meter = (tuint)trans->getPosition().x/100;
	m_meterScore()->setText( "%d Meters", m_meter );

	SWHardRef<SWRigidBody2D> body = getComponent<SWRigidBody2D>();
	if ( !body()->isFixedPosition() ) body()->addForce( tvec2( RunningForce + ((float)m_meter/5000),0 ) );

	updateCondition();
}

void Character::updateCondition()
{
	SWRigidBody2D* body = getComponent<SWRigidBody2D>();
	tvec2 vel = body->getVelocity();
	tvec2 pos = body->getPosition();

	if ( pos.y > RoofY && vel.y > 0 )
	{
		if ( m_state != GotRoof && m_state != UnderRoof )
		{
			m_state = GotRoof;
		}
		else if ( m_state != UnderRoof )
		{
			m_state = UnderRoof;
		}
	}
	else if ( pos.y < GroundY && vel.y < 0 )
	{
		if ( m_state != Landing && m_state != OnGround )
		{
			m_state = Landing;
		}
		else if ( m_state != OnGround )
		{
			m_state = OnGround;
		}
	}
	else if ( (isState(GotRoof) || isState(UnderRoof)) && vel.y < 0  )
	{
		m_state = LeaveRoof;
	}
	else if ( (isState(Landing) || isState(OnGround)) && vel.y > 0  )
	{
		m_state = TakeOff;
	}
	else if ( pos.y > GroundY && pos.y < RoofY )
	{
		m_state = AirBorne;
	}

	if ( isState(GotRoof) || isState(UnderRoof) )
	{
		vel.y = 0;
		body->setVelocity( vel );

		pos.y -= (pos.y - RoofY)/2;   
		body->setPosition( pos );
	}
	else if ( isState(Landing) || isState(OnGround) )
	{
		vel.y = 0;
		body->setVelocity( vel );

		pos.y -= (pos.y - GroundY)/2;
		body->setPosition( pos );
	}
}

void Character::onCollision( SWCollision2D* coll )
{
	if ( !coll->collider.isValid() ) return;
	
	SWGameObject* go = coll->collider()->gameObject();
	if ( go->getName() == "Coin" )
	{
		m_coinScore()->setText( "%d %c", ++m_score, (char)169 );

		tuint index = SWMath.randomInt(0,2);
		m_coinSound[index]()->play();
	}
	else if ( go->getName() == "VehicleItem" )
	{
		gameObject()->removeComponent<Runner>();
		gameObject()->addComponent<Bird>();
	}
}

SWAudioClip* Character::getAudioClip( const tstring& filePath )
{
	SWHardRef<SWAudioClip> audioClip = NULL;

	audioClip = SWAssets.loadAudioClip( filePath );
	if ( !audioClip.isValid() ) return NULL;
	
	m_audioTable[filePath] = audioClip;

	return audioClip();
}
