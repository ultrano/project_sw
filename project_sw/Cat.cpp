#include "Cat.h"
#include "SWGameContext.h"
#include "SWGameObject.h"
#include "SWMeshRenderer.h"
#include "SWMeshFilter.h"
#include "SWMesh.h"
#include "TVector2f.h"
#include "TVector3f.h"
#include "SWTransform.h"
#include "SWMath.h"
#include "SWTime.h"
#include "SWAction.h"
#include "SWActDestroy.h"
#include "SWNumber.h"
#include "SWString.h"
#include "SWTable.h"
#include "SWActPlay.h"
#include "SWActContinue.h"

#include "WIImage.h"
#include "WIDefines.h"

void Cat::onAwake()
{
	SWTransform* transform = gameObject()->getComponent<SWTransform>();
	SWAction* action = gameObject()->addComponent<SWAction>();
	WIImage* image = gameObject()->addComponent<WIImage>();
	SWTable* spriteData = swrtti_cast<SWTable>(gameObject()->getProp( "spriteData" ));
	SWArray* anim = swrtti_cast<SWArray>(spriteData->find( "idle" ));
	float duration = swrtti_cast<SWNumber>( anim->get(0) )->getValue();
	SWArray* regions = swrtti_cast<SWArray>( anim->get(1) );

	switch ( SWMath.randomInt(1,3) )
	{
	case 1: image->setTexture( "cat1.png" ); break;
	case 2: image->setTexture( "cat2.png" ); break;
	case 3: image->setTexture( "cat3.png" ); break;
	}

	SWTable* config = swrtti_cast<SWTable>(SW_GC.findItem( "config" ));
	
	SWString* anchoXItem = swrtti_cast<SWString>(config->find("catAnchorX"));
	SWString* anchoYItem = swrtti_cast<SWString>(config->find("catAnchorY"));

	SWNumber* width = swrtti_cast<SWNumber>( config->find( "catWidth" ) );
	SWNumber* height = swrtti_cast<SWNumber>( config->find( "catHeight" ) );

	SWNumber* posX = swrtti_cast<SWNumber>( config->find( "catX" ) );
	SWNumber* posY = swrtti_cast<SWNumber>( config->find( "catY" ) );
	
	int anchoX = UI_Center;
	int anchoY = UI_Center;

	if ( anchoXItem->getValue() == "left" ) anchoX = UI_Left;
	else if ( anchoXItem->getValue() == "right" ) anchoX = UI_Right;

	if ( anchoYItem->getValue() == "top" ) anchoY = UI_Top;
	else if ( anchoYItem->getValue() == "bottom" ) anchoY = UI_Bottom;

	image->setSize( width->getValue(), height->getValue() );
	image->setAlignV( anchoX );
	image->setAlignH( anchoY );

	transform->setLocalPosition( TVector3f( posX->getValue(), posY->getValue(), 0 ) );

	
	SWNumber* catLife = swrtti_cast<SWNumber>(config->find("catLife"));
	SWNumber* catSpeed = swrtti_cast<SWNumber>(config->find("catSpeed"));
	m_speed = catSpeed->getValue();
	action->runAct( new SWActDestroy( catLife->getValue() ) );
	action->runAct( new SWActContinue( new SWActPlay( regions, duration ) ) );
}

void Cat::onUpdate()
{
	float deltaTime = SWTime.getDeltaTime();
	SWTransform* transform = gameObject()->getComponent<SWTransform>();
	TVector3f pos = transform->getLocalPosition();
	pos.x -= deltaTime * m_speed;
	transform->setLocalPosition( pos );
}
