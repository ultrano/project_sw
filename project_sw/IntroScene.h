#ifndef IntroScene_h__
#define IntroScene_h__

#include "SWHeaders.h"

class IntroScene : public SWGameScene
{
	SW_RTTI( IntroScene, SWGameScene );
public:
	
	IntroScene()
	{
	};

	IntroScene(factory_constructor)
	{
	};

	void onAwake()
	{
		SW_GC.registerFactory<IntroScene>();

		//! set default camera
		{
			tvec3 screenSize( SW_GC.getScreenWidth(), SW_GC.getScreenHeight(), 0 );
			
			SWGameObject* go = new SWGameObject;
			go->setName( "Camera" );

			SWCamera* cam = go->addComponent<SWCamera>();
			cam->orthoMode( screenSize.x, screenSize.y, 1, 1000 );
			cam->getComponent<SWTransform>()->setLocalPosition( tvec3( 0, 0, -500 ) );
			cam->setClearColor( tcolor( 1,1,1,1 ) );
			cam->setClearFlags( SW_Clear_Color );
		}

		if (false)
		{
			SWGameObject* go = new SWGameObject;
			go->setName( "Logo" );

			SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "textures/logo5.png" );
			SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
			SWSprite* logoSprite = atlas()->find( "logo" );
			tvec2 logoSize = logoSprite->getSize();
			renderer->setSprite( logoSprite );
			renderer->setColor( tcolor( 1,1,1,0 ) );

			SWAction* action = go->addComponent<SWAction>();
			SWActSequence* seq = new SWActSequence();
			seq->addAct( new SWActColorTo( 1, tcolor( 1,1,1,1 ) ) );
			seq->addAct( new SWActColorTo( 2, tcolor( 1,1,1,0 ) ) );
			seq->addAct( new SWActDelegate( GetDelegator( onEndLogo ) ) );
			action->setAct( "logo", seq );
			action->play( "logo" );

			SWTransform* transform = go->getComponent<SWTransform>();
			transform->setLocalPosition( tvec3::zero );
			transform->setLocalScale( tvec3( 480/logoSize.x, 320/logoSize.y, 1 ) );
		}

		//! new font test

		SWHardRef<SWFontInfo> info = SWAssets.loadFontInfo( "fonts/test.fnt" );
		SWHardRef<SWTexture> texture = SWAssets.loadTexture( "fonts/test.png" );
		{
			SWGameObject* go = new SWGameObject;
			go->setName( "text" );

			SWFontRenderer* renderer = go->addComponent<SWFontRenderer>();
			renderer->setFontInfo( info() );
			renderer->setFontTexture( texture() );
			renderer->setText( "12341111s111111111" );
			
			SWTransform* transform = go->getComponent<SWTransform>();
			transform->setLocalPosition( tvec3( -100,0,0 ) );

			SWAction* action = go->addComponent<SWAction>();
			SWActSequence* seq = new SWActSequence();
			seq->addAct( new SWActDelay(0.1f) );
			seq->addAct( new SWActDelegate( GetDelegator( onCount ) ) );
			action->setAct( "count", new SWActRepeat( seq ) );
			action->play( "count" );

			m_count = 0;
		}
	}

	int m_count;
	void onCount()
	{
		SWFontRenderer* renderer = findGO( "text" )->getComponent<SWFontRenderer>();
		char buf[256] = {0};
		sprintf( &buf[0], "%d", m_count++ );
		renderer->setText( &buf[0] );
	}

	void onEndLogo()
	{
		SW_GC.setNextScene( new PlayScene );
		//SW_GC.setNextScene( new TestScene );
	}
};

#endif // IntroScene_h__