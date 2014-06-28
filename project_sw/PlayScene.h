#ifndef PlayScene_h__
#define PlayScene_h__

#include "SWHeaders.h"

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
		}

		//! bird
		{
			SWGameObject* go = new SWGameObject;
			SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();

			SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "flappy_bird.png" );
			renderer->setSprite( atlas()->find( "bird_1" ) );

			SWHardRef<SWSpriteAnimation> anim = SWAssets.loadSpriteAnimation( "anim.txt" );
			SWAction* action = go->addComponent<SWAction>();
			action->setAct( "flying", new SWActRepeat( new SWActAnimate( 1, anim()->getSequenceByName( "flying" ) ) ) ); 
			action->play( "flying" );

			SWRigidBody2D* body = go->addComponent<SWRigidBody2D>();
			
		}
		SWLog( "play scene awake test" );
	}

private:

};

#endif // PlayScene_h__