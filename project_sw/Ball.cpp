#include "Ball.h"
#include "SWTransform.h"
#include "SWGameContext.h"
#include "SWGameObject.h"
#include "SWMeshRenderer.h"
#include "SWMeshFilter.h"
#include "SWMesh.h"
#include "SWVector2f.h"
#include "SWMath.h"
#include "SWTime.h"
#include "SWAction.h"
#include "SWActDestroy.h"
#include "SWQuaternion.h"
#include "SWMath.h"

void Ball::onAwake()
{
	SWTransform* transform = gameObject()->getComponent<SWTransform>();
	SWMeshRenderer* renderer = gameObject()->addComponent<SWMeshRenderer>();
	SWMeshFilter* filter = gameObject()->addComponent<SWMeshFilter>();
	SWAction* action = gameObject()->addComponent<SWAction>();

	filter->setMesh( swrtti_cast<SWMesh>( gameObject()->getProp("mesh") ) );
	renderer->setTexture( SW_GC.loadTexture( "ball.png" ) );
	transform->setLocalScale( SWVector3f::one * SWMath.randomInt( 5, 20 ) );
	action->setAct( new SWActDestroy( 5 ) );

}

void Ball::onUpdate()
{
	float deltaTime = SWTime.getDeltaTime();
	velocity += SWVector3f( 0, 0.982f*100, 0 )*deltaTime;
	SWTransform* transform = gameObject()->getComponent<SWTransform>();
	SWVector3f pos = transform->getLocalPosition();
	
	float radian = SWMath.atan( velocity.y, velocity.x );
	SWQuaternion rotQuat;
	transform->setLocalRotate( rotQuat.rotate( SWVector3f::axisZ, radian ) );
	transform->setLocalPosition( pos + (velocity*deltaTime) );
}
