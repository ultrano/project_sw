#include "VehicleItem.h"
#include "PlayScene.h"


VehicleItem::VehicleItem( factory_constructor )
{
}

VehicleItem::~VehicleItem()
{
}


void VehicleItem::onAwake()
{
	__super::onAwake();

	gameObject()->setName( "VehicleItem" );

	SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "textures/effects.png" );

	SWSpriteRenderer* renderer = gameObject()->addComponent<SWSpriteRenderer>();
	renderer->setSprite( atlas()->find( "vehicle_item" ) );

	SWRectCollider2D* collider = gameObject()->addComponent<SWRectCollider2D>();
	collider->setSize( tvec2( 51, 51 ) );
}

void VehicleItem::onStart()
{
}

void VehicleItem::onCollision( SWCollision2D* coll )
{
	bool goToPool = false;
	SWCollider2D* collider = coll->collider();
	SWGameObject* go = collider->gameObject();

	if ( go->getName() == "Basket" )
	{
		goToPool = true;
	}
	else if ( go->getName() == "Character" )
	{
		goToPool = true;
		PlayScene* scene = swrtti_cast<PlayScene>( SW_GC.getScene() );
		scene->showPannel();
	}

	if ( goToPool )
	{
		SWGameObject* poolGO = SW_GC.getScene()->findGO( "Pool" );
		getComponent<SWTransform>()->setParent( poolGO->getComponent<SWTransform>() );
	}
}