#include "BallGenerator.h"
#include "SWVector3f.h"
#include "SWTriangle.h"
#include "SWGameObject.h"
#include "Ball.h"
#include "SWGameContext.h"
#include "SWTransform.h"
#include "SWMath.h"
#include "SWTime.h"
#include "SWMesh.h"

void BallGenerator::onAwake()
{
	accum = 0;
	turnOn = false;

	SWVector3f vertices[] = { SWVector3f(-0.5f,-0.5f,0), SWVector3f(0.5f,-0.5f,0), SWVector3f(-0.5f,0.5f,0), SWVector3f(0.5f,0.5f,0) };
	SWVector2f texCoords[] = { SWVector2f(0,0), SWVector2f(1,0), SWVector2f(0,1), SWVector2f(1,1) };
	SWTriangle indices[] = { SWTriangle(0,1,2), SWTriangle(3,2,1) };
	SWMesh* mesh = new SWMesh();
	mesh->setVertexStream( 4, &vertices[0] );
	mesh->setTexCoordStream( 4, &texCoords[0]);
	mesh->setTriangleStream( 2, &indices[0] );

	gameObject()->defineProp( "mesh" );
	gameObject()->setProp( "mesh", mesh );
}

void BallGenerator::onUpdate()
{
	if ( !turnOn ) return;
	accum -= SWTime.getDeltaTime();
	if ( accum <= 0 )
	{
		accum = 0.01f;

		SWGameObject* ballGO = new SWGameObject;
		ballGO->setName( "ball" );
		ballGO->defineProp( "mesh" );
		ballGO->setProp( "mesh", gameObject()->getProp( "mesh" ) );
		Ball* ball = ballGO->addComponent<Ball>();
		SWTransform* ballTrans = ballGO->getComponent<SWTransform>();
		SWTransform* transform = gameObject()->getComponent<SWTransform>();

		ball->velocity = SWVector3f( force.x, force.y, 0 ) * (float)SWMath.randomInt(100,100)/100.0f;

		ballTrans->setLocalPosition( transform->getLocalPosition() * transform->getWorldMatrix() );
	}
}
