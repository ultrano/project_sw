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
		SWLog( "onCollisionStay" );
	}

	override void onCollisionLeave()
	{
		SWLog( "onCollisionLeave" );
		while (true)
		{
			SWGameObject* go = SW_GC.getScene()->findGO("test");
			if ( !go ) break;
			go->destroyNow();
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
		//! set default camera
		{
			tvec3 screenSize( SW_GC.getScreenWidth(), SW_GC.getScreenHeight(), 0 );

			SWGameObject* go = new SWGameObject;
			go->setName( "Camera" );

			SWCamera* cam = go->addComponent<SWCamera>();
			cam->orthoMode( screenSize.x, screenSize.y, 1, 1000 );
			cam->getComponent<SWTransform>()->setLocalPosition( tvec3( 0, 0, -500 ) );
			cam->setClearColor( tcolor( 0,0,0,1 ) );
			cam->setClearFlags( SW_Clear_Color );
		}

		{
			SWGameObject* go = new SWGameObject("test");
			SWCollider2D* collider = go->addComponent<SWCollider2D>();
			collider->addCircle( tvec2::zero, 10 );
		}

		{
			SWGameObject* go = new SWGameObject("test");
			go->addComponent<TestBehavior>();
			SWCollider2D* collider = go->addComponent<SWCollider2D>();
			collider->addCircle( tvec2::zero, 10 );
		}
	}

	override void onFixedRateUpdate()
	{

		SWGameObject* go = findGO("test");
		if ( !go ) return;

		SWTransform* trans = go->getComponent<SWTransform>();
		if ( SWInput.getKey('d') )
		{
			trans->setPosition( trans->getPosition() + tvec3::axisX );
		} 
		if ( SWInput.getKey('a') )
		{
			trans->setPosition( trans->getPosition() - tvec3::axisX );
		}
		if ( SWInput.getKey('w') )
		{
			trans->setPosition( trans->getPosition() + tvec3::axisY );
		}
		if ( SWInput.getKey('s') )
		{
			trans->setPosition( trans->getPosition() - tvec3::axisY );
		}
	}

	void onTest()
	{
		if ( SWInput.getKey('x') )
		{
			while ( SWGameObject* go = findGO("p") ) go->destroyNow();
		}
	}
};

#endif // TestScene_h__