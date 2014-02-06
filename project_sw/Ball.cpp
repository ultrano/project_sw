#include "Ball.h"
#include "SWTransform.h"
#include "SWGameContext.h"
#include "SWGameObject.h"
#include "SWMeshRenderer.h"
#include "SWMeshFilter.h"
#include "SWMesh.h"
#include "TVector2f.h"
#include "SWMath.h"
#include "SWTime.h"
#include "SWAction.h"
#include "SWActDestroy.h"
#include "TQuaternion.h"
#include "SWMath.h"

void Ball::onAwake()
{
	SWTransform* transform = gameObject()->getComponent<SWTransform>();
	SWMeshRenderer* renderer = gameObject()->addComponent<SWMeshRenderer>();
	SWMeshFilter* filter = gameObject()->addComponent<SWMeshFilter>();
	SWAction* action = gameObject()->addComponent<SWAction>();

	filter->setMesh( swrtti_cast<SWMesh>( gameObject()->getProp("mesh") ) );
	renderer->setTexture( SW_GC.loadTexture( "ball.png" ) );
	transform->setLocalScale( TVector3f::one * SWMath.randomInt( 5, 20 ) );
	action->runAct( new SWActDestroy( 5 ) );

}

void Ball::onUpdate()
{
	float deltaTime = SWTime.getDeltaTime();
	velocity += TVector3f( 0, 0.982f*100, 0 )*deltaTime;
	SWTransform* transform = gameObject()->getComponent<SWTransform>();
	TVector3f pos = transform->getLocalPosition();
	
	float radian = SWMath.atan( velocity.y, velocity.x );
	TQuaternion rotQuat;
	transform->setLocalRotate( rotQuat.rotate( TVector3f::axisZ, radian ) );
	transform->setLocalPosition( pos + (velocity*deltaTime) );
}
