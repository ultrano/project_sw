#include "Cat.h"
#include "SWGameContext.h"
#include "SWGameObject.h"
#include "SWMeshRenderer.h"
#include "SWMeshFilter.h"
#include "SWMesh.h"
#include "SWVector2f.h"
#include "SWVector3f.h"
#include "SWTransform.h"
#include "SWMath.h"
#include "SWTime.h"
#include "SWAction.h"
#include "SWActDestroy.h"
#include "UIImage.h"

void Cat::onAwake()
{
	SWTransform* transform = gameObject()->getComponent<SWTransform>();
	SWAction* action = gameObject()->addComponent<SWAction>();
	UIImage* image = gameObject()->addComponent<UIImage>();

	switch ( SWMath.randomInt(1,3) )
	{
	case 1: image->setTexture( "cat1.png" ); break;
	case 2: image->setTexture( "cat2.png" ); break;
	case 3: image->setTexture( "cat3.png" ); break;
	}
	
	image->setSizeToTexture( 1.0f/8.0f, 1.0f/8.0f );

	transform->setLocalPosition( SWVector3f( 800, 525, 0 ) );

	action->setAct( new SWActDestroy( 15 ) );
}

void Cat::onUpdate()
{
	float deltaTime = SWTime.getDeltaTime();
	SWTransform* transform = gameObject()->getComponent<SWTransform>();
	SWVector3f pos = transform->getLocalPosition();
	pos.x -= deltaTime * 10;
	float limit = SWMath.pi/18.0f;
	float angle = SWMath.pingPong( SWTime.getTime(), limit)-limit/2;
	transform->setLocalPosition( pos );
	transform->setLocalRotate( SWQuaternion().rotate(SWVector3f::axisZ, angle ) );
}
