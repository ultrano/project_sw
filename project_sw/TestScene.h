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

#include "WIDefines.h"
#include "WIImage.h"

class TestScene : public SWGameScene
{
	SW_RTTI( TestScene, SWGameScene );

	SWMatrix4x4 mat;
	void onAwake()
	{
		SWGameObject* go = new SWGameObject;
		WIImage* image = go->addComponent<WIImage>();
		image->setTexture( "cat3.png" );
		image->setSizeToTexture();
		SWTransform* transform = go->getComponent<SWTransform>();
		transform->setLocalPosition( SWVector3f( 300,300,0 ) );
	}

	void onUpdate()
	{
		
		mat.transform
			( SWVector3f::one*SWMath.pingPong( SWTime.getTime(), 5 )/5
			, SWQuaternion().rotate( SWVector3f::axisZ, SWMath.pi/4 )
			, SWVector3f::zero );
		SW_GC.setTextureMatrix( mat );
	}
};

#endif // TestScene_h__