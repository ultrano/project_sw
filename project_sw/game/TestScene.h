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
	override void onCollisionEnter()
	{
		SWLog(__FUNCTION__);
		m_canJump = true;
	}

	override void onCollisionStay()
	{
	}

	override void onCollisionLeave()
	{
		SWLog(__FUNCTION__);
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
		SWSprite* sprite = atlas()->find( "box" );
		tvec2 logoSize = sprite->getSize();

		SWGameObject* parent = new SWGameObject("p");
		tuint count = 15;
		for ( tuint i = 0 ; i < count ; ++i )
		{
			SWGameObject* go = new SWGameObject;
			SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
			renderer->setSprite( sprite );

			//SWRigidBody2D* body = go->addComponent<SWRigidBody2D>();
			//body->setGravityScale( tvec2::zero );

			SWCollider2D* collider = go->addComponent<SWCollider2D>();
			//collider->addBox( tvec2::zero, logoSize.x, logoSize.y );
			collider->addBox( tvec2(111,222), logoSize.x, logoSize.y );
			//collider->addCircle( tvec2::zero, logoSize.x/2 );

			float radian = SWMath.angleToRadian( ((float)i/(float)count) * 360.0f );
			float randomX = (SWMath.randomFloat()-0.5f);
			float randomY = (SWMath.randomFloat()-0.5f);
			float x = randomX * screenSize.x;
			float y = randomY * screenSize.y;

			SWTransform* trans = go->getComponent<SWTransform>();
			trans->setPosition( tvec3(x,y,0) );
			trans->setLocalScale( tvec3::one + tvec3(15,0,0) );
			//trans->setLocalRotate( tvec3::axisZ * SWMath.angleToRadian(20) );
			trans->setParent( parent->getComponent<SWTransform>() );
		}

		sprite = atlas()->find( "circle" );
		{
			SWGameObject* go = new SWGameObject("test");
			go->addComponent<TestBehavior>();

			SWRigidBody2D* body = go->addComponent<SWRigidBody2D>();
			body->setGravityScale( tvec2::axisY* -3 );
			//body->setRotate( SWMath.angleToRadian(45) );

			SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
			renderer->setSprite( sprite );

			SWCollider2D* collider = go->addComponent<SWCollider2D>();
			//collider->addBox( tvec2::zero, logoSize.x, logoSize.y );
			collider->addCircle( tvec2::zero, logoSize.x/2 );

			SWTransform* trans = go->getComponent<SWTransform>();
			trans->setLocalScale( tvec3(2,2,1) );
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