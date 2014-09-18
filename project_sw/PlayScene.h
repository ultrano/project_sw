#ifndef PlayScene_h__
#define PlayScene_h__

#include "SWHeaders.h"
#include "GameValues.h"
#include "GameHeaders.h"

class PlayScene : public SWGameScene
{
	SW_RTTI( PlayScene, SWGameScene );

public:

	enum State
	{
		None,
		Ready,
		ReadyPools,
		LoaningCoins,
		SettingCharacter,
		SettingCharacterShadow,
		SettingWorldCamera,
		PlacingCoinBasket,
		SettingUIs,
		LoadingAudios,
		Standby,
		Playing,
	};

	State m_state;
	State m_nextState;
	bool  m_stateChanged;

	SWWeakRef<SWTransform> m_charTrans;
	SWWeakRef<SWCamera> m_camera;

	SWWeakRef<SWRigidBody2D> m_coinBasket;
	SWHardRef<SWGameObject> m_background;

	tuint m_loanedCoins;
	tvec3 m_screenSize;
	SWHardRef<SWFontInfo> m_fontInfo;
	SWHardRef<SWTexture>  m_fontTexture;

	bool m_coinPatternAD;

	SWHardRef<SWAudioClip::Source> m_readyBGM;
	SWHardRef<SWAudioClip::Source> m_playBGM;
	SWHardRef<SWAudioClip::Source> m_powerUpBGM;

	PlayScene()
	{
	}

	PlayScene( factory_constructor )
	{
	}

	void onAwake()
	{
		SW_GC.registerFactory<PlayScene>();
		registerGameAppFactories();

		//! initialize values
		{
			m_loanedCoins = 0;
			m_coinPatternAD = false;
			m_state = State::None;
			m_nextState = State::None;
			m_stateChanged = true;

			m_screenSize = tvec3( SW_GC.getScreenWidth(), SW_GC.getScreenHeight(), 0 );

			//! font info & font texture
			m_fontInfo    = SWAssets.loadFontInfo( "fonts/Jetpackia.fnt" );
			m_fontTexture = SWAssets.loadTexture( "fonts/Jetpackia.png" );
		}

		//! UI Camera
		{
			tvec3 screenSize( SW_GC.getScreenWidth(), SW_GC.getScreenHeight(), 0 );

			SWGameObject* go = new SWGameObject;
			go->setName( "UICamera" );

			SWCamera* cam = go->addComponent<SWCamera>();
			cam->orthoMode( WorldWidth, WorldHeight, 1, 1000 );
			cam->getComponent<SWTransform>()->setLocalPosition( tvec3( 0, 0, -500 ) );
			cam->setTargetLayerName( "UI" );
			cam->setDepth( 1 );
		}

		//! set logo sprite
		{
			SWGameObject* go = new SWGameObject;
			go->setName( "Logo" );
			go->setLayerName( "UI" );

			SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "textures/logo5.png" );
			SWSprite* logoSprite = atlas()->find( "logo" );
			tvec2 logoSize = logoSprite->getSize();

			SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
			renderer->setSprite( logoSprite );

			SWTransform* transform = go->getComponent<SWTransform>();
			transform->setLocalPosition( tvec3::zero );
			transform->setLocalScale( tvec3( WorldWidth/logoSize.x, WorldHeight/logoSize.y, 1 ) );
		}

		//! UI State Sentences
		{
			SWGameObject* go = new SWGameObject;
			go->setName( "InitState" );
			go->setLayerName( "UI" );

			SWFontRenderer* renderer = go->addComponent<SWFontRenderer>();
			renderer->setFontInfo( m_fontInfo() );
			renderer->setFontTexture( m_fontTexture() );
			renderer->setColor( tcolor( 1, 0, 1, 1 ) );
			renderer->setText( "WAIT PLEASE" );
			renderer->setAlignH( SW_Align_Center );
			renderer->setAlignV( SW_Align_Bottom );

			SWTransform* trans = go->getComponent<SWTransform>();
			trans->setPosition( tvec3( 0, -(m_screenSize.y/4), 0 ) );
			trans->setLocalScale( tvec3::one * 0.7f );
		}
	}

	void onUpdate()
	{
		if ( SWInput.getKey( 'p' ) )
		{
			SWHardRef<SWObject> object = SW_GC.newInstance( "IntroScene" );
			SWHardRef<SWGameScene> scene = swrtti_cast<SWGameScene>( object() );
			SW_GC.setNextScene( scene() );
		}
		else if ( SWInput.getKey( 't' ) )
		{
			showPannel();
		}

		//! state update processing
		switch ( m_state )
		{
		case None : changeState( ReadyPools ); break;
		case ReadyPools :
			{
				//! pool for game objects
				{
					SWGameObject* go = new SWGameObject();
					go->setName( "Pool" );
					go->setActive( false );
				}

				//! parent object of coins when it is activated
				{
					SWGameObject* go = new SWGameObject();
					go->setName( "Coins" );
				}

				//! parent object of obstacles when it is activated
				{
					SWGameObject* go = new SWGameObject();
					go->setName( "Obstacles" );
				}

				//! timer for making coin-patterns and obstacles
				{
					SWGameObject* go = new SWGameObject();
					go->setName( "Timer" );
					go->setActive( false );

					//! acts bunch
					SWActBunch* bunch = new SWActBunch();

					//! timer for coins
					{
						SWActSequence* seq = new SWActSequence();
						seq->addAct( new SWActDelay( 6 ) );
						seq->addAct( new SWActDelegate( GetDelegator( makeCoinPattern ) ) );
						
						bunch->addAct( new SWActRepeat( seq ) );
					}

					//! timer for obstacles
					{
						SWActSequence* seq = new SWActSequence();
						seq->addAct( new SWActDelay( 2.5f ) );
						seq->addAct( new SWActDelegate( GetDelegator( makeObstacle ) ) );
						
						bunch->addAct( new SWActRepeat( seq ) );
					}

					//! timer for vehicle item
					{
						SWActSequence* seq = new SWActSequence();
						seq->addAct( new SWActDelay( 3 ) );
						seq->addAct( new SWActDelegate( GetDelegator( makeVehicleItem ) ) );
						
						bunch->addAct( new SWActRepeat( seq ) );
					}

					SWHardRef<SWAction> action = go->addComponent<SWAction>();
					action()->setAct( "startTimer", bunch );
					action()->play( "startTimer" );
				}

				changeState( LoaningCoins );
			}
			break;
		case LoaningCoins :
			{
				SWFontRenderer* renderer = findGO( "InitState" )->getComponent<SWFontRenderer>();
				renderer->setText( "THE BANK IS LOANING COINS[%d/%d]", m_loanedCoins, LoanCoins );

				newCoin()->deposit();
				if ( ++m_loanedCoins >= LoanCoins ) changeState( State::SettingCharacter );
			}
			break;

		case SettingCharacter :
			{
				//! character 
				{
					SWGameObject* go = new SWGameObject();
					go->setName( "Character" );
					go->setActive( false );

					go->addComponent<Character>();
					SWTransform* trans = go->getComponent<SWTransform>();
					trans->setPosition( tvec3( 0,GroundY,0 ) );
					m_charTrans = trans;
				}

				changeState( State::SettingCharacterShadow );
			}
			break;

		case SettingCharacterShadow :
			{
				//! character shadow
				{
					SWGameObject* go = new SWGameObject;
					go->setName( "CharacterShadow" );
					go->setActive( false );

					SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "textures/runner.png" );	
					SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
					renderer->setSprite( atlas()->find( "shadow" ) );

					go->addUpdateDelegator( GetDelegator( characterShadowUpdate ) );
				}
				
				changeState( State::SettingWorldCamera );
			}
			break;			

		case SettingWorldCamera :
			{
				//! world camera
				{
					SWGameObject* go = new SWGameObject;
					go->setName( "WorldCamera" );
					go->setActive( false );

					SWCamera* cam = go->addComponent<SWCamera>();
					cam->orthoMode( WorldWidth, WorldHeight, 1, 1000 );
					cam->getComponent<SWTransform>()->setLocalPosition( tvec3( 0, 0, -500 ) );
					cam->setClearColor( tcolor( 1,1,1,1 ) );
					cam->setClearFlags( SW_Clear_Color );
					cam->setDepth( 0 );

					go->addUpdateDelegator( GetDelegator( worldCameraUpdate ) );
				}
				
				changeState( State::PlacingCoinBasket );
			}
			break;
		case PlacingCoinBasket :
			{
				//! coin basket
				{
					SWGameObject* go = new SWGameObject();
					go->setName( "Basket" );

					SWRectCollider2D* collider = go->addComponent<SWRectCollider2D>();
					collider->setSize( tvec2( WorldWidth, WorldHeight*2 ) );
					collider->setCenter( tvec2( -WorldWidth/2, 0 ) );

					SWTransform* trans = go->getComponent<SWTransform>();
					trans->setLocalPosition( tvec3( 0, 0, 0 ) );

					m_coinBasket = go->addComponent<SWRigidBody2D>();
					m_coinBasket()->setGravityScale( tvec2::zero );
				}

				changeState( State::SettingUIs );
			}
			break;
			
		case SettingUIs :
			{
				//! Background Looper
				{
					m_background = new SWGameObject();
					m_background()->addComponent<BackGround>();
				}

				//! UI Meter Score
				{
					SWGameObject* go = new SWGameObject;
					go->setName( "MeterScore" );
					go->setLayerName( "UI" );

					SWFontRenderer* renderer = go->addComponent<SWFontRenderer>();
					renderer->setFontInfo( m_fontInfo() );
					renderer->setFontTexture( m_fontTexture() );
					renderer->setColor( tcolor( 1, 0, 1, 1 ) );
					renderer->setText( "0 Meters" );

					SWTransform* trans = go->getComponent<SWTransform>();
					trans->setPosition( tvec3( -m_screenSize.x/2, (m_screenSize.y/2), 0 ) );
				}

				//! UI Coin Score
				{
					SWGameObject* go = new SWGameObject;
					go->setName( "CoinScore" );
					go->setLayerName( "UI" );

					SWFontRenderer* renderer = go->addComponent<SWFontRenderer>();
					renderer->setFontInfo( m_fontInfo() );
					renderer->setFontTexture( m_fontTexture() );
					renderer->setColor( tcolor( 1, 1, 0, 1 ) );
					renderer->setText( "0 Coins" );

					SWTransform* trans = go->getComponent<SWTransform>();
					trans->setPosition( tvec3( -m_screenSize.x/2, (m_screenSize.y/2) - m_fontInfo()->getLineHeight(), 0 ) );
				}

				//! World UI "PUSH SPACE OR TOUCH ANYWHERE TO PLAY"
				{
					SWGameObject* go = new SWGameObject;
					go->setName( "TATP" );

					SWFontRenderer* renderer = go->addComponent<SWFontRenderer>();
					renderer->setFontInfo( m_fontInfo() );
					renderer->setFontTexture( m_fontTexture() );
					renderer->setColor( tcolor( 1, 1, 1, 1 ) );
					renderer->setAlignV( SW_Align_Center );
					renderer->setText( "PUSH SPACE OR \nTOUCH ANYWHERE TO PLAY" );

					SWTransform* trans = go->getComponent<SWTransform>();
					trans->setPosition( tvec3( 0, GroundY, 5 ) );

					SWAction* action = go->addComponent<SWAction>();
					//! act removing
					{
						SWActSequence* seq = new SWActSequence();
						seq->addAct( new SWActColorTo( 1, tcolor( 1,1,1,1 ) ) );
						seq->addAct( new SWActDelay( 4 ) );
						seq->addAct( new SWActDestroy() );
						action->setAct( "removing", seq );
					}
					//! act blinking
					{
						SWActSequence* seq = new SWActSequence();
						seq->addAct( new SWActColorTo( 1, tcolor( 1,1,1,1 ) ) );
						seq->addAct( new SWActColorTo( 1, tcolor( 1,1,1,0 ) ) );
						action->setAct( "blinking", new SWActRepeat( seq ) );
					}
				}

				changeState( LoadingAudios );
			}
			break;

		case LoadingAudios :
			{
				SWHardRef<SWAudioClip> audioClip = NULL;

				audioClip = SWAssets.loadAudioClip( "audios/powerup_collect.wav" );
				m_powerUpBGM = audioClip()->createSource();
				m_powerUpBGM()->setLooping( false );

				audioClip = SWAssets.loadAudioClip( "audios/menu_amb_lp.wav" );
				m_readyBGM = audioClip()->createSource();
				m_readyBGM()->setLooping( true );

				audioClip = SWAssets.loadAudioClip( "audios/music_level.wav" );
				m_playBGM = audioClip()->createSource();
				m_playBGM()->setLooping( true );

				m_readyBGM()->play();
				changeState( Standby );
			}
			break;

		case Standby :
			{
				if ( isButtonPushed() )
				{
					findGO( "Character" )->setActive( true );
					findGO( "CharacterShadow" )->setActive( true );
					findGO( "Timer" )->setActive( true );
					findGO( "TATP" )->getComponent<SWAction>()->play( "removing" );

					m_readyBGM()->stop();
					m_playBGM()->play();

					changeState( Playing );
				}
			}
			break;
		}

		if ( m_stateChanged )
		{
			m_stateChanged = false;

			//! state pause processing
			switch ( m_state )
			{
			case SettingUIs :
				{
					findGO( "TATP" )->getComponent<SWAction>()->play( "blinking" );
				}
				break;
			}
			
			m_state = m_nextState;

			//! state resume processing
			switch ( m_state )
			{
			case State::ReadyPools :
				{
					SWFontRenderer* renderer = findGO( "InitState" )->getComponent<SWFontRenderer>();
					renderer->setText( "READY POOLS" );
				}
				break;
			case State::SettingCharacter :
				{
					SWFontRenderer* renderer = findGO( "InitState" )->getComponent<SWFontRenderer>();
					renderer->setText( "SETTING A CHARACTER" );
				}
				break;
			case State::SettingCharacterShadow :
				{
					SWFontRenderer* renderer = findGO( "InitState" )->getComponent<SWFontRenderer>();
					renderer->setText( "SETTING A SHADOW FOR THE CHARACTER" );
				}
				break;
			case State::SettingWorldCamera :
				{
					SWFontRenderer* renderer = findGO( "InitState" )->getComponent<SWFontRenderer>();
					renderer->setText( "SETTING A CAMERA" );
				}
				break;
			case PlacingCoinBasket :
				{
					SWFontRenderer* renderer = findGO( "InitState" )->getComponent<SWFontRenderer>();
					renderer->setText( "PLACING COIN BASKET" );
				}
				break;
			case LoadingAudios :
				{
					SWFontRenderer* renderer = findGO( "InitState" )->getComponent<SWFontRenderer>();
					renderer->setText( "FINDING GOOD MUSICS" );
				}
				break;
			case Standby :
				{
					findGO( "WorldCamera" )->setActive( true );
					findGO( "Logo" )->destroy();
					findGO( "InitState" )->destroy();
				}
				break;
			}
		}
	}

	void changeState( State state )
	{
		m_nextState = state;
		m_stateChanged = true;
	}

	void worldCameraUpdate( SWGameObject* go )
	{
		if ( m_charTrans.isValid() == false ) return;
		tvec3 pos = m_charTrans()->getPosition();
		pos.x += 150;
		pos.y = WorldHeight/2;
		pos.z = -500;

		SWTransform* trans = go->getComponent<SWTransform>();
		trans->setPosition( pos );

		pos.x -= WorldWidth/2;
		m_coinBasket()->setPosition( pos.xy() );
	}

	void characterShadowUpdate( SWGameObject* go )
	{
		if ( m_charTrans.isValid() == false ) return;

		SWTransform* trans = go->getComponent<SWTransform>();

		tvec3 pos = m_charTrans()->getPosition();
		pos.y = GroundY - 20;
		pos.z -= 1;

		float rate = (RoofY - m_charTrans()->getPosition().y)/(RoofY - GroundY);

		SWSpriteRenderer* renderer = go->getComponent<SWSpriteRenderer>();
		renderer->setColor( tcolor( 1, 1, 1, (rate*rate*0.5f) + 0.2f ) );

		trans->setLocalScale( tvec3( 0.4f, 0.4f,1 ) * rate );
		trans->setPosition( pos );
	}

	Coin* newCoin()
	{
		SWLog( "make new coin" );
		SWGameObject* go = new SWGameObject();
		return go->addComponent<Coin>();
	}

	Coin* getCoin()
	{
		Coin* coin = NULL;
		if ( SWGameObject* coinGO = findGO( "Pool/Coin" ) )
		{
			coin = coinGO->getComponent<Coin>();
		}
		else
		{
			coin = newCoin();
		}
		coin->withDraw();
		return coin;
	}

	void makeCoinPattern()
	{
		tvec3 pos = m_charTrans()->getPosition();
		float frontFromChar = pos.x + WorldWidth;
		
		tuint patternNum = 1;
		char buf[256] = {0};
		if ( !m_coinPatternAD )
		{
			patternNum = 27;
			m_coinPatternAD = true;
		}
		else
		{
			patternNum = SWMath.randomInt(1,27);
		}
		sprintf( &buf[0], "patterns/coinpattern%d.txt", patternNum );

		SWHardRef<SWInputStream> is = SWAssets.loadBuffer( &buf[0] );
		SWInputStreamReader reader( is() );
		
		tvec2 step(0, SWMath.randomInt( (RoofY+GroundY)/2, RoofY) );
		tstring line;
		while ( reader.readLine( line ) > 0 )
		{
			step.y -= 15;
			step.x = frontFromChar;
			for ( int i = 0 ; i < line.size() ; ++i )
			{
				step.x += 15;
				char c = line[i];
				if ( c == '1' )
				{
					Coin* coin = getCoin();
					SWTransform* trans = coin->getComponent<SWTransform>();
					trans->setPosition( tvec3( step.x, step.y, pos.z ) );
				}
			}
		}
	}

	void makeObstacle()
	{
		SWGameObject* go = findGO( "Pool/Obstacle" );

		if ( go == NULL ) (go = new SWGameObject())->addComponent<Obstacle>();

		SWTransform* trans = go->addComponent<Obstacle>()->getComponent<SWTransform>();;
		tvec3 pos = m_charTrans()->getPosition();
		pos.y = SWMath.randomInt( GroundY, RoofY );
		trans->setPosition( pos + tvec3( WorldWidth, 0, 0 ) );
		trans->setParent( findGO( "Obstacles" )->getComponent<SWTransform>() );

		SWAction* action = go->getComponent<SWAction>();
		
		if ( SWMath.randomInt(0,100) % 2 == 0 ) action->play( "pattern1" );
		else action->play( "pattern2" );
	}

	void makeVehicleItem()
	{
		CharacterState* charState = m_charTrans()->getComponent<CharacterState>();
		if ( charState->queryRtti() != Runner::getRtti() ) return;

		SWGameObject* go = findGO( "Pool/VehicleItem" );

		if ( go == NULL ) (go = new SWGameObject)->addComponent<VehicleItem>();

		SWTransform* trans = go->getComponent<SWTransform>();
		tvec3 pos = m_charTrans()->getPosition();
		pos.y = SWMath.randomInt( GroundY, RoofY );
		trans->setPosition( pos + tvec3( WorldWidth, 0, 0 ) );
		trans->setParent( NULL );
	}

	void showPannel()
	{
		//! holding character for showing
		{
			SWRigidBody2D* body = m_charTrans()->getComponent<SWRigidBody2D>();
			body->setFixedPosition( true );
		}

		//! pause timer for showing
		{
			findGO( "Timer" )->setActive( false );
		}

		//! removing all obstacles in field
		{
			SWTransform* poolTrans = findGO( "Pool" )->getComponent<SWTransform>();

			SWTransform* trans = findGO( "Obstacles" )->getComponent<SWTransform>();
			SWObject::List children;
			trans->copyChildren( children );
			
			SWObject::List::iterator itor = children.begin();
			for ( ; itor != children.end() ; ++itor )
			{
				SWTransform* child = swrtti_cast<SWTransform>( (*itor)() );
				child->setParent( poolTrans );
			}
		}

		//! right pannel
		{
			SWGameObject* go = new SWGameObject();
			go->setLayerName( "UI" );
			go->addFixedRateUpdateDelegator( GetDelegator( onUpdatePannel ) );

			SWHardRef<SWFontInfo> fontInfo    = SWAssets.loadFontInfo( "fonts/Jetpackia.fnt" );
			SWHardRef<SWTexture>  fontTexture = SWAssets.loadTexture( "fonts/Jetpackia.png" );

			SWFontRenderer* renderer = go->addComponent<SWFontRenderer>();
			renderer->setFontInfo( fontInfo() );
			renderer->setFontTexture( fontTexture() );
			renderer->setText( "FLAPPY BIRD" );
			renderer->setColor( tcolor( 169.0f/256,169.0f/256,169.0f/256,1 ) );
			renderer->setAlignH( SW_Align_Center );

			SWTransform* trans = go->getComponent<SWTransform>();
			trans->setPosition( tvec3( WorldWidth*2, -WorldHeight/4, 0 ) );
			trans->setLocalScale( tvec3::one * 1.5f );

			SWAction* action = go->addComponent<SWAction>();
			SWHardRef<SWActSequence> seq = new SWActSequence();
			seq()->addAct( new SWActDelay(1) );
			seq()->addAct( new SWActColorTo( 0.5f, tcolor( 1,1,1,0 ) ) );
			seq()->addAct( new SWActDelegate( GetDelegator( showOver ) ) );
			seq()->addAct( new SWActDestroy() );
			action->setAct( "action", seq() );
			action->play( "action" );
		}

		//! left pannel
		{
			SWGameObject* go = new SWGameObject();
			go->setLayerName( "UI" );
			go->addFixedRateUpdateDelegator( GetDelegator( onUpdatePannel ) );

			SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "textures/flappy_bird.png" );

			SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
			renderer->setSprite( atlas()->find( "bird_gray" ) );
			
			SWTransform* trans = go->getComponent<SWTransform>();
			trans->setPosition( tvec3( WorldWidth*-2, -WorldHeight/4, 0 ) );
			trans->setLocalScale( tvec3( 4, 4, 1 ) );

			SWAction* action = go->addComponent<SWAction>();
			SWHardRef<SWActSequence> seq = new SWActSequence();
			seq()->addAct( new SWActDelay(1) );
			seq()->addAct( new SWActColorTo( 0.5f, tcolor( 1,1,1,0 ) ) );
			seq()->addAct( new SWActDestroy() );
			action->setAct( "action", seq() );
			action->play( "action" );
		}

		//! bar pannel
		{
			SWGameObject* go = new SWGameObject();
			go->setLayerName( "UI" );
			go->addFixedRateUpdateDelegator( GetDelegator( onUpdatePannel ) );

			SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "textures/flappy_bird.png" );

			SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
			renderer->setSprite( atlas()->find( "bar" ) );
			
			SWTransform* trans = go->getComponent<SWTransform>();
			trans->setPosition( tvec3( WorldWidth*-10, -WorldHeight/4, -1 ) );
			trans->setLocalScale( tvec3( 8, 8, 1 ) );

			SWAction* action = go->addComponent<SWAction>();
			SWHardRef<SWActSequence> seq = new SWActSequence();
			seq()->addAct( new SWActDelay(1) );
			seq()->addAct( new SWActColorTo( 0.5f, tcolor( 1,1,1,0 ) ) );
			seq()->addAct( new SWActDestroy() );
			action->setAct( "action", seq() );
			action->play( "action" );
		}

		//! play sound
		{
			m_powerUpBGM()->play();
		}
	}

	void showOver()
	{
		SWRigidBody2D* body = m_charTrans()->getComponent<SWRigidBody2D>();
		if ( !body->isFixedPosition() ) return;
		
		body->setFixedPosition( false );
		body->setVelocity( body->getVelocity() / 2 );

		findGO( "Timer" )->setActive( true );
	}

	void onUpdatePannel( SWGameObject* go )
	{
		tvec3 dest( 0, 0, 0 );

		SWTransform* trans = go->getComponent<SWTransform>();

		tvec3 pos = trans->getPosition();

		tvec3 delta = (dest - pos);
		pos.x += (delta.x/5);
		pos.x -= delta.normal().x * 20;

		trans->setPosition( pos );

	}

private:

};

#endif // PlayScene_h__