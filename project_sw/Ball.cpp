#include "Ball.h"
#include "SWTransform.h"
#include "SWGameContext.h"
#include "SWGameObject.h"
#include "SWMeshRenderer.h"
#include "SWMeshFilter.h"
#include "SWMesh.h"
#include "SWVector2f.h"

void Ball::onAwake()
{
	SWTransform* transform = gameObject()->getComponent<SWTransform>();
	SWMeshRenderer* renderer = gameObject()->addComponent<SWMeshRenderer>();
	SWMeshFilter* filter = gameObject()->addComponent<SWMeshFilter>();

	SWVector3f vertices[] = { SWVector3f(0,0,0), SWVector3f(1,0,0), SWVector3f(0,1,0), SWVector3f(1,1,0) };
	SWVector2f texCoords[] = { SWVector2f(0,0), SWVector2f(1,0), SWVector2f(0,1), SWVector2f(1,1) };
	unsigned short indices[] = {0,1,2,3,2,1};
	SWMesh* mesh = new SWMesh();
	mesh->setVertexStream( 4, &vertices[0] );
	mesh->setTexCoordStream( 4, &texCoords[0]);
	mesh->setIndexStream( 6, &indices[0] );

	filter->setMesh( mesh );
	renderer->setTexture( SW_GC.loadTexture( "unit1.png" ) );
	transform->setLocalScale( SWVector3f(10,10,1) );
	transform->setLocalPosition( SWVector3f( 500, 500, 0 ) );

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
