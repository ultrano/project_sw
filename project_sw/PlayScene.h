#ifndef PlayScene_h__
#define PlayScene_h__

#include "SWHeaders.h"
#include "Rider.h"
#include "GasCloud.h"
#include "Coin.h"
#include "GameValues.h"
#include "BackGround.h"

class PlayScene : public SWGameScene
{
	SW_RTTI( PlayScene, SWGameScene );

public:

	SWWeakRef<Rider> m_rider;
	SWWeakRef<SWCamera> m_camera;

	SWWeakRef<SWRigidBody2D> m_coinBasket;

	SWHardRef<SWGameObject> m_background;

	PlayScene()
	{
	}

	PlayScene( factory_constructor )
	{
	}

	void CameraUpdate()
	{
		if ( m_camera.isValid() == false ) return;
		if ( m_rider.isValid() == false ) return;
		tvec3 pos = m_rider()->getComponent<SWTransform>()->getPosition();
		pos.x += 150;
		pos.y = WorldHeight/2;
		pos.z = -500;

		SWTransform* trans = m_camera()->getComponent<SWTransform>();
		trans->setPosition( pos );

		pos.x -= WorldWidth/2;
		m_coinBasket()->setCenter( pos.xy() );
	}

	void onAwake()
	{
		SW_GC.registerFactory<Rider>();
		SW_GC.registerFactory<GasCloud>();
		SW_GC.registerFactory<Coin>();
		SW_GC.registerFactory<BackGround>();

		//! initialize values
		{
		}

		//! rider test
		{
			SWGameObject* go = new SWGameObject();
			Rider* rider = go->addComponent<Rider>();
			SWTransform* trans = go->getComponent<SWTransform>();
			trans->setPosition( tvec3( 0,GroundY,0 ) );
			m_rider = rider;
		}

		//! camera
		{
			tvec3 screenSize( SW_GC.getScreenWidth(), SW_GC.getScreenHeight(), 0 );
			
			SWGameObject* go = new SWGameObject;
			go->setName( "camera" );

			SWCamera* cam = go->addComponent<SWCamera>();
			cam->orthoMode( WorldWidth, WorldHeight, 1, 1000 );
			cam->getComponent<SWTransform>()->setLocalPosition( tvec3( 0, 0, -500 ) );
			cam->setClearColor( tcolor( 1,1,1,1 ) );
			cam->setClearFlags( SW_Clear_Color );
			
			m_camera = cam;
			go->addUpdateDelegator( GetDelegator( CameraUpdate ) );
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
			go->setName( "bank" );
			go->setActive( false );

			int count = 50;
			while ( count-- )
			{
				newCoin()->deposit();
			}
		}

		//! make coin test
		{
			SWGameObject* go = new SWGameObject();
			SWHardRef<SWAction> action = go->addComponent<SWAction>();
			SWActDelegate* act = new SWActDelegate( GetDelegator( MakeCoin ) );
			SWActSequence* seq = new SWActSequence();
			seq->addAct( new SWActDelay( 6 ) );
			seq->addAct( act );
			action()->setAct( "makeCoin", new SWActRepeat( seq ) );
			action()->play( "makeCoin" );
		}

		//! background test
		{
			m_background = new SWGameObject();
			m_background()->addComponent<BackGround>();
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
		SWTransform* bankTrans = findGO( "bank" )->getComponent<SWTransform>();

		if ( bankTrans->getChildrenCount() > 0 )
		{
			SWLog( "reuse coin" );
			SWTransform* coinTrans = bankTrans->getChildAt(0);
			coinTrans->setParent( NULL );
			coin = coinTrans->getComponent<Coin>();
		}
		else
		{
			coin = newCoin();
		}
		return coin;
	}
	void MakeCoin()
	{
		SWHardRef<SWTransform> riderTrans = m_rider()->getComponent<SWTransform>();
		tvec3 pos = riderTrans()->getPosition();
		float frontFromRider = pos.x + WorldWidth;
		
		char buf[256] = {0};
		sprintf( &buf[0], "patterns/coinpattern%d.txt", SWMath.randomInt(1,26) );

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
	}

private:

};

#endif // PlayScene_h__