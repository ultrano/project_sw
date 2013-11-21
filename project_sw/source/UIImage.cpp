#include "UIImage.h"
#include "SWGameObject.h"
#include "SWMeshRenderer.h"
#include "SWMeshFilter.h"
#include "SWMesh.h"
#include "SWVector3f.h"
#include "SWVector2f.h"
#include "SWGameContext.h"

#include "UIDefines.h"

void UIImage::onAwake()
{
	m_updateMesh = false;
	SWVector3f vertices[] = { SWVector3f(-0.5f,-0.5f,0), SWVector3f(0.5f,-0.5f,0), SWVector3f(-0.5f,0.5f,0), SWVector3f(0.5f,0.5f,0) };
	SWVector2f texCoords[] = { SWVector2f(0,0), SWVector2f(1,0), SWVector2f(0,1), SWVector2f(1,1) };
	unsigned short indices[] = {0,1,2,3,2,1};
	SWMesh* mesh = new SWMesh();
	mesh->setVertexStream( 4, &vertices[0] );
	mesh->setTexCoordStream( 4, &texCoords[0]);
	mesh->setIndexStream( 6, &indices[0] );
	
	setAlign( UI_Center, UI_Center );
	gameObject()->addComponent<SWMeshFilter>()->setMesh( mesh );
	gameObject()->addComponent<SWMeshRenderer>();
	gameObject()->addUpdateDelegate( GetDelegate( onUpdate ) );
}

void UIImage::onRemove()
{
	gameObject()->removeUpdateDelegate( GetDelegate( onUpdate ) );
}

void UIImage::onUpdate()
{
	if ( !m_updateMesh ) return;
	m_updateMesh = false;

	SWMesh* mesh = gameObject()->getComponent<SWMeshFilter>()->getMesh();
	
	SWVector2f anchor;
	switch ( m_alignV )
	{
	case UI_Center : anchor.y = 0.5f; break;
	case UI_Top    : anchor.y = 1.0f; break;
	case UI_Bottom : anchor.y = -1.0f; break;
	}
	switch ( m_alignH )
	{
	case UI_Center : anchor.x = 0.5f; break;
	case UI_Left   : anchor.x = 1.0f; break;
	case UI_Right  : anchor.x = -1.0f; break;
	}
	SWVector3f vertices[] = 
	{ SWVector3f(-anchor.x*m_width, -anchor.y*m_height, 0)
	, SWVector3f(+anchor.x*m_width, -anchor.y*m_height, 0)
	, SWVector3f(-anchor.x*m_width, +anchor.y*m_height, 0)
	, SWVector3f(+anchor.x*m_width, +anchor.y*m_height, 0) };
	mesh->setVertexStream( 4, &vertices[0] );
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
	m_updateMesh = true;
}
void UIImage::setTexture( const std::string& filePath )
{
	gameObject()->getComponent<SWMeshRenderer>()->setTexture( SW_GC.loadTexture( filePath ) );
}

void UIImage::setAlignV( int align )
{
	m_alignV = align;
	m_updateMesh = true;
}

void UIImage::setAlignH( int align )
{
	m_alignH = align;
	m_updateMesh = true;
}

void UIImage::setAlign( int alignV, int alignH )
{
	m_alignV = alignV;
	m_alignH = alignH;
	m_updateMesh = true;
}
