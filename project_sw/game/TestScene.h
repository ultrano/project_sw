#ifndef TestScene_h__
#define TestScene_h__

#include "SWHeaders.h"
#include "SWShape2D.h"
#include "SWBroadPhase2D.h"
#include "SWFixture2D.h"

class TestBehavior : public SWBehavior
{
	SW_RTTI( TestBehavior, SWBehavior );
	bool m_canJump;
public:
	TestBehavior(factory_constructor)
	{
		m_canJump = true;
	};

	override void onAwake()
	{
		__super::onAwake();

		SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "test.png" );
		SWSprite* sprite = NULL;
		bool isBox = 1;
		if ( isBox ) sprite = atlas()->find( "box" );
		else sprite = atlas()->find( "circle" );
		tvec2 logoSize = sprite->getSize();

		SWGameObject* go = gameObject();

		SWRigidBody2D* body = go->addComponent<SWRigidBody2D>();
		//body->setGravityScale(tvec2::zero);
		//body->setGravityScale( tvec2::axisY* -3 );
		//body->setRotate( SWMath.angleToRadian(45) );
		//body->setMass(1);
		//body->setMass(500);

		SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
		renderer->setSprite( sprite );

		SWCollider2D* collider = go->addComponent<SWCollider2D>();
		SWFixture2D* fixtur = NULL;
		if ( isBox )
		{
			fixtur = collider->addBox( tvec2::zero, logoSize.x, logoSize.y );
		}
		else fixtur = collider->addCircle( tvec2::zero, logoSize.x/2 );
		//fixtur->setFriction(0.5f);

		SWTransform* trans = go->getComponent<SWTransform>();
		trans->setLocalScale( tvec3::one*0.1f );
	}
	override void onCollisionEnter()
	{
		//SWLog(__FUNCTION__);
		m_canJump = true;
	}

	override void onCollisionStay()
	{
	}

	override void onCollisionLeave()
	{
		//SWLog(__FUNCTION__);
		m_canJump = false;
	}

	override void onFixedRateUpdate()
	{
		SWGameObject* go = gameObject();
		if ( !go ) return;

		float force = 500;
		SWRigidBody2D* body = go->getComponent<SWRigidBody2D>();
		SWTransform* trans = go->getComponent<SWTransform>();
		if ( SWInput.getKey('d') )
		{
			body->addForce(  (tvec2::axisX*force) );
		} 
		if ( SWInput.getKey('a') )
		{
			body->addForce( -(tvec2::axisX*force) );
		}
		if ( SWInput.getKey('w') )
		{
			body->addForce( (tvec2::axisY*force) );
		}
		if ( SWInput.getKey('s') )
		{
			body->addForce( -(tvec2::axisY*force) );
		}
		if ( SWInput.getKey('z') )
		{
			body->addTorque( -11100);
		}
		if ( SWInput.getKey('c') )
		{
			body->addTorque( 11100 );
		}
		if ( SWInput.getKey(' ') && m_canJump )
		{
			body->addForce( (tvec2::axisY*force*5) );
		}
		if ( SWInput.getKey('r') )
		{
			tvec3 scale = trans->getLocalScale();
			trans->setLocalScale( scale + tvec3::one*0.5f );
		}
		if ( SWInput.getKey('f') )
		{
			tvec3 scale = trans->getLocalScale();
			trans->setLocalScale( scale - tvec3::one*0.1f );
		}
	}
};


class TestScene : public SWGameScene
{
	SW_RTTI(TestScene,SWGameScene);
public:
	TestScene() {}
	~TestScene() {}

	override void onAwake()
	{
		SW_GC.registerFactory<TestBehavior>();
		tvec3 screenSize( SW_GC.getScreenWidth(), SW_GC.getScreenHeight(), 0 );
		screenSize *= 1;

		//! set default camera
		{
			SWGameObject* go = new SWGameObject;
			go->setName( "Camera" );

			SWCamera* cam = go->addComponent<SWCamera>();
			cam->orthoMode( screenSize.x, screenSize.y, 1, 1000 );
			cam->getComponent<SWTransform>()->setLocalPosition( tvec3( 0, 0, -500 ) );
			cam->setClearColor( tcolor( 0,0,1,1 ) );
			cam->setClearFlags( SW_Clear_Color );
		}

		SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "test.png" );

		//! ground
		do
		{
			SWSprite* sprite = atlas()->find( "box" );
			tvec2 logoSize = sprite->getSize();
			SWGameObject* go = new SWGameObject;
			SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
			renderer->setSprite( sprite );
			renderer->setColor( tcolor( 1,1,1,0.5f ) );

			SWCollider2D* collider = go->addComponent<SWCollider2D>();
			collider->addBox( tvec2::zero, logoSize.x, logoSize.y );

			SWTransform* trans = go->getComponent<SWTransform>();
			trans->setPosition( tvec3(0,-25,0) );
			trans->setLocalScale( tvec3( 100, 1, 1 ) );
		} while(false);

		//! boxes
		{
			SWSprite* sprite = atlas()->find( "box" );
			tvec2 logoSize = sprite->getSize();
			tuint count = 4;
			for ( tuint i = 0 ; i < count ; ++i )
			{
				SWGameObject* go = new SWGameObject;
				SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
				renderer->setSprite( sprite );

				SWCollider2D* collider = go->addComponent<SWCollider2D>();
				collider->addBox( tvec2::zero, logoSize.x, logoSize.y );

				float radian = SWMath.angleToRadian( ((float)i/(float)count) * 360.0f );
				float randomX = (SWMath.randomFloat()-0.5f);
				float randomY = (SWMath.randomFloat()-0.5f);
				float x = SWMath.cos(radian) * 150;
				float y = SWMath.sin(radian) * 150;

				SWTransform* trans = go->getComponent<SWTransform>();
				trans->setPosition( tvec3(x,y,0) );
				trans->setLocalScale( tvec3::one + tvec3(y,x,0).normal()*40 );
			}
		}

		//! 
		{
			bool isBox = 1;
			SWSprite* sprite = atlas()->find( "circle" );
			if ( isBox ) sprite = atlas()->find( "box" );
			tvec2 logoSize = sprite->getSize();
			tuint count = 10;
			for ( tuint i = 0 ; i < count ; ++i )
			{
				SWGameObject* go = new SWGameObject;
				SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
				renderer->setSprite( sprite );

				SWRigidBody2D* body = go->addComponent<SWRigidBody2D>();
				//body->setGravityScale(tvec2::zero);

				SWFixture2D* fixture = NULL;
				SWCollider2D* collider = go->addComponent<SWCollider2D>();
				if ( isBox ) fixture = collider->addBox( tvec2::zero, logoSize.x, logoSize.y );
				else fixture = collider->addCircle( tvec2::zero, logoSize.x/2 );
				//fixture->setFriction( 0.5f );
				 
				float radian = SWMath.angleToRadian( ((float)i/(float)count) * 360.0f );
				float randomX = (SWMath.randomFloat()-0.5f);
				float randomY = (SWMath.randomFloat()-0.5f);
				float x = randomX * screenSize.x/3;
				float y = randomY * screenSize.y/3;

				SWTransform* trans = go->getComponent<SWTransform>();
				trans->setPosition( tvec3(x,y,0) );
				trans->setLocalScale( tvec3::one*1.1f );
				trans->setLocalRotate( tvec3::axisZ * SWMath.angleToRadian(45) );
				//trans->setLocalScale( tvec3::one*2 + tvec3(y,x,0).normal()*20 );
			}
		}
		
		{
			SWGameObject* go = new SWGameObject("test");
			go->addComponent<TestBehavior>();
		}
	}

	override void onFixedRateUpdate()
	{
		if ( SWInput.getKey('p') )
		{
			SW_GC.setNextScene( new TestScene );
		}
	}
};

#endif // TestScene_h__