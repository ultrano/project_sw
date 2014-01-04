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
		}
		//! set default camera
		{
			SWGameObject* go = new SWGameObject;
			SWCamera* cam = go->addComponent<SWCamera>();
			//cam->cameraMatrix.ortho( 0, SW_GC.getScreenWidth(), 0, SW_GC.getScreenHeight(),1000,-1000);
			cam->cameraMatrix.perspective( SWMath.pi/2,1,0,1000 );
			SWVector3f vec = SWVector3f( 0,0,500 ) * cam->cameraMatrix;
			SWCamera::mainCamera = cam;
		}
		{
			tstring test = SWUtil.unicodeToUtf8( L"asdf123" );
		}
		{
			SWGameObject* go = new SWGameObject;
			WIImage* image = go->addComponent<WIImage>();
			image->setTexture( "cat3.png" );
			image->setSizeToTexture();
			SWTransform* transform = go->getComponent<SWTransform>();
			transform->setLocalPosition( SWVector3f( 0,0,500 ) );
			go->setName( "cat" );
			go->defineProp( "rot" );
			go->setProp( "rot", new SWNumber( 0 ) );
		}
			tarray<tbyte> buf;
			SWHardRef<SWFileInputStream> fis = new SWFileInputStream( SW_GC.assetPath( "read_test.txt" ) );
			buf.resize( fis()->size() + 1 );
			fis()->read( (tbyte*)&buf[0], buf.size() );
			
			SWHardRef<SWObject> fontJson = SW_GC.loadJson( "font/font1.fnt" );
			WIFontData* fontData = new WIFontData;
			fontData->load( fontJson() );

			SWGameObject* go = new SWGameObject;
			go->setName( "font" );
			WIText* text = go->addComponent<WIText>();
			text->setFont( fontData );
			text->setText( SWUtil.utf8ToUnicode( (char*)&buf[0] ) );
			text->setFontSize( 40);

			SWTransform* transform = go->getComponent<SWTransform>();
			transform->setLocalPosition( SWVector3f( 0,0,500 ) );

	
	}

	void onHandleTouch()
	{
		SWCamera* cam = SWCamera::mainCamera();
		SWTransform* trans = cam->getComponent<SWTransform>();
		static float near = 0;
		near +=SWInput.getDeltaX();
		SWLog( "near %f", near );
		switch ( SWInput.getTouchState() )
		{
			/*
		case SW_TouchRelease   : 
			{
				float halfW = SW_GC.getScreenWidth()/2;
				float halfH = SW_GC.getScreenHeight()/2;
				cam->cameraMatrix.ortho( -halfW, halfW, halfH, -halfH,500,1000);
			SWVector3f vec = SWVector3f( 0,0,-500 ) * cam->cameraMatrix;
			 vec = SWVector3f( 0,0,500 ) * cam->cameraMatrix;
			}break;
			*/
		case SW_TouchMove : cam->cameraMatrix.perspective( SWMath.pi/2,1, near,1000 ); break;
		}
		SWVector3f vec = SWVector3f( 0,0,500 ) * cam->cameraMatrix;
		SWLog( "vec %f", vec.z );
		return;
		SWVector3f pos = trans->getLocalPosition();
		pos.z += SWInput.getDeltaX();
		trans->setLocalPosition( pos );
		SWLog( "pos %f", pos.z );
	}

	void onUpdate()
	{
		SWGameObject* go = find( "cat" );
		SWNumber* num = swrtti_cast<SWNumber>( go->getProp( "rot" ) );
		SWTransform* transform = go->getComponent<SWTransform>();
		SWQuaternion quat;
		quat.rotate( SWVector3f::axisY, num->getValue() );
		transform->setLocalRotate( quat );
		num->setValue( num->getValue() + (SWMath.pi/300) );
	}

	void onPostDraw()
	{
	}
};

#endif // TestScene_h__