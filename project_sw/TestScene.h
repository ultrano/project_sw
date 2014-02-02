#ifndef TestScene_h__
#define TestScene_h__

#include "SWGameContext.h"
#include "SWMatrix4x4.h"
#include "SWQuaternion.h"
#include "SWGameScene.h"
#include "SWGameObject.h"
#include "SWTransform.h"
#include "SWMeshFilter.h"
#include "SWMesh.h"
#include "SWVector2f.h"
#include "SWMeshRenderer.h"
#include "SWLog.h"
#include "SWDefines.h"
#include "SWMath.h"
#include "SWInput.h"
#include "SWParam.h"
#include "SWValue.h"
#include "SWActPlay.h"
#include "SWSpriteData.h"
#include "SWAction.h"
#include "SWActContinue.h"
#include "SWFileStream.h"
#include "SWUtil.h"
#include "SWCamera.h"
#include "SWShader.h"
#include "SWSocket.h"

#include "WIDefines.h"
#include "WIImage.h"
#include "WIFontChar.h"
#include "WIFontData.h"
#include "WIText.h"

#include <stdio.h>
#include <set>
#include <map>
#include <fstream>
#include <locale>
#include <codecvt>

class TestScene : public SWGameScene
{
	SW_RTTI( TestScene, SWGameScene );

	SWMatrix4x4 mat;
	void onAwake()
	{
		{
			SWHardRef<SWSocket> sock = new SWSocket;
			
			sock()->connect( "127.0.0.1", 10000 );
			
			SWHardRef<SWOutputStream> os = sock()->getOutputStream();
			SWHardRef<SWInputStream>  is = sock()->getInputStream();

			char buf[] = "are you there?";
			os()->write( (tbyte*)buf, sizeof(buf) );

			memset( buf, 0, sizeof(buf) );

			is()->read( (tbyte*)buf, sizeof(buf) );

			SWLog( (char*)buf );
		}
		//! set default camera
		{
			SWGameObject* go = new SWGameObject;
			SWCamera* cam = go->addComponent<SWCamera>();
			cam->orthoMode( SW_GC.getScreenWidth(), SW_GC.getScreenHeight(), 1, 1000 );
			cam->perspectiveMode( SWMath.angleToRadian(90), 1, 1, 1000 );
			SWCamera::mainCamera = cam;
		}

		{
			SWGameObject* go = new SWGameObject;
			WIImage* image = go->addComponent<WIImage>();
			image->setTexture( "cat3.png" );
			image->setSizeToTexture();
			SWTransform* transform = go->getComponent<SWTransform>();
			transform->setLocalPosition( SWVector3f( 100,100,500 ) );
			go->setName( "cat" );
			go->defineProp( "rot" );
			go->setProp( "rot", new SWNumber( 0 ) );
		}
			tarray<tbyte> buf;
			SWHardRef<SWFileInputStream> fis = new SWFileInputStream( SW_GC.assetPath( "read_test.txt" ) );
			buf.resize( fis()->available() + 1 );
			fis()->read( (tbyte*)&buf[0], buf.size() );
			
			SWObject::Ref fontJson = SW_GC.loadJson( "font/font1.fnt" );
			WIFontData* fontData = new WIFontData;
			fontData->load( fontJson() );
			fontJson()->destroy();

			SWGameObject* go = new SWGameObject;
			go->setName( "font" );
			WIText* text = go->addComponent<WIText>();
			text->setFont( fontData );
			//text->setText( SWUtil.utf8ToUnicode( (char*)&buf[0] ) );
			text->setFontSize( 40 );

			SWTransform* transform = go->getComponent<SWTransform>();
			transform->setLocalPosition( SWVector3f( 0,0,500 ) );
	}

	void onHandleTouch()
	{
		if ( !SWInput.getKey( SWInput.getLastKey() ) ) return;
		SWGameObject* go = find( "font" );
		WIText* text = go->getComponent<WIText>();
		tstring str = SWUtil.unicodeToUtf8( text->getText() );
		str += (char)SWInput.getLastKey();
		text->setText( SWUtil.utf8ToUnicode( str ) );
	}

	void onUpdate()
	{
		SWGameObject* go = find( "cat" );
		SWNumber* num = swrtti_cast<SWNumber>( go->getProp( "rot" ) );
		SWTransform* transform = go->getComponent<SWTransform>();
		SWQuaternion quat;
		quat.rotate( SWVector3f::axisY, num->getValue() );
		transform->setLocalRotate( quat );

		if ( SWInput.getKey('a') )
		{
			num->setValue( num->getValue() + (SWMath.pi/900) );
		}
		else if ( SWInput.getKey('d') )
		{
			num->setValue( num->getValue() - (SWMath.pi/900) );
		}
	}

	void onPostDraw()
	{
	}
};

#endif // TestScene_h__