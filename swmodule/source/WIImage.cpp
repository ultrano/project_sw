#include "WIImage.h"
#include "SWGameObject.h"
#include "SWMeshRenderer.h"
#include "SWMeshFilter.h"
#include "SWMesh.h"
#include "TVector3f.h"
#include "TVector2f.h"
#include "TIndex3.h"
#include "SWGameContext.h"

#include "WIDefines.h"

void WIImage::onAwake()
{
	m_updateVert = false;
	m_updateTex  = false;
	TVector3f vertices[] = { TVector3f(-0.5f,-0.5f,0), TVector3f(-0.5f, 0.5f,0), TVector3f(0.5f, 0.5f,0), TVector3f(0.5f,-0.5f,0) };
	TVector2f texCoords[] = { TVector2f(0,0), TVector2f(1,0), TVector2f(0,1), TVector2f(1,1) };
	TIndex3 indices[] = { TIndex3(0,1,2), TIndex3(3,2,1) };
	SWMesh* mesh = new SWMesh();
	mesh->setVertexStream( 4, &vertices[0] );
	mesh->setTexCoordStream( 4, &texCoords[0]);
	mesh->setTriangleStream( 2, &indices[0] );
	
	setAlign( UI_Center, UI_Center );
	gameObject()->addComponent<SWMeshFilter>()->setMesh( mesh );
	gameObject()->addComponent<SWMeshRenderer>();
	gameObject()->addUpdateDelegator( GetDelegator( onUpdate ) );
}

void WIImage::onRemove()
{
	gameObject()->removeUpdateDelegator( GetDelegator( onUpdate ) );
}

void WIImage::onUpdate()
{
	if ( m_updateVert )
	{
		m_updateVert = false;

		TVector2f anchor;
		switch ( m_alignV )
		{
		case UI_Center : anchor.x = 0; break;
		case UI_Left   : anchor.x = +0.5f; break;
		case UI_Right  : anchor.x = -0.5f; break;
		}
		switch ( m_alignH )
		{
		case UI_Center : anchor.y = 0; break;
		case UI_Top    : anchor.y = +0.5f; break;
		case UI_Bottom : anchor.y = -0.5f; break;
		}

		TVector3f vertices[] = 
		{ TVector3f((anchor.x-0.5f)*m_width, (anchor.y+0.5f)*m_height, 0)
		, TVector3f((anchor.x+0.5f)*m_width, (anchor.y+0.5f)*m_height, 0)
		, TVector3f((anchor.x-0.5f)*m_width, (anchor.y-0.5f)*m_height, 0)
		, TVector3f((anchor.x+0.5f)*m_width, (anchor.y-0.5f)*m_height, 0) };
		
		SWMesh* mesh = gameObject()->getComponent<SWMeshFilter>()->getMesh();
		mesh->setVertexStream( 4, &vertices[0] );
	}

	if ( m_updateTex )
	{
		m_updateTex = false;
		
		float left   = m_uvRect.left   / m_width;
		float top    = m_uvRect.top    / m_height;
		float right  = m_uvRect.right  / m_width;
		float bottom = m_uvRect.bottom / m_height;
		TVector2f texCoords[] = 
		{ TVector2f( left , top)
		, TVector2f( right, top)
		, TVector2f( left , bottom)
		, TVector2f( right, bottom) };
		
		SWMesh* mesh = gameObject()->getComponent<SWMeshFilter>()->getMesh();
		mesh->setTexCoordStream( 4, &texCoords[0]);
	}
}

void WIImage::setSizeToTexture( float scaleW, float scaleH )
{
	unsigned int texID = gameObject()->getComponent<SWMeshRenderer>()->getTexture();
	if ( texID == 0 ) return;
	int width, height;
	SW_GC.getTextureSize( texID, width, height );
	scaleW = (scaleW == 0)? 1:scaleW;
	scaleH = (scaleH == 0)? 1:scaleH;
	setSize( width*scaleW, height*scaleH );
}

void WIImage::setSize( float width, float height )
{
	m_width = width;
	m_height = height;
	m_updateVert = true;
}
void WIImage::setTexture( const tstring& filePath )
{
	gameObject()->getComponent<SWMeshRenderer>()->setTexture( SW_GC.loadTexture( filePath ) );
}

void WIImage::setAlignV( int align )
{
	m_alignV = align;
	m_updateVert = true;
}

void WIImage::setAlignH( int align )
{
	m_alignH = align;
	m_updateVert = true;
}

void WIImage::setAlign( int alignV, int alignH )
{
	m_alignV = alignV;
	m_alignH = alignH;
	m_updateVert = true;
}

void WIImage::setUVRect( float left, float top, float right, float bottom )
{
	m_uvRect.setRect( left, top, right, bottom );
	m_updateTex = true;
}