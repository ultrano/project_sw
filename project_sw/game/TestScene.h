#ifndef TestScene_h__
#define TestScene_h__

#include "SWHeaders.h"
#include "SWShape2D.h"
#include "SWBroadPhase2D.h"

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
		bool isBox = 0;
		if ( isBox ) sprite = atlas()->find( "box" );
		else sprite = atlas()->find( "circle" );
		tvec2 logoSize = sprite->getSize();

		SWGameObject* go = gameObject();

		SWRigidBody2D* body = go->addComponent<SWRigidBody2D>();
		//body->setGravityScale(tvec2::zero);
		//body->setGravityScale( tvec2::axisY* -3 );
		//body->setRotate( SWMath.angleToRadian(45) );
		body->setRotate( SWMath.angleToRadian(45) );

		SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
		renderer->setSprite( sprite );

		SWCollider2D* collider = go->addComponent<SWCollider2D>();
		if ( isBox )
		{
			collider->addBox( tvec2::zero, logoSize.x, logoSize.y );
			//collider->addBox( tvec2::zero, 0.5f, 0.5f );
		}
		else collider->addCircle( tvec2::zero, logoSize.x/2 );

		SWTransform* trans = go->getComponent<SWTransform>();
		trans->setLocalScale( tvec3::one*2 );
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

		float speed = 5;
		SWRigidBody2D* body = go->getComponent<SWRigidBody2D>();
		SWTransform* trans = go->getComponent<SWTransform>();
		if ( SWInput.getKey('d') )
		{
			body->addForce(  (tvec2::axisX*speed) );
		} 
		if ( SWInput.getKey('a') )
		{
			body->addForce( -(tvec2::axisX*speed) );
		}
		if ( SWInput.getKey('w') )
		{
			body->addForce( (tvec2::axisY*speed) );
		}
		if ( SWInput.getKey('s') )
		{
			body->addForce( -(tvec2::axisY*speed) );
		}
		if ( SWInput.getKey('z') )
		{
			body->setAngularVelocity( -SWMath.pi/20.0f );
		}
		if ( SWInput.getKey('c') )
		{
			body->setAngularVelocity( SWMath.pi/20.0f );
		}
		if ( SWInput.getKey(' ') && m_canJump )
		{
			body->addForce( (tvec2::axisY*20) );
		}
		if ( SWInput.getKey('r') )
		{
			tvec3 scale = trans->getLocalScale();
			trans->setLocalScale( scale + tvec3::one );
		}
		if ( SWInput.getKey('f') )
		{
			tvec3 scale = trans->getLocalScale();
			trans->setLocalScale( scale - tvec3::one );
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
		screenSize *= 3;

		//! set default camera
		{
			SWGameObject* go = new SWGameObject;
			go->setName( "Camera" );

			SWCamera* cam = go->addComponent<SWCamera>();
			cam->orthoMode( screenSize.x, screenSize.y, 1, 1000 );
			cam->getComponent<SWTransform>()->setLocalPosition( tvec3( 0, 0, -500 ) );
			cam->setClearColor( tcolor( 0,0,0,1 ) );
			cam->setClearFlags( SW_Clear_Color );
		}

		SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "test.png" );

		//! ground
		do
		{
			break;
			SWSprite* sprite = atlas()->find( "box" );
			tvec2 logoSize = sprite->getSize();
			SWGameObject* go = new SWGameObject;
			SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
			renderer->setSprite( sprite );
			renderer->setColor( tcolor( 1,1,1,0.5f ) );

			SWCollider2D* collider = go->addComponent<SWCollider2D>();
			collider->addBox( tvec2::zero, logoSize.x, logoSize.y );

			SWTransform* trans = go->getComponent<SWTransform>();
			trans->setPosition( tvec3(0,-300,0) );
			trans->setLocalScale( tvec3( 100, 10, 1 ) );
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
				float x = SWMath.cos(radian) * 400;
				float y = SWMath.sin(radian) * 400;

				SWTransform* trans = go->getComponent<SWTransform>();
				trans->setPosition( tvec3(x,y,0) );
				trans->setLocalScale( tvec3::one*2 + tvec3(y,x,0).normal()*40 );
			}
		}

		//! 
		{
			bool isBox = false;
			SWSprite* sprite = atlas()->find( "circle" );
			if ( isBox ) sprite = atlas()->find( "box" );
			tvec2 logoSize = sprite->getSize();
			tuint count = 50;
			for ( tuint i = 0 ; i < count ; ++i )
			{
				SWGameObject* go = new SWGameObject;
				SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
				renderer->setSprite( sprite );

				SWRigidBody2D* body = go->addComponent<SWRigidBody2D>();

				SWCollider2D* collider = go->addComponent<SWCollider2D>();
				if ( isBox ) collider->addBox( tvec2::zero, logoSize.x, logoSize.y );
				else collider->addCircle( tvec2::zero, logoSize.x/2 );

				float radian = SWMath.angleToRadian( ((float)i/(float)count) * 360.0f );
				float randomX = (SWMath.randomFloat()-0.5f);
				float randomY = (SWMath.randomFloat()-0.5f);
				float x = randomX * 400;
				float y = randomY * 400;

				SWTransform* trans = go->getComponent<SWTransform>();
				trans->setPosition( tvec3(x,y,0) );
				trans->setLocalScale( tvec3::one*2 );
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