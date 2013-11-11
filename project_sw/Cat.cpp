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
void Cat::onAwake()
{
	SWTransform* transform = gameObject()->getComponent<SWTransform>();
	SWMeshRenderer* renderer = gameObject()->addComponent<SWMeshRenderer>();
	SWMeshFilter* filter = gameObject()->addComponent<SWMeshFilter>();

	filter->setMesh( swrtti_cast<SWMesh>(SW_GC.findItem("unitRectMesh")) );
	switch ( SWMath.randomInt(1,3) )
	{
	case 1: renderer->setTexture( SW_GC.loadTexture( "cat1.png" ) ); break;
	case 2: renderer->setTexture( SW_GC.loadTexture( "cat2.png" ) ); break;
	case 3: renderer->setTexture( SW_GC.loadTexture( "cat3.png" ) ); break;
	}
	
	transform->setLocalScale( SWVector3f(50,50,1) );
	transform->setLocalPosition( SWVector3f( 800, 500, 0 ) );

	lifeTime = 10;
}

void Cat::onUpdate()
{
	SWTransform* transform = gameObject()->getComponent<SWTransform>();
	SWVector3f pos = transform->getLocalPosition();
	pos.x -= SW_GC.deltaTime() * 10;
	float limit = SWMath.pi/18.0f;
	float angle = SWMath.pingPong( SW_GC.awakeTime(), limit)-limit/2;
	transform->setLocalPosition( pos );
	transform->setLocalRotate( SWQuaternion().rotate(SWVector3f::axisZ, angle ) );
	if ( (lifeTime -= SW_GC.deltaTime()) < 0 ) gameObject()->destroy();
}
