#ifndef IntroScene_h__
#define IntroScene_h__

#include "SWHeaders.h"
#include "GameHeaders.h"
#include "SWOpenAL.h"

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

	SWHardRef<SWAudioClip::Source> m_clipSource;
	void onAwake()
	{
		{
			SWHardRef<SWAudioClip> audioClip = SWAssets.loadAudioClip( "sounds/Jump.wav" );
			m_clipSource = audioClip()->createSource();

		}
		/*/
		{
			SWHardRef<SWInputStream> is = SWAssets.loadBuffer( "sounds/Jump.wav" );

			SWHardRef<SWByteBufferInputStream> bbis = new SWByteBufferInputStream( is() );
			SWByteBuffer* buffer = bbis()->getBuffer();

			tuint bufferID = alGenWaveBuffer( buffer->getRaw(), buffer->size() );

			tuint sourceID = 0;
			alGenSources( 1, &sourceID );
			alSourcei( sourceID, AL_BUFFER, bufferID );

			alSourcePlay( sourceID );
		}
		//*/

		SW_GC.registerFactory<IntroScene>();
		registerGameAppFactories();

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


	}


	void onEndLogo()
	{
		//SW_GC.setNextScene( new PlayScene );
	}

	void onUpdate()
	{
	}
};

#endif // IntroScene_h__