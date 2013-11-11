#include "Ball.h"
#include "SWTransform.h"
#include "SWGameContext.h"
#include "SWGameObject.h"
#include "SWMeshRenderer.h"
#include "SWMeshFilter.h"
#include "SWMesh.h"
#include "SWVector2f.h"
#include "SWMath.h"

void Ball::onAwake()
{
	SWTransform* transform = gameObject()->getComponent<SWTransform>();
	SWMeshRenderer* renderer = gameObject()->addComponent<SWMeshRenderer>();
	SWMeshFilter* filter = gameObject()->addComponent<SWMeshFilter>();

	filter->setMesh( swrtti_cast<SWMesh>(SW_GC.findItem("unitRectMesh")) );
	renderer->setTexture( SW_GC.loadTexture( "ball.png" ) );
	transform->setLocalScale( SWVector3f::one * SWMath.randomInt( 5, 20 ) );

	lifeTime = 5;
}

void Ball::onUpdate()
{
	velocity += SWVector3f( 0, 0.982f*100, 0 )*SW_GC.deltaTime();
	SWTransform* transform = gameObject()->getComponent<SWTransform>();
	SWVector3f pos = transform->getLocalPosition();
	transform->setLocalPosition( pos + (velocity*SW_GC.deltaTime()) );
	if ( ( lifeTime -= SW_GC.deltaTime() ) < 0 ) gameObject()->destroy();
}
