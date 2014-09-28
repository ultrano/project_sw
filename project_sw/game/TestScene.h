#ifndef TestScene_h__
#define TestScene_h__

#include "SWHeaders.h"

class TestScene : public SWGameScene
{
	SW_RTTI(TestScene,SWGameScene);
public:
	TestScene() {}
	~TestScene() {}

	override void onAwake()
	{
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

		//! set logo sprite
		SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "flappy_bird.png" );
		for ( tuint i = 0 ; i < 10 ; ++i )
		{
			SWGameObject* go = new SWGameObject;

			SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
			SWSprite* logoSprite = atlas()->find( "bird_0" );
			tvec2 logoSize = logoSprite->getSize();
			renderer->setSprite( logoSprite );

			SWTransform* transform = go->getComponent<SWTransform>();
			transform->setPosition( tvec3(50*i,0,0) );
		}
	}

	override void onFixedRateUpdate()
	{
		if ( SWInput.getKey('d') )
		{
			SWTransform* trans = findGO("Camera")->getComponent<SWTransform>();
			trans->setPosition( trans->getPosition() + tvec3(1,0,0) );
		}
		else if ( SWInput.getKey('a') )
		{
			SWTransform* trans = findGO("Camera")->getComponent<SWTransform>();
			trans->setPosition( trans->getPosition() - tvec3(1,0,0) );
		}
	}

};

#endif // TestScene_h__