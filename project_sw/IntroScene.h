#ifndef IntroScene_h__
#define IntroScene_h__

#include "SWHeaders.h"

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

	void onAwake()
	{
		SW_GC.registerFactory<IntroScene>();

		//! set default camera
		{
			tvec3 screenSize( SW_GC.getScreenWidth(), SW_GC.getScreenHeight(), 0 );
			
			SWGameObject* go = new SWGameObject;
			go->setName( "Camera" );

			SWCamera* cam = go->addComponent<SWCamera>();
			cam->orthoMode( screenSize.x, screenSize.y, 1, 1000 );
			cam->getComponent<SWTransform>()->setLocalPosition( tvec3( 0, 0, -500 ) );
			cam->setClearColor( tcolor( 1,1,1,1 ) );
			cam->setClearFlags( SW_Clear_Color );
		}

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

		//! new font test

		SWHardRef<SWInputStream> is = SWAssets.loadBuffer( "fonts/test.fnt" );
		SWInputStreamReader reader( is() );

		int id,x,y,w,h,xoffset,yoffset,xadvence,page,chnl;
		tstring line;
		reader.readLine( line );
		reader.readLine( line );
		reader.readLine( line );
		reader.readLine( line );
		while ( reader.readLine( line ) )
		{
			sscanf( line.c_str(), "char id=%d x=%d y=%d width=%d height=%d xoffset=%d yoffset=%d xadvance=%d page=%d chnl=%d"
				, &id, &x, &y, &w, &h, &xoffset, &yoffset, &xadvence, &page, &chnl);
		}
	}
	void onEndLogo()
	{
		SW_GC.setNextScene( new PlayScene );
		//SW_GC.setNextScene( new TestScene );
	}
};

#endif // IntroScene_h__