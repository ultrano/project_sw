#include "BackGround.h"

BackGround::BackGround()
{

}

BackGround::BackGround( factory_constructor )
{

}

BackGround::~BackGround()
{

}

void BackGround::onAwake()
{
	__super::onAwake();

	m_camera = SW_GC.getScene()->findGO( "WorldCamera" );
	SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "textures/background.png" );
	SWHardRef<SWTransform> trans = gameObject()->getComponent<SWTransform>();
	SWSprite* sprite = atlas()->find( "background" );
	const tvec2& spriteSize = sprite->getSize();

	//! size scaling
	{
		SWTransform* cameraTrans = m_camera()->getComponent<SWTransform>();
		tvec3 pos = cameraTrans->getPosition();
		trans()->setPosition( tvec3( pos.x, WorldHeight/2, -100 ) );
		trans()->setLocalScale( tvec3( WorldWidth/spriteSize.x, WorldHeight/spriteSize.y, 1 ) );
	}

	//! first image
	{
		SWGameObject* imageGO = new SWGameObject;
		m_images[0] = imageGO;

		SWHardRef<SWSpriteRenderer> renderer = imageGO->addComponent<SWSpriteRenderer>();
		renderer()->setSprite( sprite );

		imageGO->getComponent<SWTransform>()->setParent( trans() );
	}

	//! second image
	{
		SWGameObject* imageGO = new SWGameObject;
		m_images[1] = imageGO;

		SWHardRef<SWSpriteRenderer> renderer = imageGO->addComponent<SWSpriteRenderer>();
		renderer()->setSprite( sprite );

		SWHardRef<SWTransform> imageTrans = imageGO->getComponent<SWTransform>();
		imageTrans()->setParent( trans() );

		imageTrans()->setPosition( tvec3( spriteSize.x, 0, 0 ) );
	}

}

void BackGround::onUpdate()
{
	if ( m_camera.isValid() == false ) return;

	tvec3 cameraPos = m_camera()->getComponent<SWTransform>()->getPosition();
	
	SWGameObject* frontImg = m_images[0]();
	SWHardRef<SWTransform> imgTrans = frontImg->getComponent<SWTransform>();
	tvec3 imgPos = imgTrans()->getPosition();

	SWSpriteRenderer* renderer = frontImg->getComponent<SWSpriteRenderer>();
	float spriteWidth = renderer->getSprite()->getSize().x;

	if ( (cameraPos.x - imgPos.x) >= WorldWidth )
	{
		imgTrans()->move( spriteWidth*2, 0, 0 );

		m_images[0] = m_images[1];
		m_images[1] = frontImg;
	}

}