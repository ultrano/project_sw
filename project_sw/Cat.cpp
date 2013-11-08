#include "Cat.h"
#include "SWGameContext.h"
#include "SWGameObject.h"
#include "SWMeshRenderer.h"
#include "SWMeshFilter.h"
#include "SWMesh.h"
#include "SWVector2f.h"
#include "SWVector3f.h"
#include "SWTransform.h"

void Cat::onAwake()
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
	renderer->setTexture( SW_GC.loadTexture( "cat1.png" ) );
	transform->setLocalScale( SWVector3f(100,100,1) );
	transform->setLocalPosition( SWVector3f( 1000, 1000, 0 ) );
}

void Cat::onStart()
{

}

void Cat::onUpdate()
{
	SWTransform* transform = gameObject()->getComponent<SWTransform>();
	SWVector3f pos = transform->getLocalPosition();
	pos.x -= SW_GC.deltaTime() * 10;
	transform->setLocalPosition( pos );
	transform->setLocalRotate( SWQuaternion().rotate(SWVector3f::axisZ, 3.14f/2.0f ) );
}
