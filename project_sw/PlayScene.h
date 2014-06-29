#ifndef PlayScene_h__
#define PlayScene_h__

#include "SWHeaders.h"
#include "Bird.h"
#include "Pipe.h"
#include "PipeGen.h"

class PlayScene : public SWGameScene
{
	SW_RTTI( PlayScene, SWGameScene );

public:

	PlayScene()
	{
	}

	PlayScene( factory_constructor )
	{
	}

	void onAwake()
	{
		SW_GC.registerFactory<PlayScene>();
		SW_GC.registerFactory<Bird>();
		SW_GC.registerFactory<Pipe>();
		SW_GC.registerFactory<PipeGen>();

		//! camera
		{
			tvec3 screenSize( SW_GC.getScreenWidth(), SW_GC.getScreenHeight(), 0 );
			
			SWGameObject* go = new SWGameObject;
			go->setName( "camera" );

			SWCamera* cam = go->addComponent<SWCamera>();
			cam->orthoMode( 140, 256, 1, 1000 );
			cam->getComponent<SWTransform>()->setLocalPosition( tvec3( 0, 0, -500 ) );
			cam->setClearColor( tcolor( 1,1,1,1 ) );
			cam->setClearFlags( SW_Clear_Color );
		}

		//! background image
		{
			SWGameObject* go = new SWGameObject;
			SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();

			SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "flappy_bird.png" );
			renderer->setSprite( atlas()->find( "background" ) );

			SWTransform* transform = go->getComponent<SWTransform>();
			transform->setLocalPosition( tvec3( 0, 0, -1 ) );
		}

		//! bird
		{
			SWGameObject* go = new SWGameObject;
			m_bird = go->addComponent<Bird>();
		}

		//! pipe generator
		{
			SWGameObject* go = new SWGameObject;
			go->addComponent<PipeGen>();
		}

		//! down dead line
		{
			SWGameObject* go = new SWGameObject();
			
			SWRectCollider2D* collider = go->addComponent<SWRectCollider2D>();
			collider->setSize( tvec2( 100, 10 ) );

			SWTransform* transform = go->getComponent<SWTransform>();
			transform->setPosition( tvec3( 0, -138,0 ) );
		}

		SWLog( "play scene awake test" );
	}

	void onUpdate()
	{
		if ( SWInput.getTouchState() == SW_TouchPress )
		{
			SWRigidBody2D* body = m_bird->getComponent<SWRigidBody2D>();
			body->addForce( tvec2( 0, 50 ) );
		}
		if ( SWInput.getKey( 'p' ) )
		{
			SWHardRef<PlayScene> scene = SW_GC.newInstance<PlayScene>();
			SW_GC.setNextScene( (SWGameScene*)scene() );
		}
	}

private:

	Bird* m_bird;

};

#endif // PlayScene_h__