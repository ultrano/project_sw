#ifndef PlayScene_h__
#define PlayScene_h__

#include "SWHeaders.h"
#include "Rider.h"
#include "GasCloud.h"
#include "Coin.h"

class PlayScene : public SWGameScene
{
	SW_RTTI( PlayScene, SWGameScene );

public:

	SWWeakRef<Rider> m_rider;
	SWWeakRef<SWCamera> m_camera;

	SWHardRef<SWGameObject> m_originCoin;

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
		pos.x += 80;
		pos.y = 70;
		pos.z = -500;

		m_camera()->getComponent<SWTransform>()->setPosition( pos );

	}

	void onAwake()
	{
		SW_GC.registerFactory<Rider>();
		SW_GC.registerFactory<GasCloud>();
		SW_GC.registerFactory<Coin>();

		//! rider test
		{
			SWGameObject* go = new SWGameObject();
			Rider* rider = go->addComponent<Rider>();
			SWTransform* trans = go->getComponent<SWTransform>();
			trans->setPosition( tvec3( 0,0,0 ) );
			m_rider = rider;
		}

		//! camera
		{
			tvec3 screenSize( SW_GC.getScreenWidth(), SW_GC.getScreenHeight(), 0 );
			
			SWGameObject* go = new SWGameObject;
			go->setName( "camera" );

			SWCamera* cam = go->addComponent<SWCamera>();
			cam->orthoMode( 256, 140, 1, 1000 );
			cam->getComponent<SWTransform>()->setLocalPosition( tvec3( 0, 0, -500 ) );
			cam->setClearColor( tcolor( 1,1,1,1 ) );
			cam->setClearFlags( SW_Clear_Color );
			
			m_camera = cam;
			go->addUpdateDelegator( GetDelegator( CameraUpdate ) );
		}

		//! make coin test
		{
			SWGameObject* go = new SWGameObject();
			SWHardRef<SWAction> action = go->addComponent<SWAction>();
			SWActDelegate* act = new SWActDelegate( GetDelegator( MakeCoin ) );
			SWActSequence* seq = new SWActSequence();
			seq->addAct( new SWActDelay( 1 ) );
			seq->addAct( act );
			action()->setAct( "makeCoin", new SWActRepeat( seq ) );
			action()->play( "makeCoin" );


			m_originCoin = new SWGameObject();
			m_originCoin()->addComponent<Coin>();
			
			SWHardRef<SWTransform> trans = m_originCoin()->getComponent<SWTransform>();
			trans()->setPosition( tvec3(0, -100, 0) );
		}
	}

	void MakeCoin()
	{
		SWHardRef<SWTransform> riderTrans = m_rider()->getComponent<SWTransform>();
		tvec3 pos = riderTrans()->getPosition();
		pos.x += 150;

		SWHardRef<SWGameObject> go = m_originCoin()->clone<SWGameObject>();
		SWHardRef<SWTransform> trans = go()->getComponent<SWTransform>();
		trans()->setPosition( pos );
	}

	void onUpdate()
	{
	}

private:

};

#endif // PlayScene_h__