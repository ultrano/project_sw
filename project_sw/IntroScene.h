#ifndef IntroScene_h__
#define IntroScene_h__

#include "SWGameScene.h"
#include "SWGameContext.h"
#include "SWGameObject.h"
#include "SWCamera.h"
#include "SWTransform.h"
#include "WIImage.h"
#include "SWAssets.h"
#include "SWTime.h"
#include "TestScene.h"

#include "SWActAlphaTo.h"
#include "SWActDelegate.h"
#include "SWActSequence.h"

#include "SWUtil.h"

class IntroScene : public SWGameScene
{
	SW_RTTI( IntroScene, SWGameScene );

	float m_time;
	float m_remain;

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

			SWCamera::mainCamera = cam;
		}

		{
			SWGameObject* go = new SWGameObject;
			go->setName( "logo" );

			WIImage* image = go->addComponent<WIImage>();
			image->setTexture( SWAssets.loadTexture( "logo.png" ) );
			image->setSize( 320, 480 );

			SWAction* action = go->addComponent<SWAction>();
			SWActSequence* seq = new SWActSequence();
			seq->addAct( new SWActAlphaTo( 1, 0 ) );
			seq->addAct( new SWActDelegate( GetDelegator( onEndLogo ) ) );
			action->setAct( "logo", seq );
			action->play( "logo" );
		}

		{
			m_time = 4;
			m_remain = m_time;
		}
	}
	void onEndLogo()
	{
		SW_GC.setNextScene( new TestScene );
	}
};

#endif // IntroScene_h__