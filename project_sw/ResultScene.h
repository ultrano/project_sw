#ifndef ResultScene_h__
#define ResultScene_h__

#include "SWHeaders.h"

class ResultScene : public SWGameScene
{
	SW_RTTI( ResultScene, SWGameScene );

private:

	tuint m_meter;
	tuint m_score;
	SWHardRef<SWFontInfo> m_fontInfo;
	SWHardRef<SWTexture>  m_fontTexture;
public:

	ResultScene( tuint meter, tuint score )
		: m_meter( meter )
		, m_score( score )
	{
	};

	ResultScene(factory_constructor)
	{
	};

	void onAwake()
	{
		//! font info & font texture
		{
			m_fontInfo    = SWAssets.loadFontInfo( "fonts/Jetpackia.fnt" );
			m_fontTexture = SWAssets.loadTexture( "fonts/Jetpackia.png" );

		}
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
			SWFontRenderer* renderer = go->addComponent<SWFontRenderer>();
			renderer->setFontInfo( m_fontInfo() );
			renderer->setFontTexture( m_fontTexture() );
			renderer->setText( "%d", m_meter );
		}
	}

	virtual void onFixedRateUpdate()
	{
		if ( isButtonPushed() )
		{
			SWHardRef<SWObject> instance = SW_GC.newInstance( "PlayScene" );
			SWGameScene* scene = swrtti_cast<SWGameScene>( instance() );
			SW_GC.setNextScene( scene );
		}
	}
};

#endif // ResultScene_h__