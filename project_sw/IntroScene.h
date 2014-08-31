#ifndef IntroScene_h__
#define IntroScene_h__

#include "SWHeaders.h"

class IntroScene : public SWGameScene
{
	SW_RTTI( IntroScene, SWGameScene );

	float m_time;
	float m_remain;
public:
	
	IntroScene()
	{
	};

	IntroScene(factory_constructor)
	{
		SW_GC.registerFactory<IntroScene>();
	};

	void onAwake()
	{
		//! convert test
		{
			twstring wstr = L"test";
			tstring tstr = "";
			SWUtil.utf16ToUTF8( wstr, tstr );
			int a= 0;
		}

		//! set default camera
		{
			tvec3 screenSize( SW_GC.getScreenWidth(), SW_GC.getScreenHeight(), 0 );
			
			SWGameObject* go = new SWGameObject;
			go->setName( "camera" );

			SWCamera* cam = go->addComponent<SWCamera>();
			cam->orthoMode( screenSize.x, screenSize.y, 1, 1000 );
			cam->getComponent<SWTransform>()->setLocalPosition( tvec3( 0, 0, -500 ) );
			cam->setClearColor( tcolor( 1,1,1,1 ) );
			cam->setClearFlags( SW_Clear_Color );
		}

		{
			SWGameObject* go = new SWGameObject;
			go->setName( "logo" );

			SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "logo5.png" );
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

		{
			m_time = 4;
			m_remain = m_time;
		}

		{
			SWHardRef<SWInputStream> fis = SWAssets.loadBuffer( "animation.txt" );
			SWInputStreamReader reader( fis() );
			tstring line;
			while( reader.readLine( line ) )
			{
				SWLog( line.c_str() );
			}
		}

		//! finding test
		{
			SWGameObject* go = new SWGameObject();
			go->setName( "go" );
			SWTransform* parent = go->getComponent<SWTransform>();

			tuint count = 3;
			while ( count-- )
			{
				go = new SWGameObject();
				go->setName( "go" );
				SWTransform* trans = go->getComponent<SWTransform>();
				trans->setParent( parent );
				parent = trans;
			}

			go = findGO( "go1/go/go1" );
			parent = go->getComponent<SWTransform>();
		}

	}
	void onEndLogo()
	{
		SW_GC.setNextScene( new PlayScene );
		//SW_GC.setNextScene( new TestScene );
	}
};

#endif // IntroScene_h__