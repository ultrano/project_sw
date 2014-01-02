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
			SWGameObject* go = new SWGameObject;
			WIImage* image = go->addComponent<WIImage>();
			image->setTexture( "cat3.png" );
			image->setSizeToTexture();
			SWTransform* transform = go->getComponent<SWTransform>();
			transform->setLocalPosition( SWVector3f( 300,300,0 ) );
		}
			tarray<tbyte> buf;
			SWHardRef<SWFileInputStream> fis = new SWFileInputStream( SW_GC.assetPath( "read_test.txt" ) );
			buf.resize( fis()->size() + 1 );
			fis()->read( (tbyte*)&buf[0], buf.size() );
			std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
			tstring str = (char*)&buf[0];
			str.assign( (char*)&buf[0], (char*)&buf[buf.size()-1] );

			twstring wstr = conv.from_bytes( str );
			
			SWHardRef<SWObject> fontJson = SW_GC.loadJson( "font/font1.fnt" );
			WIFontData* fontData = new WIFontData;
			fontData->load( fontJson() );

			SWGameObject* go = new SWGameObject;
			go->setName( "font" );
			WIText* text = go->addComponent<WIText>();
			text->setFont( fontData );
			text->setText( wstr );
			text->setFontSize( 40);

			SWTransform* transform = go->getComponent<SWTransform>();
			transform->setLocalPosition( SWVector3f( 300,300,0 ) );
	}

	void onHandleTouch()
	{
		WIText* text = find("font")->getComponent<WIText>();
		text->setFontSize( text->getFontSize() + 1 );
	}

	void onUpdate()
	{
	}
};

#endif // TestScene_h__