#ifndef PlayScene_h__
#define PlayScene_h__

#include "SWHeaders.h"
#include "Rider.h"
#include "GasCloud.h"

class PlayScene : public SWGameScene
{
	SW_RTTI( PlayScene, SWGameScene );

public:

	SWWeakRef<Rider> m_rider;
	SWWeakRef<SWCamera> m_camera;

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
	}

	void onUpdate()
	{
	}

private:

};

#endif // PlayScene_h__