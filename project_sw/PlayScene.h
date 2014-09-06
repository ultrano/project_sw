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
		SettingRider,
		SettingRiderShadow,
		SettingRiderCamera,
		PlacingCoinBasket,
		SettingUIs,
		Standby,
		Playing,
	};

	State m_state;
	State m_nextState;
	bool  m_stateChanged;

	SWWeakRef<Rider> m_rider;
	SWWeakRef<SWCamera> m_camera;

	SWWeakRef<SWRigidBody2D> m_coinBasket;
	SWHardRef<SWGameObject> m_background;

	tuint m_loanedCoins;
	tvec3 m_screenSize;
	SWHardRef<SWFontInfo> m_fontInfo;
	SWHardRef<SWTexture>  m_fontTexture;

	bool m_coinPatternAD;

	PlayScene()
	{
	}

	PlayScene( factory_constructor )
	{
	}

	void onAwake()
	{
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

			SWAction* action = go->addComponent<SWAction>();
			SWActSequence* seq = new SWActSequence();

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

		//! state update processing
		switch ( m_state )
		{
		case None : changeState( ReadyPools ); break;
		case ReadyPools :
			{
				//! coin bank (for pooling)
				{
					SWGameObject* go = new SWGameObject();
					go->setName( "Bank" );
					go->setActive( false );
				}

				//! gas cloud pool
				{
					SWGameObject* go = new SWGameObject();
					go->setName( "GasCloudPool" );
					go->setActive( false );
				}

				//! timer for making coin-patterns
				{
					SWGameObject* go = new SWGameObject();
					go->setName( "CoinTimer" );
					go->setActive( false );

					SWHardRef<SWAction> action = go->addComponent<SWAction>();
					SWActSequence* seq = new SWActSequence();
					seq->addAct( new SWActDelay( 6 ) );
					seq->addAct( new SWActDelegate( GetDelegator( makeCoinPattern ) ) );
					action()->setAct( "makeCoin", new SWActRepeat( seq ) );
					action()->play( "makeCoin" );
				}

				changeState( LoaningCoins );
			}
			break;
		case LoaningCoins :
			{
				SWFontRenderer* renderer = findGO( "InitState" )->getComponent<SWFontRenderer>();
				renderer->setText( "THE BANK IS LOANING COINS[%d/%d]", m_loanedCoins, LoanCoins );

				newCoin()->deposit();
				if ( ++m_loanedCoins >= LoanCoins ) changeState( State::SettingRider );
			}
			break;

		case SettingRider :
			{
				//! rider 
				{
					SWGameObject* go = new SWGameObject();
					go->setName( "Rider" );
					go->setActive( false );

					Rider* rider = go->addComponent<Rider>();
					SWTransform* trans = go->getComponent<SWTransform>();
					trans->setPosition( tvec3( 0,GroundY,0 ) );
					m_rider = rider;
				}

				changeState( State::SettingRiderShadow );
			}
			break;

		case SettingRiderShadow :
			{
				//! rider shadow
				{
					SWGameObject* go = new SWGameObject;
					go->setName( "RiderShadow" );
					go->setActive( false );

					SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "textures/runner.png" );	
					SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
					renderer->setSprite( atlas()->find( "shadow" ) );

					go->addUpdateDelegator( GetDelegator( riderShadowUpdate ) );
				}
				
				changeState( State::SettingRiderCamera );
			}
			break;			

		case SettingRiderCamera :
			{
				//! rider camera
				{
					SWGameObject* go = new SWGameObject;
					go->setName( "RiderCamera" );
					go->setActive( false );

					SWCamera* cam = go->addComponent<SWCamera>();
					cam->orthoMode( WorldWidth, WorldHeight, 1, 1000 );
					cam->getComponent<SWTransform>()->setLocalPosition( tvec3( 0, 0, -500 ) );
					cam->setClearColor( tcolor( 1,1,1,1 ) );
					cam->setClearFlags( SW_Clear_Color );
					cam->setDepth( 0 );

					go->addUpdateDelegator( GetDelegator( riderCameraUpdate ) );
				}
				
				changeState( State::PlacingCoinBasket );
			}
			break;
		case PlacingCoinBasket :
			{
				//! coin basket
				{
					SWGameObject* go = new SWGameObject();
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

				//! World UI "TOUCH ANYWHERE TO PLAY"
				{
					SWGameObject* go = new SWGameObject;
					go->setName( "TATP" );

					SWFontRenderer* renderer = go->addComponent<SWFontRenderer>();
					renderer->setFontInfo( m_fontInfo() );
					renderer->setFontTexture( m_fontTexture() );
					renderer->setColor( tcolor( 1, 1, 1, 1 ) );
					renderer->setText( "TOUCH ANYWHERE TO PLAY" );

					SWTransform* trans = go->getComponent<SWTransform>();
					trans->setPosition( tvec3( 0, GroundY, 0 ) );

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

				changeState( Standby );
			}
			break;

		case Standby :
			{
				if ( SWInput.getTouchState() == SW_TouchPress )
				{
					//m_state = State::SettingRiderShadow;
					findGO( "Rider" )->setActive( true );
					findGO( "RiderShadow" )->setActive( true );
					findGO( "CoinTimer" )->setActive( true );
					findGO( "TATP" )->getComponent<SWAction>()->play( "removing" );

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
			case State::SettingRider :
				{
					SWFontRenderer* renderer = findGO( "InitState" )->getComponent<SWFontRenderer>();
					renderer->setText( "SETTING A RIDER" );
				}
				break;
			case State::SettingRiderShadow :
				{
					SWFontRenderer* renderer = findGO( "InitState" )->getComponent<SWFontRenderer>();
					renderer->setText( "SETTING A SHADOW FOR THE RIDER" );
				}
				break;
			case State::SettingRiderCamera :
				{
					SWFontRenderer* renderer = findGO( "InitState" )->getComponent<SWFontRenderer>();
					renderer->setText( "SETTING A CAMERA FOR THE RIDER" );
				}
				break;
			case PlacingCoinBasket :
				{
					SWFontRenderer* renderer = findGO( "InitState" )->getComponent<SWFontRenderer>();
					renderer->setText( "PLACING COIN BASKET" );
				}
				break;
			case Standby :
				{
					findGO( "RiderCamera" )->setActive( true );
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

	void riderCameraUpdate( SWGameObject* go )
	{
		if ( m_rider.isValid() == false ) return;
		tvec3 pos = m_rider()->getComponent<SWTransform>()->getPosition();
		pos.x += 150;
		pos.y = WorldHeight/2;
		pos.z = -500;

		SWTransform* trans = go->getComponent<SWTransform>();
		trans->setPosition( pos );

		pos.x -= WorldWidth/2;
		m_coinBasket()->setPosition( pos.xy() );
	}

	void riderShadowUpdate( SWGameObject* go )
	{
		if ( m_rider.isValid() == false ) return;

		SWTransform* riderTrans = m_rider()->getComponent<SWTransform>();
		SWTransform* trans = go->getComponent<SWTransform>();

		tvec3 pos = riderTrans->getPosition();
		pos.y = GroundY - 20;
		pos.z -= 1;

		float rate = (RoofY - riderTrans->getPosition().y)/(RoofY - GroundY);

		SWSpriteRenderer* renderer = go->getComponent<SWSpriteRenderer>();
		renderer->setColor( tcolor( 1, 1, 1, (rate*rate*0.5f) + 0.2f ) );

		trans->setLocalScale( tvec3( 0.4f, 0.4f,1 ) * rate );
		trans->setPosition( pos );
	}

	Coin* newCoin()
	{
		SWLog( "make new coin" );
		SWGameObject* go = new SWGameObject();
		go->setName( "Coin" );
		return go->addComponent<Coin>();
	}

	Coin* getCoin()
	{
		Coin* coin = NULL;
		if ( SWGameObject* coinGO = findGO( "Bank/Coin" ) )
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
		SWHardRef<SWTransform> riderTrans = m_rider()->getComponent<SWTransform>();
		tvec3 pos = riderTrans()->getPosition();
		float frontFromRider = pos.x + WorldWidth;
		
		tuint patternNum = 1;
		char buf[256] = {0};
		if ( !m_coinPatternAD )
		{
			patternNum = 27;
			m_coinPatternAD = true;
		}
		else patternNum = SWMath.randomInt(1,27);
		sprintf( &buf[0], "patterns/coinpattern%d.txt", patternNum );

		SWHardRef<SWInputStream> is = SWAssets.loadBuffer( &buf[0] );
		SWInputStreamReader reader( is() );
		
		tvec2 step(0, SWMath.randomInt( (RoofY+GroundY)/2, RoofY) );
		tstring line;
		while ( reader.readLine( line ) )
		{
			step.y -= 15;
			step.x = frontFromRider;
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

private:

};

#endif // PlayScene_h__