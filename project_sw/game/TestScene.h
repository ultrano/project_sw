#ifndef TestScene_h__
#define TestScene_h__

#include "SWHeaders.h"
#include "SWShape2D.h"
#include "SWBroadPhase2D.h"

class TestBehavior : public SWBehavior
{
	SW_RTTI( TestBehavior, SWBehavior );

public:
	TestBehavior(factory_constructor) {};
	override void onCollisionEnter()
	{
		SWLog( "onCollisionEnter" );
	}

	override void onCollisionStay()
	{
		//SWLog( "onCollisionStay" );
	}

	override void onCollisionLeave()
	{
		SWLog( "onCollisionLeave" );
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
		//! set default camera
		{
			tvec3 screenSize( SW_GC.getScreenWidth(), SW_GC.getScreenHeight(), 0 );
			//screenSize *= 3;
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
		tuint count = 10;
		float radius = 5*count;
		for ( tuint i = 0 ; i < count ; ++i )
		{
			SWGameObject* go = new SWGameObject;
			go->addComponent<TestBehavior>();
			SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
			renderer->setSprite( sprite );

			SWCollider2D* collider = go->addComponent<SWCollider2D>();
			collider->addBox( tvec2::zero, logoSize.x, logoSize.y );
			//collider->addCircle( tvec2::zero, logoSize.length()/2 );

			float radian = SWMath.angleToRadian( ((float)i/(float)count) * 360.0f );
			float x = radius * SWMath.cos( radian );
			float y = radius * SWMath.sin( radian );

			SWTransform* trans = go->getComponent<SWTransform>();
			trans->setPosition( tvec3(x,y,0) );
			trans->setParent( parent->getComponent<SWTransform>() );
		}

		sprite = atlas()->find( "circle" );
		{
			SWGameObject* go = new SWGameObject("test");
			SWRigidBody2D* body = go->addComponent<SWRigidBody2D>();
			body->setGravityScale( tvec2::zero );
			body->setRotate( SWMath.angleToRadian(45) );

			SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
			renderer->setSprite( sprite );

			SWCollider2D* collider = go->addComponent<SWCollider2D>();
			//collider->addBox( tvec2::zero, logoSize.x, logoSize.y );
			collider->addCircle( tvec2::zero, logoSize.x/2 );

			SWTransform* trans = go->getComponent<SWTransform>();
			trans->setLocalScale( tvec3(3,1,1) );
		}
	}

	override void onFixedRateUpdate()
	{
		if ( SWInput.getKey('p') )
		{
			SW_GC.setNextScene( new TestScene );
		}

		SWGameObject* go = findGO("test");
		if ( !go ) return;

		float speed = 1;
		SWRigidBody2D* body = go->getComponent<SWRigidBody2D>();
		if ( SWInput.getKey('d') )
		{
			body->setPosition( body->getPosition() + (tvec2::axisX*speed) );
		} 
		if ( SWInput.getKey('a') )
		{
			body->setPosition( body->getPosition() - (tvec2::axisX*speed) );
		}
		if ( SWInput.getKey('w') )
		{
			body->setPosition( body->getPosition() + (tvec2::axisY*speed) );
		}
		if ( SWInput.getKey('s') )
		{
			body->setPosition( body->getPosition() - (tvec2::axisY*speed) );
		}
	}
};

#endif // TestScene_h__