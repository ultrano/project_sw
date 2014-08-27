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

			SWTransform* transform = go->getComponent<SWTransform>();
			transform->setLocalPosition( tvec3::zero );

			SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "logo5.png" );
			SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
			renderer->setSprite( atlas()->find( "logo" )  );
			renderer->setColor( tcolor( 1,1,1,0 ) );

			SWAction* action = go->addComponent<SWAction>();
			SWActSequence* seq = new SWActSequence();
			seq->addAct( new SWActColorTo( 1, tcolor( 1,1,1,1 ) ) );
			seq->addAct( new SWActColorTo( 1, tcolor( 1,1,1,0 ) ) );
			seq->addAct( new SWActDelegate( GetDelegator( onEndLogo ) ) );
			action->setAct( "logo", seq );
			action->play( "logo" );
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

	}
	void onEndLogo()
	{
		SW_GC.setNextScene( new PlayScene );
		//SW_GC.setNextScene( new TestScene );
	}
};

#endif // IntroScene_h__