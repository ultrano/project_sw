#include "UIImage.h"
#include "SWGameObject.h"
#include "SWMeshRenderer.h"
#include "SWMeshFilter.h"
#include "SWMesh.h"
#include "SWVector3f.h"
#include "SWVector2f.h"
#include "SWGameContext.h"

void UIImage::onAwake()
{
	SWVector3f vertices[] = { SWVector3f(-0.5f,-0.5f,0), SWVector3f(0.5f,-0.5f,0), SWVector3f(-0.5f,0.5f,0), SWVector3f(0.5f,0.5f,0) };
	SWVector2f texCoords[] = { SWVector2f(0,0), SWVector2f(1,0), SWVector2f(0,1), SWVector2f(1,1) };
	unsigned short indices[] = {0,1,2,3,2,1};
	SWMesh* mesh = new SWMesh();
	mesh->setVertexStream( 4, &vertices[0] );
	mesh->setTexCoordStream( 4, &texCoords[0]);
	mesh->setIndexStream( 6, &indices[0] );
	
	gameObject()->addComponent<SWMeshFilter>()->setMesh( mesh );
	gameObject()->addComponent<SWMeshRenderer>();
}

void UIImage::setSizeToTexture( float scaleW, float scaleH )
{
	unsigned int texID = gameObject()->getComponent<SWMeshRenderer>()->getTexture();
	if ( texID == 0 ) return;
	int width, height;
	SW_GC.getTextureSize( texID, width, height );
	scaleW = (scaleW == 0)? 1:scaleW;
	scaleH = (scaleH == 0)? 1:scaleH;
	setSize( width*scaleW, height*scaleH );
}

void UIImage::setSize( float width, float height )
{
	m_width = width;
	m_height = height;
	SWMesh* mesh = gameObject()->getComponent<SWMeshFilter>()->getMesh();
	
	SWVector3f vertices[] = 
	{ SWVector3f(-0.5f*width, -0.5f*height, 0)
	, SWVector3f(+0.5f*width, -0.5f*height, 0)
	, SWVector3f(-0.5f*width, +0.5f*height, 0)
	, SWVector3f(+0.5f*width, +0.5f*height, 0) };
	mesh->setVertexStream( 4, &vertices[0] );
}
void UIImage::setTexture( const std::string& filePath )
{
	gameObject()->getComponent<SWMeshRenderer>()->setTexture( SW_GC.loadTexture( filePath ) );
}