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
}

void Character::onStart()
{
	__super::onStart();
	m_meterScore = SW_GC.getScene()->findGO( "MeterScore" )->getComponent<SWFontRenderer>();
	m_coinScore  = SW_GC.getScene()->findGO( "CoinScore" )->getComponent<SWFontRenderer>();
}

void Character::onUpdate()
{
	if ( SWInput.getKey( 't' ) )
	{
		if ( Runner* runner = getComponent<Runner>() )
		{
			runner->destroy();
			gameObject()->addComponent<Bird>();
		}
		else if ( Bird* bird = getComponent<Bird>() )
		{
			bird->destroy();
			gameObject()->addComponent<Runner>();
		}
	}
}

void Character::onFixedRateUpdate()
{
	SWTransform* trans = getComponent<SWTransform>();
	tuint meters = (tuint)trans->getPosition().x/100;
	m_meterScore()->setText( "%d Meters", meters );

	SWHardRef<SWRigidBody2D> body = getComponent<SWRigidBody2D>();
	body()->addForce( tvec2( RunningForce + ((float)meters/5000),0 ) );

	tvec2 vel = body()->getVelocity();
	tvec2 pos = body()->getPosition();
	if ( pos.y > RoofY && vel.y > 0 )
	{
		vel.y = 0;
		body()->setVelocity( vel );

		pos.y -= (pos.y - RoofY)/2;
		body()->setPosition( pos );
	}
	else if ( pos.y < GroundY && vel.y < 0 )
	{
		tvec2 vel = body()->getVelocity();

		vel.y = 0;
		body()->setVelocity( vel );

		pos.y -= (pos.y - GroundY)/2;
		body()->setPosition( pos );
	}
}

void Character::onCollision( SWCollision2D* coll )
{
	if ( !coll->collider.isValid() ) return;
	
	SWGameObject* go = coll->collider()->gameObject();
	if ( go->getName() == "Coin" )
	{
		m_coinScore()->setText( "%d Coins", ++m_score );
	}
}