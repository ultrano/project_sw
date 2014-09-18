#ifndef ResultScene_h__
#define ResultScene_h__

#include "SWHeaders.h"

class ResultScene : public SWGameScene
{
	SW_RTTI( ResultScene, SWGameScene );

private:

	enum
	{
		ReadyCount,
		CountMeter,
		CountScore,
		WaitReplay,
	};

	const tuint m_meter;
	const tuint m_score;
	tuint m_count;
	tuint m_state;
	bool m_wasPressed;
	tint m_fireWorkDelay;

	SWHardRef<SWFontRenderer> m_meterFont;
	SWHardRef<SWFontRenderer> m_coinFont;
	SWHardRef<SWFontInfo> m_fontInfo;
	SWHardRef<SWTexture>  m_fontTexture;
	SWHardRef<SWAudioClip::Source> m_countSound;
	SWHardRef<SWAudioClip::Source> m_tadahSound1;
	SWHardRef<SWAudioClip::Source> m_tadahSound2;
public:

	ResultScene( tuint meter, tuint score )
		: m_meter( meter )
		, m_score( score )
		, m_count( 0 )
		, m_fireWorkDelay( 0 )
	{
	};

	ResultScene(factory_constructor)
		: m_meter( 0 )
		, m_score( 0 )
		, m_count( 0 )
		, m_fireWorkDelay( 0 )
	{
	};

	void onAwake()
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
		{
			SWGameObject* go = new SWGameObject;
			go->setName( "Logo" );

			SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "textures/flappy_bird.png" );
			SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
			SWSprite* sprite = atlas()->find( "background_2" );
			tvec2 spriteSize = sprite->getSize();
			renderer->setSprite( sprite );

			SWTransform* transform = go->getComponent<SWTransform>();
			transform->setLocalPosition( tvec3::zero );
			transform->setLocalScale( tvec3( 480/spriteSize.x, 320/spriteSize.y, 1 ) );
		}

		//! font info & font texture
		{
			m_fontInfo    = SWAssets.loadFontInfo( "fonts/Jetpackia.fnt" );
			m_fontTexture = SWAssets.loadTexture( "fonts/Jetpackia.png" );
		}

		//! meter font
		{
			SWGameObject* go = new SWGameObject;
			SWFontRenderer* renderer = go->addComponent<SWFontRenderer>();
			renderer->setFontInfo( m_fontInfo() );
			renderer->setFontTexture( m_fontTexture() );
			renderer->setText( "0M" );
			renderer->setAlignH( SW_Align_Center );
			renderer->setAlignV( SW_Align_Bottom );
			renderer->setColor( tcolor( 1, 0, 1, 1 ) );
			m_meterFont = renderer;
		}

		//! coin font
		{
			SWGameObject* go = new SWGameObject;
			SWFontRenderer* renderer = go->addComponent<SWFontRenderer>();
			renderer->setFontInfo( m_fontInfo() );
			renderer->setFontTexture( m_fontTexture() );
			renderer->setText( "0%c", (char)169 );
			renderer->setAlignH( SW_Align_Center );
			renderer->setAlignV( SW_Align_Top );
			renderer->setColor( tcolor( 1, 1, 0, 1 ) );
			m_coinFont = renderer;
		}

		//! load sound
		{
			SWHardRef<SWAudioClip> audioClip = NULL;

			audioClip = SWAssets.loadAudioClip( "audios/EF_Result_score_gauge.wav" );
			m_countSound = audioClip()->createSource();
			m_countSound()->setLooping( true );

			audioClip = SWAssets.loadAudioClip( "audios/EF_halbae_clear.wav" );
			m_tadahSound1 = audioClip()->createSource();

			audioClip = SWAssets.loadAudioClip( "audios/EF_play_rank_up_02.wav" );
			m_tadahSound2 = audioClip()->createSource();
		}

		m_state = ReadyCount;
	}

	virtual void onFixedRateUpdate()
	{
		scoreCheck();
	}

	void scoreCheck()
	{
		switch ( m_state )
		{
		case ReadyCount :
			m_state = CountMeter;
			m_countSound()->play();
			break;
		case CountMeter :
			if ( ++m_count < m_meter )
			{
				m_meterFont()->setText( "%dM", m_count );
			}
			else
			{
				m_tadahSound1()->play();
				m_meterFont()->setText( "%dM", m_meter );
				m_count = 0;
				m_state = CountScore;
			}
			if ( !m_wasPressed && isButtonPushed() ) m_count = m_meter;
			break;
		case CountScore :
			if ( ++m_count < m_score )
			{
				m_coinFont()->setText( "%d%c", m_count, (char)169 );
			}
			else
			{
				m_tadahSound2()->play();
				m_coinFont()->setText( "%d%c", m_score, (char)169 );
				m_count = 0;
				m_state = WaitReplay;
			}
			if ( !m_wasPressed &&  isButtonPushed() ) m_count = m_score;
			break;
		case WaitReplay :
			if ( m_countSound()->isPlaying() ) m_countSound()->stop();
			if ( !m_wasPressed &&  isButtonPushed() )
			{
				SWHardRef<SWObject> instance = SW_GC.newInstance( "PlayScene" );
				SWGameScene* scene = swrtti_cast<SWGameScene>( instance() );
				SW_GC.setNextScene( scene );
			}
			makeFireWorks();
			break;
		}

		m_wasPressed = isButtonPushed();
	}

	void makeFireWorks()
	{
		if ( --m_fireWorkDelay > 0 ) return;
		m_fireWorkDelay = 10;

		tuint count = SWMath.randomInt( 1, 4 );
		while ( count-- )
		{
			SWGameObject* go = new SWGameObject;
			SWTransform* trans = go->addComponent<FireWork>()->getComponent<SWTransform>();

			float halfW = WorldWidth/2.0f;
			float halfH = WorldHeight/2.0f;
			tvec3 pos( SWMath.randomInt( -halfW,halfW ), SWMath.randomInt( -halfH,halfH ), 0 );
			trans->setPosition( pos );
		}
	}
};

#endif // ResultScene_h__