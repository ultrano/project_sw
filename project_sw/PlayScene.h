#ifndef PlayScene_h__
#define PlayScene_h__

#include "SWHeaders.h"

#include "GameValues.h"

#include "Rider.h"
#include "GasCloud.h"
#include "Coin.h"
#include "BackGround.h"
#include "ScoreBoard.h"

void registerGameAppFactories()
{
	SW_GC.registerFactory<Rider>();
	SW_GC.registerFactory<GasCloud>();
	SW_GC.registerFactory<Coin>();
	SW_GC.registerFactory<BackGround>();
	SW_GC.registerFactory<ScoreBoard>();
}

class PlayScene : public SWGameScene
{
	SW_RTTI( PlayScene, SWGameScene );

public:

	SWWeakRef<Rider> m_rider;
	SWWeakRef<SWCamera> m_camera;

	SWWeakRef<SWRigidBody2D> m_coinBasket;
	SWHardRef<SWGameObject> m_background;

	bool m_coinPatternAD;

	PlayScene()
	{
	}

	PlayScene( factory_constructor )
	{
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

	void onAwake()
	{
		registerGameAppFactories();

		tvec3 screenSize( SW_GC.getScreenWidth(), SW_GC.getScreenHeight(), 0 );

		//! initialize values
		{
			m_coinPatternAD = false;
		}

		//! rider 
		{
			SWGameObject* go = new SWGameObject();
			go->setName( "Rider" );

			Rider* rider = go->addComponent<Rider>();
			SWTransform* trans = go->getComponent<SWTransform>();
			trans->setPosition( tvec3( 0,GroundY,0 ) );
			m_rider = rider;
		}

		//! rider shadow
		{
			SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "textures/runner.png" );
			SWGameObject* go = new SWGameObject;
			
			SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
			renderer->setSprite( atlas()->find( "shadow" ) );
			
			go->addUpdateDelegator( GetDelegator( riderShadowUpdate ) );
		}

		//! rider camera
		{
			SWGameObject* go = new SWGameObject;
			go->setName( "RiderCamera" );

			SWCamera* cam = go->addComponent<SWCamera>();
			cam->orthoMode( WorldWidth, WorldHeight, 1, 1000 );
			cam->getComponent<SWTransform>()->setLocalPosition( tvec3( 0, 0, -500 ) );
			cam->setClearColor( tcolor( 1,1,1,1 ) );
			cam->setClearFlags( SW_Clear_Color );
			cam->setDepth( 0 );
			
			go->addUpdateDelegator( GetDelegator( riderCameraUpdate ) );
		}

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

		//! coin bank (for pooling)
		{
			SWGameObject* go = new SWGameObject();
			go->setName( "Bank" );
			go->setActive( false );

			int count = 100;
			while ( count-- ) newCoin()->deposit();
		}

		//! timer for making coin-patterns
		{
			SWGameObject* go = new SWGameObject();
			SWHardRef<SWAction> action = go->addComponent<SWAction>();
			SWActSequence* seq = new SWActSequence();
			seq->addAct( new SWActDelay( 6 ) );
			seq->addAct( new SWActDelegate( GetDelegator( makeCoinPattern ) ) );
			action()->setAct( "makeCoin", new SWActRepeat( seq ) );
			action()->play( "makeCoin" );
		}

		//! background looper
		{
			m_background = new SWGameObject();
			m_background()->addComponent<BackGround>();
		}

		//! gas cloud pool
		{
			SWGameObject* go = new SWGameObject();
			go->setName( "GasCloudPool" );
			go->setActive( false );
		}

		//! UI
		{
			SWGameObject* go = new SWGameObject;
			go->setName( "CoinScore" );
			go->setLayerName( "UI" );

			SWHardRef<SWFontInfo> info = SWAssets.loadFontInfo( "fonts/test.fnt" );
			SWHardRef<SWTexture> texture = SWAssets.loadTexture( "fonts/test.png" );
			SWFontRenderer* renderer = go->addComponent<SWFontRenderer>();
			renderer->setFontInfo( info() );
			renderer->setFontTexture( texture() );
			renderer->setText( "111111111" );

			SWTransform* trans = go->getComponent<SWTransform>();
			trans->setPosition( tvec3( -screenSize.x/2, (screenSize.y/2)-info()->getLineHeight(), 0 ) );
		}
		
		//! UI camera
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
		SWTransform* bankTrans = findGO( "Bank" )->getComponent<SWTransform>();

		if ( SWTransform* coinTrans = bankTrans->getChildAt(0) )
		{
			coin = coinTrans->getComponent<Coin>();
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
		
		tvec2 step(0,RoofY);
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

	void onUpdate()
	{
		if ( SWInput.getKey( 'p' ) )
		{
			SWHardRef<SWObject> object = SW_GC.newInstance( "IntroScene" );
			SWHardRef<SWGameScene> scene = swrtti_cast<SWGameScene>( object() );
			SW_GC.setNextScene( scene() );
		}
	}

private:

};

#endif // PlayScene_h__