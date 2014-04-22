#include "WIImage.h"
#include "SWGameObject.h"
#include "SWMeshRenderer.h"
#include "SWMeshFilter.h"
#include "SWMaterial.h"
#include "SWMesh.h"
#include "TVector3f.h"
#include "TVector2f.h"
#include "TIndex3.h"
#include "SWAssets.h"
#include "SWTexture.h"

#include "WIDefines.h"


WIImage::WIImage()
	: m_width( 0 )
	, m_height( 0 )
	, m_uvRect( 0, 0, 0, 0 )
	, m_alignV( UI_Center )
	, m_alignH( UI_Center )
	, m_updateVert( false )
	, m_updateTex( false )
	, m_color(1,1,1,1)
{

}

WIImage::WIImage( factory_constructor )
	: m_width( 0 )
	, m_height( 0 )
	, m_uvRect( 0, 0, 0, 0 )
	, m_alignV( UI_Center )
	, m_alignH( UI_Center )
	, m_updateVert( false )
	, m_updateTex( false )
	, m_color(1,1,1,1)
{

}

WIImage::~WIImage()
{

}

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
	gameObject()->addUpdateDelegator( GetDelegator( onUpdate ) );

	gameObject()->addComponent<SWMeshFilter>()->setMesh( mesh );
	SWMeshRenderer* renderer = gameObject()->addComponent<SWMeshRenderer>();
	renderer->addPreRenderDelegate( GetDelegator( onPreRender ) );

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

		float width  = m_texture()->getWidth();
		float height = m_texture()->getHeight();

		float left   = m_uvRect.left   / width;
		float top    = m_uvRect.top    / height;
		float right  = m_uvRect.right  / width;
		float bottom = m_uvRect.bottom / height;
		TVector2f texCoords[] = 
		{ TVector2f( left , top)
		, TVector2f( right, top)
		, TVector2f( left , bottom)
		, TVector2f( right, bottom) };

		SWMesh* mesh = gameObject()->getComponent<SWMeshFilter>()->getMesh();
		mesh->setTexCoordStream( 4, &texCoords[0]);
	}
}

void WIImage::onPreRender()
{
	SWMeshRenderer* renderer = getComponent<SWMeshRenderer>();
	if ( renderer == NULL ) return;

	SWMaterial* material = renderer->getMaterial();
	if ( material == NULL ) return;

	material->setTexture( "TEXTURE_0", m_texture() );
	material->setVector4( "COLOR", tquat( m_color.r, m_color.g, m_color.b, m_color.a ) );
}

void WIImage::setSizeToTexture( float scaleW, float scaleH )
{
	if ( m_texture() == NULL ) return;

	scaleW = (scaleW == 0)? 1:scaleW;
	scaleH = (scaleH == 0)? 1:scaleH;
	setSize( m_texture()->getWidth()*scaleW, m_texture()->getHeight()*scaleH );
}

void WIImage::setSize( float width, float height )
{
	m_width = width;
	m_height = height;
	m_updateVert = true;
}
void WIImage::setTexture( SWTexture* texture )
{
	m_texture = texture;
}

void WIImage::setTexture( SWHardRef<SWTexture> texture )
{
	m_texture = texture;
}

SWTexture* WIImage::getTexture() const
{
	return m_texture();
}

void WIImage::setColor( float r, float g, float b, float a )
{
	m_color = tcolor( r,g,b,a );
}

void WIImage::setColor( const tcolor& color )
{
	m_color = color;
}

const tcolor& WIImage::getColor() const
{
	return m_color;
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

void WIImage::setUVRect( float x, float y, float width, float height )
{
	m_uvRect.setRect( x, y, x+width, y+height );
	m_updateTex = true;
}