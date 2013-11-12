#include "BallGenerator.h"
#include "SWVector3f.h"
#include "SWGameObject.h"
#include "Ball.h"
#include "SWGameContext.h"
#include "SWTransform.h"
#include "SWMath.h"

void BallGenerator::onAwake()
{
	accum = 0;
	turnOn = false;
}

void BallGenerator::onUpdate()
{
	if ( !turnOn ) return;
	accum -= SW_GC.deltaTime();
	if ( accum <= 0 )
	{
		accum = 0.1f;

		SWGameObject* ballGO = new SWGameObject;
		Ball* ball = ballGO->addComponent<Ball>();
		SWTransform* ballTrans = ballGO->getComponent<SWTransform>();
		SWTransform* transform = gameObject()->getComponent<SWTransform>();

		ballGO->setName( "ball" );
		ball->velocity = SWVector3f( force.x, force.y, 0 ) * (float)SWMath.randomInt(100,100)/100.0f;

		ballTrans->setLocalPosition( transform->getLocalPosition() * transform->getWorldMatrix() );
	}
}
