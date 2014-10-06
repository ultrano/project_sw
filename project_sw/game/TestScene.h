#ifndef TestScene_h__
#define TestScene_h__

#include "SWHeaders.h"
#include "SWShape2D.h"
#include "SWBroadPhase2D.h"

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

		SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "flappy_bird.png" );
		SWSprite* logoSprite = atlas()->find( "bird_0" );
		tvec2 logoSize = logoSprite->getSize();

		tuint count = 30;
		float radius = 150;
		for ( tuint i = 0 ; i < count ; ++i )
		{
			SWGameObject* go = new SWGameObject;
			SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
			renderer->setSprite( logoSprite );
			
			SWCollider2D* collider = go->addComponent<SWCollider2D>();
			collider->addBox( tvec2::zero, logoSize.x, logoSize.y );

			float radian = SWMath.angleToRadian( ((float)i/(float)count) * 360.0f );
			float x = radius * SWMath.cos( radian );
			float y = radius * SWMath.sin( radian );

			SWTransform* trans = go->getComponent<SWTransform>();
			trans->setPosition( tvec3(x,y,0) );
		}
		{
			SWGameObject* go = new SWGameObject("test");
			SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
			renderer->setSprite( logoSprite );

			SWCollider2D* collider = go->addComponent<SWCollider2D>();
			collider->addBox( tvec2::zero, logoSize.x, logoSize.y );

			SWTransform* trans = go->getComponent<SWTransform>();
			trans->setLocalScale( tvec3(1,1,1) );
			trans->setLocalRotate( tvec3::axisZ*SWMath.angleToRadian(45));
		}

	}

	override void onFixedRateUpdate()
	{
		SWTransform* trans = findGO("test")->getComponent<SWTransform>();
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

};

#endif // TestScene_h__