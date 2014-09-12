#ifndef PhysicsTestScene_h__
#define PhysicsTestScene_h__

#include "SWHeaders.h"
#include "GameHeaders.h"

class PhysicsTestScene : public SWGameScene
{
	SW_RTTI( PhysicsTestScene, SWGameScene );
public:
	
	float m_startTIme;

	PhysicsTestScene()
	{
	};

	PhysicsTestScene(factory_constructor)
	{
	};

	void onAwake()
	{
		SW_GC.registerFactory<PhysicsTestScene>();
		registerGameAppFactories();

		//! set default camera
		{
			tvec3 screenSize( SW_GC.getScreenWidth(), SW_GC.getScreenHeight(), 0 );
			screenSize = screenSize * 2;

			SWGameObject* go = new SWGameObject;
			go->setName( "Camera" );

			SWCamera* cam = go->addComponent<SWCamera>();
			cam->orthoMode( screenSize.x, screenSize.y, 1, 1000 );
			cam->getComponent<SWTransform>()->setLocalPosition( tvec3( 0, 0, -500 ) );
			cam->setClearColor( tcolor( 0,0,0,1 ) );
			cam->setClearFlags( SW_Clear_Color );
		}

		//! set logo sprite
		{
			SWGameObject* go = new SWGameObject;
			go->setName( "Logo" );
			go->addFixedRateUpdateDelegator( GetDelegator( onGOUpdate ) );

			SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "textures/flappy_bird.png" );
			SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
			renderer->setSprite( atlas()->find( "bird_0" ) );

			SWTransform* transform = go->getComponent<SWTransform>();
			transform->setLocalPosition( tvec3::zero );
			transform->setLocalScale( tvec3::one*2 );

			SWRigidBody2D* body = go->addComponent<SWRigidBody2D>();
			body->setGravityScale( -tvec2::axisY * 0.1f );
		}

		m_startTIme = SWTime.getTime();

		m_lastState = SW_None;
	}

	void onGOUpdate( SWGameObject* go )
	{
		
	}

	void onEndLogo()
	{
	}

	void onUpdate()
	{
		if ( SWInput.getKey( 'p' ) )
		{
			SW_GC.setNextScene( new __this() );
		}

		if ( SWInput.getTouchState() == SW_TouchPress && m_lastState == SW_TouchRelease )
		{
			SWRigidBody2D* body = findGO( "Logo" )->getComponent<SWRigidBody2D>();
			body->addForce( tvec2( 0, 20 ) );
		}
		m_lastState = SWInput.getTouchState();
	}

	tuint m_lastState;
	void onFixedRateUpdate()
	{

	}
};

#endif // PhysicsTestScene_h__