#ifndef CatAss_h__
#define CatAss_h__

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

#include "WIDefines.h"
#include "WIImage.h"
#include "WIFontChar.h"
#include "WIFontData.h"
#include "WIText.h"

#include "Cat.h"
#include "Ball.h"
#include "BallGenerator.h"
#include "CatGenerator.h"

#include "json/json.h"

#include <stdio.h>
#include <set>
#include <map>
#include <fstream>


using namespace std;

SWHardRef<SWObject> convertJsonValue( const Json::Value& value );

class GameMainScene : public SWGameScene
{
	SW_RTTI( GameMainScene, SWGameScene );
	SWHardRef<SWGameObject> target;

	float m_minAngle;
	float m_maxAngle;
	void makeImages( SWValue* images )
	{
		if ( images->isArray() == false ) return;

		for ( int i = 0 ; i < images->count() ; ++i )
		{
			SWValue* data   = images->get(i);
			if ( data->isTable() == false ) continue;

			SWValue* name   = data->find( "name" );
			SWValue* image  = data->find( "image" );
			SWValue* size   = data->find( "size" );
			SWValue* pos    = data->find( "pos" );
			SWValue* rotate = data->find( "rotate" );
			SWValue* anchor = data->find( "anchor" );

			SWGameObject* go = new SWGameObject;
			go->setName( name->asString() );
			SWTransform* transform = go->getComponent<SWTransform>();
			transform->setLocalPosition( SWVector3f( pos->get(0)->asNumber(), pos->get(1)->asNumber(), 0 ) );
			transform->setLocalRotate( SWQuaternion().rotate( SWVector3f::axisZ, SWMath.angleToRadian( rotate->asNumber() ) ) );

			WIImage* uiImage = go->addComponent<WIImage>();
			uiImage->setTexture( image->asString() );
			uiImage->setSize( size->get(0)->asNumber(), size->get(1)->asNumber() );

			if ( anchor->get(0)->asString() == "left" ) uiImage->setAlignV( UI_Left );
			else if ( anchor->get(0)->asString() == "right" ) uiImage->setAlignV( UI_Right );
			else uiImage->setAlignV( UI_Center );

			if ( anchor->get(1)->asString() == "top" ) uiImage->setAlignH( UI_Top );
			else if ( anchor->get(1)->asString() == "bottom" ) uiImage->setAlignH( UI_Bottom );
			else uiImage->setAlignH( UI_Center );
		}
	}

	void onAwake()
	{
		SWTable* config = swrtti_cast<SWTable>(SW_GC.findItem( "config" ));

		SWNumber* width = swrtti_cast<SWNumber>(config->find( "width" ));
		SWNumber* height = swrtti_cast<SWNumber>(config->find( "height" ));
		SWNumber* ratX = swrtti_cast<SWNumber>(config->find( "ratX" ));
		SWNumber* ratY = swrtti_cast<SWNumber>(config->find( "ratY" ));
		SWNumber* ratScaleX = swrtti_cast<SWNumber>(config->find( "ratScaleX" ));
		SWNumber* ratScaleY = swrtti_cast<SWNumber>(config->find( "ratScaleY" ));
		SWNumber* fireHoleX = swrtti_cast<SWNumber>(config->find( "fireHoleX" ));
		SWNumber* fireHoleY = swrtti_cast<SWNumber>(config->find( "fireHoleY" ));

		m_maxAngle = swrtti_cast<SWNumber>(config->find( "maxAngle" ))->getValue();
		m_minAngle = swrtti_cast<SWNumber>(config->find( "minAngle" ))->getValue();

		makeImages( (SWValue*)config->find("images") );



		{
			SWGameObject* go = new SWGameObject;
			go->addComponent<CatGenerator>();
			go->setName( "catGenerator" );
		}

		{
			SWGameObject* ratGO = new SWGameObject;
			{
				ratGO->setName( "rat" );
				SWTransform* transform = ratGO->getComponent<SWTransform>();
				transform->setLocalPosition( SWVector3f( ratX->getValue(), ratY->getValue(),0 ) );
				transform->setLocalScale( SWVector3f( ratScaleX->getValue(), ratScaleY->getValue(), 1 ) );
			}

			{
				SWGameObject* go = new SWGameObject;
				SWTransform* transform = go->getComponent<SWTransform>();
				WIImage* image = go->addComponent<WIImage>();
				image->setTexture( "rat.png" );
				image->setSizeToTexture();
				image->setAlignH( UI_Bottom );
				transform->setParent( ratGO->getComponent<SWTransform>() );
			}

			{
				SWGameObject* go = new SWGameObject;
				go->addComponent<BallGenerator>();
				go->setName( "generator" );
				SWTransform* transform = go->getComponent<SWTransform>();
				transform->setParent( ratGO->getComponent<SWTransform>() );
				transform->setLocalPosition( SWVector3f( fireHoleX->getValue(), fireHoleY->getValue(),0 ) );
			}
		}


		{
			SWGameObject* go = find( "siso_up" );
			go->addUpdateDelegate( GetDelegate( onSisoUpUpdate ) );
		}
	}

	void onSisoUpUpdate()
	{

	}

	void onUpdate()
	{
		SWGameObject* ratGO = find( "rat" );
		SWTransform* ratTrans = ratGO->getComponent<SWTransform>();

		SWGameObject* go = ratTrans->find("generator")->gameObject();
		BallGenerator* gen = go->getComponent<BallGenerator>();

		switch ( SWInput.getTouchState() )
		{
		case SW_TouchRelease : gen->turnOn = false; break;
		case SW_TouchPress :   gen->turnOn = true;
		case SW_TouchMove :
			{
				SWVector3f worldPos = ratTrans->getLocalPosition() * ratTrans->getWorldMatrix();
				SWVector3f touchPos( SWInput.getTouchX(), SWInput.getTouchY(), 0 );
				SWVector2f delta = (worldPos - touchPos).xy;
				float power = delta.length();
				gen->force = delta;
				delta = delta.normalize();
				float maxRadian = SWMath.angleToRadian( m_maxAngle );
				float minRadian = SWMath.angleToRadian( m_minAngle );
				float radian = SWMath.atan( delta.y, delta.x );
				SWLog( "angle : %f", SWMath.radianToAngle( radian ) );

				radian = ( radian > maxRadian )? maxRadian : ( radian < minRadian )? minRadian : radian;
				gen->force = SWVector2f( SWMath.cos(radian), SWMath.sin(radian) ) * power;
				ratTrans->setLocalRotate( SWQuaternion().rotate( SWVector3f::axisZ, radian ) );
			}
			break;
		}
	}
};

class TitleScene : public SWGameScene
{
	SW_RTTI( TitleScene, SWGameScene );
	void onAwake()
	{
		SWTable* config = swrtti_cast<SWTable>(SW_GC.findItem( "config" ));

		SWNumber* width = swrtti_cast<SWNumber>(config->find( "width" ));
		SWNumber* height = swrtti_cast<SWNumber>(config->find( "height" ));
		//! background
		{
			SWGameObject* go = new SWGameObject;
			SWTransform* transform = go->getComponent<SWTransform>();
			WIImage* image = go->addComponent<WIImage>();

			image->setSize( width->getValue(), height->getValue() );
			image->setTexture( "background/title_01.png" );
			image->setAlign( UI_Left, UI_Top );

			//transform->setLocalPosition( SWVector3f( 480,320,0 ) );
		}
	}

	void onHandleTouch()
	{
		if ( SWInput.getTouchState() != SW_TouchPress ) return;
		SW_GC.setNextScene( new GameMainScene );
	}
};

class InitScene : public SWGameScene
{
	SW_RTTI( InitScene, SWGameScene );

	void onAwake()
	{
		std::ifstream ifs( "../resource/config.json" );

		Json::Reader reader;
		Json::Value root;
		reader.parse( (std::istream&)ifs, root );

		SWHardRef<SWObject> rootObj = convertJsonValue( root );

		SW_GC.storeItem( "config", rootObj() );
		
	}
	void onUpdate()
	{
		SW_GC.setNextScene( new TitleScene );
	}
};


#endif // CatAss_h__